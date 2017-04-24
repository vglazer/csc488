
/**********************************************************/
/*							                              */
/*    astDump.c						                      */
/*    Abstract Syntax Tree dump module                    */
/*    for the CSC488S Project 			                  */
/*							                              */
/**********************************************************/


/********************************************************/
/*   Include common definitions				            */
/********************************************************/

#include "common.h"

/********************************************************/
/*   External Definitions				                */
/********************************************************/

#include "globalvars.h"

/********************************************************/
/*   Include interface to this module			        */
/********************************************************/

#include "ast.h"

/********************************************************/
/*   Definitions for the Abstract Syntax Tree		    */
/********************************************************/

/* removed: feb 13 newsgroup instructions
   #include "astDef.h" */

/********************************************************/
/*   Internal functions defined in this file		    */
/********************************************************/

static void printObject( const ObjectP tree, int indn ) ;
static void printDecl(const DeclP decl, int indn ) ;
static void printScope(const ScopeP scope, int indn ) ;
static void printStmt(const StmtP stmt, int indn ) ;
static void printExpn( const ExpnP expn, int indn  ) ;
static void printLiteral( const LitObj lit ) ;
static void printArgs( const ArgsP args , int indn );
static void printExpnTree( const ExpnP expn , int indn );

/********************************************************/
/*   Internal data for this module                      */
/********************************************************/

/*  data for indentation mechanism in astDump		*/
	/* step size (in characters) for indentation	*/
#define INDENT_STEP  ( 4 )
	/* maximum number of indent levels		*/
#define INDENT_MAX   ( 12 )

	/* indentation table for efficient indenting 	*/
static char * indents[ INDENT_MAX + 1 ] ;
static char inBlanks[ INDENT_STEP * INDENT_MAX + 1 ] ;

#define SETINDENT( ind ) \
       (indents[ ( ind <= INDENT_MAX ? ind : INDENT_MAX )] )
       
/****************************************************************/
/*	Tables of names used for labelling output		            */
/*	NOTE that the order of the entries in these arrays	        */
/*      MUST be the same as the order of enum definitions 	    */
/* 	in ast.h						                            */
/*								                                */
/*	Also define macros for safe access to these tables	        */
/****************************************************************/

/*  Table of names for data types, index by DataType 	*/       
char * typeNames[] = { "unknown", "boolean", "int", "text", "void" };

#define TYPENAME( typ ) \
	( typeNames[( Dnone <= typ && typ <= Dvoid ? typ : Dnone )] )
       
/*  Table of names for statements, indexed by StmtType   */
char * stmtNames[] = { "unknown", "assignment", "if", "if+else", "while", 
	"do-until", "exit",  "return", "result", "put", "get", "assert", "proc call", 
	"scope", "for" };

#define STMTNAME( st ) \
	( stmtNames[( Snone <= st && st <= Sfor ? st : Snone )] )

/*  Table of names for operators, indexed by OperType   */
char * operNames[] = {	"?", "&", "|", "!", 	          /* unknown, boolean */
			"=", "#", "<", "<=", ">", ">=" ,  /* comparisons */
			"-", "+", "-", "*", "/", "[]" } ; /* arithmetic, subscript */

#define OPERNAME( op ) \
	( operNames[( Onop <= op && op <= Osubs ? op : Onone )] )

/*  Table of names for declarations types, index by DeclType 	*/       
char * declNames[] = { "unknown", "scalar",  "array", "function", 
			"procedure", "forward function", "forward procedure" };

#define DCNAME( dcl ) \
	( declNames[( DCnone <= dcl && dcl <= DCforward_proc ? dcl : DCnone )] )
       
       
static BOOLEAN initNeeded = TRUE ;

/********************************************************/
/*   Function definitions                               */
/********************************************************/

/********************************************/
/*  astDump - print AST to file dumpFile    */
/********************************************/
void astDump( const ASTtype  dumpTree)
{
  int  i ;
  char * indp ;
  
  if( initNeeded ) {
	/*  set up indentation table for astDump */
	for( indp = &inBlanks[0] , i = 0 ; i < INDENT_STEP * INDENT_MAX ; i++ )
	    *indp++ = ' ' ;		/* fill inBlanks with blanks */
	*indp = '\0' ;		/* null terminate inBlanks */
  
	assert( indp == &inBlanks[ INDENT_STEP * INDENT_MAX ] );

	/* NOTE indp now points at the end of inBlanks */
	for( i = 0 ; i <= INDENT_MAX ;  i++ , indp -= INDENT_STEP )
	    indents[ i ] = indp ;       

	assert( indp + INDENT_STEP >= &inBlanks[0] ) ;
	
	initNeeded = FALSE ;
  };  
     
  fprintf (dumpFile, "\n*** Abstract Syntax Tree ***\n");
  if (dumpTree == NULL)
    fprintf (dumpFile, "\tThe tree is empty.\n");
  else
    printObject((ObjectP) dumpTree, 0 );
  fprintf(dumpFile, "\n*** End Abstract Syntax Tree ***\n");
}


/*********************************************/
/* Internal function definitions             */
/*********************************************/

/********************************************************/
/*  objectPrint -  print a list of objects		        */
/*                 starting with indentation indn	    */
/********************************************************/

static void printObject(const ObjectP tree, int indn ) {
  char * indent = SETINDENT( indn );	/* indentation block */
  ObjectP trace = tree ;
  
  while( trace ){

    /* objects are enclosed by {} */
    fprintf (dumpFile, "%s{ (%p) \n", indent, (void *)trace  );
    
    /*************************************/
    /* print object type and information */
    /*************************************/
    switch (trace->otype) {
    
      case Odecl:	printDecl( trace->ovalue.decl, indn + 1 ); 
        		break;
        		
      case Ostmt:	printStmt( trace->ovalue.stmt, indn + 1 );
        		break;
        		
      case Oexpn:	printExpn( trace->ovalue.expn, indn + 1 );
	        	break;
	        	
      case Oscope: 	printScope( trace->ovalue.scope, indn + 1 );
      			break ;

      default:		assert( FALSE );	/* unknown type */
      			
    }
    
    fprintf (dumpFile, "%s}\n", indent);
    
    /* do next object at this level */
    trace = trace->onext ;
  }
}

/************************************************/
/* 	Print declaration 			                */
/************************************************/

static void printDecl(DeclP decl, int indn ) {
   char * indent = SETINDENT( indn );	/* indentation block */
   FparmP fparms ;

   if ( !decl ) 
       return ;
   fprintf (dumpFile, "%s Declaration - %s (%p) \n", 
   	indent, DCNAME( decl->dctype ), (void *)decl );
   fprintf(dumpFile, "%s Line: %i\n", indent, decl->line);

   fprintf(dumpFile, "%s ", indent );	/* indent declaration */

   /* print type in declaration and ID */
#if 0
   if( decl->dctype == DCfunc ) 
#endif
       fprintf(dumpFile, "%s ", TYPENAME( decl->dtype ) );
   fprintf (dumpFile, "%s ", decl->dname);
   if (decl->dctype == DCarray )      /* array */
       fprintf (dumpFile, "[ %d ] " , decl->dbound );
   if(     decl->dctype == DCfunc 
        || decl->dctype == DCproc
        || decl->dctype == DCforward_func 
        || decl->dctype == DCforward_proc  ){
       /* print formal parameters (if any) */
       fprintf (dumpFile, "(");
       fparms = decl->dfparm ;
       while (fparms ) {
   	 fprintf (dumpFile, "%s : %s",
   	       fparms->fname, TYPENAME( fparms->ftype ));
   	 fparms = fparms->fnext;
   	 if (fparms != NULL) fprintf (dumpFile, ", ");
   	 else fprintf (dumpFile, ")\n");
     }
     /* print object associated */
     if (decl->dobject ){
       fprintf(dumpFile,"%s Associated Object\n", indent);
       printObject(decl->dobject, indn + 1);
     }
   }
#if 0
else	/* scalar variable or array */ 
       fprintf(dumpFile, " : %s\n", TYPENAME( decl->dtype ) );
#endif
   fprintf(dumpFile, "%s Containing Scope pointer (%p)\n",
   	indent, (void *)decl->dscope );
   /* print initexpn */
   if (decl->dinitexpn ){
     fprintf(dumpFile,"%s Init: \n", indent);
     printExpn(decl->dinitexpn, indn + 1);
   } 
   fprintf (dumpFile, "%s End Declaration\n", indent);
}

/************************************************/
/*	Print scope 				                */
/************************************************/

static void printScope(ScopeP scope, int indn ) {
   char * indent = SETINDENT( indn );	/* indentation block */

   if ( !scope ) 
       return ;
   fprintf (dumpFile, "%s Scope (%p)\n", indent, (void *)scope );
   /* Don't print parent scope, we've probably been there already */
   fprintf(dumpFile,"%s    Parent pointer = %p \n", 
   	indent, (void *)scope->scparent );
   printObject( scope->scbody, indn + 1 );
   fprintf (dumpFile, "%s End Scope\n", indent);
}


/************************************************/
/*	Print statement	 			                */
/************************************************/
    
static void printStmt(const StmtP stmt, int indn ) {
   char * indent = SETINDENT( indn );	/* indentation block */

   if ( !stmt ) 
       return ;
   fprintf (dumpFile, "%s %s Statement (%p)", indent ,
   	STMTNAME( stmt-> stype ), (void *)stmt );
   if (stmt->pname != NULL) {
       fprintf (dumpFile, " : %s", stmt->pname);
   }
   fprintf (dumpFile, "\n"); 

   fprintf(dumpFile, "%s Line: %i\n", indent, stmt->line);

   printExpn( stmt->sexpn1 , indn + 1 );
   printExpn( stmt->sexpn2 , indn + 1 );
   printExpn( stmt->sexpn3 , indn + 1 );
   printObject( stmt->sstmt1 , indn + 1 );
   printObject( stmt->sstmt2 , indn + 1 );
   if( stmt->sargs ) {
   	fprintf(dumpFile, "%s  Related arguments: ", indent );
	printArgs( stmt->sargs , indn  );
	fprintf(dumpFile,"\n");
   }
   if( stmt->sscope ) {
   	fprintf(dumpFile, "%s  Related scope: ", indent );
	printScope( stmt->sscope , indn + 1 );
   }
   
}
        
/************************************************/
/*	print literal value 			            */
/************************************************/

static void printLiteral(LitObj lit ) {
  
  switch (lit.ltype)
    {
      case Dint:
      case Dbool:
	fprintf (dumpFile, "%d", lit.lvalue.num);
	break;
      case Dtext:
	fprintf (dumpFile, "%s", lit.lvalue.str);
	break;
      case Dvoid:
	fprintf (dumpFile, "void");
	break;
      case Dnone:
	fprintf (dumpFile, "unknown");
	break;
      default:
         assert( FALSE );
    }
}



/************************************************/
/*	Print argument list			                */
/************************************************/

static void printArgs( const ArgsP args , int indn ) {
   ArgsP trace = args ;

   while ( trace ){
	printExpnTree( trace->avalue , indn  );
	trace = trace->anext ;
	if( trace )
	    fprintf(dumpFile, ", ");
   }       
}


/************************************************/
/*	Print expression 			                */
/************************************************/

static void printExpn(const ExpnP expn, int indn ) {

   if ( !expn ) 
       return ;
   fprintf (dumpFile, "%s Expression (%p)\t", SETINDENT( indn ), (void *)expn );
   printExpnTree( expn, indn );
   fprintf(dumpFile,"\n");
  
}

/************************************************/
/*	Print expression tree			            */
/*	ASSUMES all indentation done by caller	    */
/*	ASSUMES expression will fit on one line	    */
/************************************************/

static void printExpnTree(const ExpnP expn , int indn ) {
   if ( !expn ) 
       return ;

   switch( expn->etype ){
   
   case Enone:	fprintf(dumpFile," Unknown" );
   		break ;
		
   case Eunary:   fprintf(dumpFile," %s (", OPERNAME( expn->evalue.eop.eoper ));
   		printExpnTree( expn->evalue.eop.eopright, indn );
		fprintf(dumpFile, ")" );
		break ;
	
   case Ebinary:   if( expn->evalue.eop.eoper != Osubs ) {
   		    /* all binary operators except subscript */
		    fprintf(dumpFile, "(" );
		    printExpnTree( expn->evalue.eop.eopleft, indn );
		    fprintf( dumpFile, " %s ", OPERNAME( expn->evalue.eop.eoper));
		    printExpnTree( expn->evalue.eop.eopright, indn );
		    fprintf(dumpFile, ")" );
		}else {
		    /* Print subscript expression */
		    printExpnTree( expn->evalue.eop.eopleft, indn );
		    fprintf(dumpFile, " [ " );
		    printExpnTree( expn->evalue.eop.eopright , indn );
		    fprintf(dumpFile, " ] " );
		}
		break ;
		
   case Efcall: fprintf(dumpFile, "%s(", expn->evalue.efn.efname );
   		printArgs( expn->evalue.efn.efargs , indn  );
		fprintf(dumpFile, ")" );
		break ;
		
   case Econst: printLiteral( expn->evalue.elitVal );
   		break ;
		
   case Evar:   fprintf(dumpFile, "%s", expn->evalue.eident );
   		break ;
	
   case Econd:  fprintf(dumpFile, "(" );	/* conditional expression */
		printExpnTree( expn->evalue.econx.control, indn );
		fprintf(dumpFile, " ? " );
		printExpnTree( expn->evalue.econx.contrue, indn );
		fprintf(dumpFile, " : " );
		printExpnTree( expn->evalue.econx.confalse , indn );
		fprintf(dumpFile, ") " );
   		break ;
    
   case Eskip:  fprintf(dumpFile, "skip");
                break;

   default:
   		assert( FALSE );
   };
}

/*********************************/
/*        end of file astDump.c  */
/*********************************/

