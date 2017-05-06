
/****************************************************************/
/*                                                              */
/*      semantics.c                                             */
/*      Semantics Analysis Module for CSC488S Project Compiler  */
/*                                                              */
/****************************************************************/


/****************************************************************/
/*  Include common definitions and global variables             */
/****************************************************************/
#include "common.h"
#include "globalvars.h"

/****************************************************************/
/*  Include interface to this module                            */
/****************************************************************/
#include "semantics.h"

/****************************************************************/
/*  Include interface to symbol table                           */
/****************************************************************/
#include "symbol.h"

#include "codegen.h"

/****************************************************************/
/*                                                              */
/*  Utility procedures used for semantic analysis               */
/*                                                              */
/****************************************************************/

/********************************************************/
/*  return the number of arguement                     */
/********************************************************/
int numArgs( ArgsP args )
{
    int count = 0;

    while ( args != NULL ) {
        count++;
        args = args->anext;
    }
    return count;
}

/****************************************************************/
/*  Write log messages                                          */
/****************************************************************/

void SLOG( char * msg )
{
    if ( traceSemantics == TRUE )
        fprintf( traceFile, "SEMANTICS: %s\n", msg );        
}

/****************************************************************/
/*  Semantic information to be propagated up the tree           */
/****************************************************************/

typedef struct commFlags_Tag {
    BOOLEAN insideLoop;     /* whether we're in a loop          */
    BOOLEAN insideFunc;     /* whether we're in a function      */
    BOOLEAN insideProc;     /* whether we're in a procedure     */
    BOOLEAN foundResult;
    DataType returnType;    /* return type of the function      */
} commFlags;

typedef struct commFlags_Tag *commFlagsP;

/****************************************************************/
/*  Initialize flags                                            */
/****************************************************************/

void initFlags( commFlagsP flags ) 
{
    flags->foundResult = FALSE;
    flags->insideLoop  = FALSE;
    flags->insideFunc  = FALSE;
    flags->insideProc  = FALSE;
    flags->returnType  = Dnone; 
}

/****************************************************************/
/*  Check routine signatures                                    */
/****************************************************************/
void checkSignature( FparmP params1 , FparmP params2, int line ); 

/****************************************************************/
/*  Check declarations                                          */
/****************************************************************/
void checkDecl( DeclP decl );

/****************************************************************/
/*  Check scope                                                 */
/****************************************************************/
void checkScope( ScopeP scope, commFlagsP flags );

/****************************************************************/
/*  Check statement                                             */
/****************************************************************/
void checkStmt( StmtP stmt, commFlagsP flags );

/****************************************************************/
/*  Check a list of statements                                  */
/****************************************************************/
void checkStmt_list( ObjectP stmts, commFlagsP flags );

/****************************************************************/
/*  Check expression                                            */
/*      returns DataType of expression                          */
/****************************************************************/
DataType checkExpn( ExpnP expn );

/****************************************************************/
/*  Check literal                                               */
/*      returns DataType of expression                          */
/****************************************************************/
DataType checkLiteral( LitObj lit );

/****************************************************************/
/*  Check formal parameters                                     */
/****************************************************************/
void checkFParam ( FparmP params );

/****************************************************************/
/*  Check arguments for a put statement                         */
/****************************************************************/
void checkPutArgs( ArgsP args );

/****************************************************************/
/*  Check arguments for a get statement                         */
/****************************************************************/
void checkGetArgs( ArgsP args );

/****************************************************************/
/*  types of errors                                             */
/****************************************************************/
typedef enum {
    EemptyTree,             /* tree is empty  ( DEBUG )           */
    EinvalidScalarType,     /* illegal scalar type ( DEBUG )      */
    EemptyScope,            /* scope has no statements ( DEBUG )  */
    EdeclAfterStatement,    /* declaration after statement ( DEBUG ) */
    EalreadyDeclared,       /* variable already declared        */
    EarrayOutOfBound,       /* declared size of array too large */
    EinvalidReturnType,     /* function return type is invalid  */
    EintegerOutOfRange,     /* literal integer is out of range  */
    EundeclaredVariable,    /* undeclared variable referenced   */
    EinvalidExpnType,       /* expression used improperly       */ 
    EtypeMismatch,          /* type mistmatch in expression     */
    EinvalidControl,        /* conditional control not boolean  */
    EinvalidExit,           /* exit occurs outside a loop       */
    EinvalidReturn,         /* return occurs outside procedure  */
    EinvalidResult,         /* result occurs outside function   */
    EinvalidPutArgs,        /* invalid put argument             */
    EinvalidGetArgs,        /* invalid get argument             */
    EillegalSubscripting,   /* non-array variable subscripted   */
    EillegalAssignment,     /* illegal assignment               */
    EexpectedSubscript,     /* array variable not subscripted   */
    EmissingResult,         /* function missing result statment */
    EundeclaredFunc,        /* undeclared function reference    */
    EundeclaredProc,        /* undeclared procedure reference   */
    EparamCount,            /* parameter count                  */             
    Eunknown                /* non-implemented error            */
} errors;

/****************************************************************/
/*  report error                                                */
/*      err_code - the error being reported                     */
/*      line     - location of error                            */
/*      ext_msg  - a string to customize error message          */
/****************************************************************/
void error_msg( errors err_code, int line, char * ext_msg ){
    errorOccurred = TRUE;               /* set global error flag */

    switch( err_code ){
        case EemptyTree:
            fprintf( errorFile, "ERROR: line %d: empty tree \n", line ); 
            break;
        case EalreadyDeclared:
            fprintf( errorFile, "ERROR: line %d: %s already declared\n", line, ext_msg ); 
            break;
        case EundeclaredVariable:
            fprintf( errorFile, "ERROR: line %d: undeclared variable: %s\n",
                    line, ext_msg ); 
            break;
        case EinvalidScalarType:
            fprintf( errorFile, "ERROR: line %d: invalid scalar type, must be bool or int\n", line ); 
            break;
        case EarrayOutOfBound:
            fprintf( errorFile, "ERROR: line %d: array %s size must be between %d and %d\n", line, ext_msg, MIN_ARR, MAX_ARR ); 
            break;
        case EinvalidReturnType:
            fprintf( errorFile, "ERROR: line %d: invalid return type, must be bool or int\n", line ); 
            break;
        case EintegerOutOfRange:
            fprintf( errorFile, "ERROR: line %d: integer must be between %d and %d\n", line, -MAX_INTEGER, MAX_INTEGER ); 
            break;
        case EinvalidExpnType:
            fprintf( errorFile, "ERROR: line %d: expected %s expression type\n", line, ext_msg ); 
            break;
        case EtypeMismatch:
            fprintf( errorFile, "ERROR: line %d: type mismatch in %s\n", line,
                    ext_msg ); 
            break;
        case EinvalidControl:
            fprintf( errorFile, "ERROR: line %d: expected boolean control in conditional expresion\n", line ); 
            break;
        case EinvalidExit:
            fprintf( errorFile, "ERROR: line %d: exit occured outside a loop\n", line ); 
            break;
        case EinvalidReturn:
            fprintf( errorFile, "ERROR: line %d: return occured outside a procedure\n", line ); 
            break;
        case EinvalidResult:
            fprintf( errorFile, "ERROR: line %d: result occured outside a function\n", line ); 
            break;
        case EinvalidPutArgs:
            fprintf( errorFile, "ERROR: line %d: put arguments can only be text, integer, or 'skip'\n", line ); 
            break;
        case EinvalidGetArgs:
            fprintf( errorFile, "ERROR: line %d: get arguments can only be integers\n", line ); 
            break;
        case EillegalSubscripting: 
            fprintf( errorFile, "ERROR: line %d: illegal subscripting of non-array variable\n", line ); 
            break;
        case EillegalAssignment:
            fprintf( errorFile, "ERROR: line %d: illegal assignment to %s\n", line, ext_msg ); 
            break;
        case EexpectedSubscript: 
            fprintf( errorFile, "ERROR: line %d: use of array variable as a scalar\n", line ); 
            break;
        case EemptyScope: 
            fprintf( errorFile, "ERROR: line %d: no statements in scope\n", line ); 
            break;
        case EdeclAfterStatement: 
            fprintf( errorFile, "ERROR: line %d: declaration after statement block \n", line ); 
            break;
        case EmissingResult: 
            fprintf( errorFile, "ERROR: line %d: function missing result statment \n", line ); 
            break;
        case EundeclaredFunc: 
            fprintf( errorFile, "ERROR: line %d: undeclared function: %s \n", line, ext_msg ); 
            break;
        case EundeclaredProc: 
            fprintf( errorFile, "ERROR: line %d: undeclared procedure: %s \n", line, ext_msg ); 
            break;
        case EparamCount: 
            fprintf( errorFile, "ERROR: line %d: too few/many arguments in %s call\n", line, ext_msg ); 
            break;

        default:
            fprintf( errorFile, "ERROR: line %d: unknown \n", line ); 
            break;
    }
}

/****************************************************************/
/*                                                              */
/*  Procedures exported by the semantics module                 */
/*                                                              */
/****************************************************************/

/****************************************************************/
/*                                                              */
/*  semanticsInitialize - called once by the parser at the      */
/*                        start of  compilation                 */
/*                                                              */
/****************************************************************/

void semanticsInitialize( ) {

   /*********************************************/
   /*   Initialize the symbol table             */
   /*********************************************/
   symbolInitialize( );

   /*********************************************/
   /*  Additional initialization code for the   */
   /*  semantic analysis module                 */
   /*  GOES HERE                                */
   /*********************************************/
}

/****************************************************************/
/*                                                              */
/*  semanticsFinalize - called by the parser once at the        */
/*                      end of compilation                      */
/*                                                              */
/****************************************************************/

void semanticsFinalize( ){

  /**********************************************/
  /*  Finalize the symbol table                 */
  /**********************************************/
  symbolFinalize( );
  
   /*********************************************/
  /*  Additional finalization code for the      */
  /*  semantics analysis module                 */
  /*  GOES here.                                */
  /**********************************************/
  
}

/****************************************************************/
/*  start the semantic checker                                  */
/****************************************************************/
void semanticsCheck( ASTtype tree )
{
    ObjectP trace = ( ObjectP )tree; 
    commFlags flags;

    initFlags( &flags );

    assert ( trace != NULL );
    assert ( trace->otype == Oscope );

    /* S00 : enter scope */
    symbolEnterScope( );

    checkScope( trace->ovalue.scope, &flags );

    /* S01 : exit scope */
    symbolExitScope( );
}

/****************************************************************/
/*  Check declarations                                          */
/****************************************************************/
void checkDecl( DeclP decl ) {
    SymbTabEntryInfo symEntryInfo; 
    DataType datatype;
    int dbound;             /* array bound */
    commFlags flags; 

    initFlags( &flags );

    assert( decl != NULL );

    /* initialize symbol table entry */
    symEntryInfo.dctype = DCnone; 

    /* check that symbol has not already been defined */
    if ( ( symbolLookup( decl->dname, &symEntryInfo ) == TRUE )
            && ( symEntryInfo.lexic_level == symbolLexicLevel( )
            && ( symEntryInfo.dctype != DCforward_func ) 
            && ( symEntryInfo.dctype != DCforward_proc )
       ) ) {
        /* error: symbol already defined at this level */ 
        error_msg( EalreadyDeclared, decl->line, "blah" ) ; /*decl->dname );*/
    }
    
     
    switch( decl->dctype ) {
    
        case DCforward_func:
            SLOG( "found function forward declaration" );
            if ( symEntryInfo.dctype == DCforward_proc ) {
                    /* TODO */
                    error_msg( EalreadyDeclared, decl->line, decl->dname );
            }  else  if ( symEntryInfo.dctype == DCforward_func ) {
                /* don't add multiple forward decls to the symbol table */ 
                /* check that the signatures match */
                checkSignature( symEntryInfo.dfparm, decl->dfparm, decl->line );
            } else { 
                symbolAddDecl( decl );
            } 
            /* CODEGEN: set memory offset */
            break;

        case DCforward_proc:
            SLOG( "found procedure forward declaration" );
            if ( symEntryInfo.dctype == DCforward_func ) {
                    /* TODO */ 
                    error_msg( EalreadyDeclared, decl->line, decl->dname );
            }  else  if ( symEntryInfo.dctype == DCforward_proc ) {
                /* don't add multiple forward decls to the symbol table */ 
                /* check that the signatures match */
                checkSignature( symEntryInfo.dfparm, decl->dfparm, decl->line );
            } else { 
                symbolAddDecl( decl );
            } 
            /* CODEGEN: set memory offset */
            break;

        case DCscalar: 
            SLOG( "processing scalar declaration" );
            /* S10 : declare scalar */
            symbolAddDecl( decl );
            /* check initexpn */
            if ( decl->dinitexpn != NULL )
            {
                datatype = checkExpn( decl->dinitexpn );
		if (datatype != decl->dtype)
                {
                    if (decl->dtype == Dbool)
                        error_msg( EtypeMismatch, decl->dinitexpn->line, "assigning integer to boolean" ); 
                    else
                    {
                        assert(decl->dtype == Dint);
                        error_msg( EtypeMismatch, decl->dinitexpn->line, "assigning boolean to integer" ); 
                    }
                }
            }
            break;

        /* array: enter in symtab, check if declared     */
        case DCarray: 
            SLOG( "processing array declaration" );
            dbound = decl->dbound; 

            /* check size */
            if ( ( dbound > MAX_ARR ) && ( dbound < MIN_ARR ) ) {
                /* error: array too large or too small */
                error_msg( EarrayOutOfBound, decl->line, decl->dname );
            } 

            /* S11 : declare array */
            symbolAddDecl( decl ); 
            break;

        case DCfunc: 
        {
            SLOG( "processing function declaration" );
            /* if existing forward declaration is for a procedure */
            if ( symEntryInfo.dctype == DCforward_proc ) {
                    /* TODO */ 
                    error_msg( EalreadyDeclared, decl->line, decl->dname );
            }  else  if ( symEntryInfo.dctype == DCforward_func ) {
                /* a forward declaration exists */
                /* check that signatures match */
                checkSignature( symEntryInfo.dfparm, decl->dfparm, decl->line );
                /* update symbol table dctype to DCforward_func */
                symbolSetType( decl->dname, decl->dctype );
            } else {   
                /* otherwise add an entry to the symbol table */
                /* S12, S13 : declare function */
                symbolAddDecl( decl );
            }

            /* S04 : enter function scope */
            symbolEnterScope( );
		
            /* S18 : declare parameters */
            checkFParam( decl->dfparm );
            flags.insideFunc = TRUE;
            flags.returnType = decl->dtype; 
            checkScope( decl->dobject->ovalue.scope, &flags );

            if( flags.foundResult == FALSE ) {
                error_msg( EmissingResult, decl->line, NULL );
            }
            
            /* s05 : end function scope */
            symbolExitScope( );
            } break;

        case DCproc:
        {
            SLOG("processing procedure declaration");
            /* if existing forward declaration is for a procedure */
            if ( symEntryInfo.dctype == DCforward_func) {
                    /* TODO */ 
                    error_msg( EalreadyDeclared, decl->line, decl->dname );
            }  else  if ( symEntryInfo.dctype == DCforward_proc ) {
                /* a forward declaration exists */
                /* check that signatures match */
                checkSignature( symEntryInfo.dfparm, decl->dfparm, decl->line );
                /* update symbol table dctype to DCforward_func */
                symbolSetType( decl->dname, decl->dctype );
            } else {   
                /* otherwise add an entry to the symbol table */
                /* S12, S13 : declare function */
                symbolAddDecl( decl );
            }

            /* S06 : start procedure scope */
            symbolEnterScope( );
           
            /* S18 : declare parameters */
            checkFParam( decl->dfparm );
            flags.insideProc = TRUE;
            checkScope( decl->dobject->ovalue.scope, &flags ); 

            /* S07 : end procedure scope */
            symbolExitScope( );
            } break;

        default:
            assert( FALSE );
    }
}

/****************************************************************/
/*  Check formal parameters                                     */
/****************************************************************/
void checkFParam ( FparmP params ) {
    SLOG("processing paramters");
    while ( params != NULL ) {
            symbolAddFparm( params );
            params = params->fnext;
    }

    
}

/****************************************************************/
/*  Check a list of statements                                  */
/****************************************************************/
void checkStmt_list( ObjectP stmts, commFlagsP flags ) 
{
    while ( stmts != NULL ) {
        checkStmt( stmts->ovalue.stmt, flags );
        stmts = stmts->onext;
    }
} 

/****************************************************************/
/*  Check statement                                             */
/****************************************************************/
void checkStmt( StmtP stmt, commFlagsP flags ) 
{
    DataType type = Dnone;
    DataType type1 = Dnone;
    FparmP parms;
    SymbTabEntryInfo symEntryInfo; 
    BOOLEAN alreadyInside = FALSE;
    ArgsP args;
    int i;
    
    assert( stmt != NULL );
    
    switch( stmt->stype ){

        case Sassign:
            type = checkExpn( stmt->sexpn1 );
            type1 = checkExpn( stmt->sexpn2 );

            /* S34 : variables of same type */
            if ( type == Dint && type1 != Dint ) {
                error_msg( EtypeMismatch, stmt->sexpn1->line, "assigning boolean to integer" ); 
            } else if ( type == Dbool && type1 != Dbool ) {
                error_msg( EtypeMismatch, stmt->sexpn1->line, "assigning integer to boolean" ); 
            }

            /* assigning value to a function call 
             * Note: all other assignments should be legal or unparseable
             */
            if ( stmt->sexpn1->etype == Efcall ) {
                error_msg( EillegalAssignment, stmt->sexpn1->line, "function"
                        );
            }         
            
            break;

        case Sif:
            type1 = checkExpn( stmt->sexpn1 );

            /* S30 : control expression is boolean */
            if ( type1 != Dbool ) {
                /* error, if expn not boolean */
                error_msg( EinvalidExpnType, stmt->sexpn1->line, "boolean" );
            }

            checkStmt_list( stmt->sstmt1, flags );
            break;

        case Sifelse:
            type1 = checkExpn( stmt->sexpn1 );

            /* S30 : control expression is boolean */
            if ( type1 != Dbool ) {
                /* error, if expn not boolean */
                error_msg( EinvalidExpnType, stmt->line, "boolean" );
            }
            checkStmt_list( stmt->sstmt1, flags );
            checkStmt_list( stmt->sstmt2, flags );
            break;

	case Sfor: 
            if (checkExpn( stmt->sexpn1 ) != Dint)
                error_msg( EinvalidExpnType, stmt->line, "integer" );
            if (checkExpn( stmt->sexpn2 ) != Dint)
                error_msg( EinvalidExpnType, stmt->line, "integer" );
            if (checkExpn( stmt->sexpn3 ) != Dint)
                error_msg( EinvalidExpnType, stmt->line, "integer" );
             
            alreadyInside = flags->insideLoop;
            flags->insideLoop = TRUE;
            checkStmt_list( stmt->sstmt1, flags );
            flags->insideLoop = alreadyInside;

	    break;            

        case Swhile: /* fall through */
        case Sdo:
            type1 = checkExpn( stmt->sexpn1 );

            /* S30 : control expression is boolean */
            if ( type1 != Dbool ) {
                /* error, while param not boolean */
                error_msg( EinvalidExpnType, stmt->line, "boolean" );
            }
            alreadyInside = flags->insideLoop;
            flags->insideLoop = TRUE;
            checkStmt_list( stmt->sstmt1, flags );
            flags->insideLoop = alreadyInside;

            break;

        case Sexit:
            /* S50 : exit inside loop */
            if (!flags->insideLoop) 
                error_msg(EinvalidExit, stmt->line, NULL);
            break;

        case Sreturn:
            /* S51 : return inside proc */
            if (!flags->insideProc) 
                error_msg(EinvalidReturn, stmt->line, NULL);
            break;

        case Sresult:
            /* S52 : result inside func */
            if (!flags->insideFunc) {
                error_msg(EinvalidResult, stmt->line, NULL);
            }
            type = checkExpn( stmt->sexpn1 );

            /* S35 : expression type matches enclosing function */
            if ( flags->returnType != type ) {
                error_msg(EtypeMismatch, stmt->line, "result expression");
            }
            flags->foundResult = TRUE;
            break;

        case Sput:
            checkPutArgs( stmt->sargs );
            break;

        case Sget:
            checkGetArgs( stmt->sargs );
            break;

        case Sassert:
            if (checkExpn( stmt->sexpn1 ) != Dbool)
                error_msg( EinvalidExpnType, stmt->line, "boolean" );
            break;

        case Scall:
            if ( symbolLookup( stmt->pname, &symEntryInfo ) != TRUE ) {
                /* error: function not visible */
                error_msg( EundeclaredProc, stmt->line, stmt->pname );
            }

            /* check that symbol is a procedure */
            if (   symEntryInfo.dctype != DCproc 
                && symEntryInfo.dctype != DCforward_proc) {
                error_msg( EundeclaredProc, stmt->line, stmt->pname );
                return;
            }

            args = stmt->sargs;

            /* S42,S43 : check number arguments match number parameters */
            if ( symEntryInfo.dbound != numArgs( args ) ) {
                /* error: number of the arguement not match */
                error_msg( EparamCount, stmt->line, "procedure" );
            }

            parms = symEntryInfo.dfparm;

            /* S36 : check argument type matches parameter type */
            for(i = 1; i <= symEntryInfo.dbound; i++ ) {
                type1 = checkExpn(args->avalue);
                if( parms->ftype != type1) {
                    /* error: arguement type mismatch */
                    error_msg( EtypeMismatch, stmt->line, "procedure argument" );
                }
                parms = parms->fnext;
            }
            break;

        case Sscope:
            /* S08 : start ordinary scope */
            symbolEnterScope( );
            checkScope( stmt->sscope, flags );
            /* S09 : end ordinary scope */
            symbolExitScope( );
            break;

        default:
            assert( FALSE );

    }

}


/****************************************************************/
/*  Check expression                                            */
/*      returns DataType of expression                          */
/****************************************************************/
DataType checkExpn( ExpnP expn ) {
    DataType result1 = Dnone;
    DataType result2 = Dnone;
    DataType result = Dnone;
    FparmP parm;
    int i;
    ArgsP args;
    
    int lvalue; 
    SymbTabEntryInfo symEntryInfo; 
    
    assert( expn != NULL );
    
    switch( expn->etype ){

        case Econst:
            result = expn->evalue.elitVal.ltype;
            lvalue = expn->evalue.elitVal.lvalue.num;
            if ( result == Dint 
                    && lvalue < -MAX_INTEGER 
                    && lvalue > MAX_INTEGER ) {
                /* error: literal integer out of range */
                error_msg( EintegerOutOfRange, expn->line, NULL );
            }
            return result;
            break;

        case Evar: /* S24 */
            if ( symbolLookup( expn->evalue.eident, &symEntryInfo ) != TRUE ) {
                /* error: variable not visible */
                error_msg( EundeclaredVariable, expn->line, expn->evalue.eident );
            }

            if (   symEntryInfo.dctype == DCfunc 
                || symEntryInfo.dctype == DCforward_func) {
                /* set the etype properly */
                expn->etype = Efcall; 

                /* S42, : check that function has no paramters */
                if ( symEntryInfo.dbound != 0 ) {
                    error_msg( EparamCount, expn->line, "function" );
                }
            } else if ( symEntryInfo.dctype == DCarray ) {
                error_msg( EexpectedSubscript, expn->line, NULL );
            }

            return symEntryInfo.dtype;
            break;

        case Eunary:
            /* uminus or negation */
            if ( expn->evalue.eop.eoper == Onot ) {
                result = checkExpn( expn->evalue.eop.eopright );
                /* S30 : check expr is boolean */
                if ( result != Dbool ) {
                    error_msg( EinvalidExpnType, expn->line, "boolean" );
                } else {
                    return Dbool;
                }
            } else if ( expn->evalue.eop.eoper == Ouminus ) {
                result = checkExpn( expn->evalue.eop.eopright );
                /* S31 : check expr is int */
                if ( result != Dint ) {
                    error_msg( EinvalidExpnType, expn->line, "integer" );
                } else {
                    return Dint;
                }
            }
            break;

        case Ebinary:
            switch ( expn->evalue.eop.eoper ) {
                case Oadd: /* fall through */
                case Osub: /* fall through */
                case Omul: /* fall through */
                case Odiv:
                    result1 = checkExpn( expn->evalue.eop.eopleft );                
                    result2 = checkExpn( expn->evalue.eop.eopright );                
                    /* S31 : check expr are type int */
                    if ( result1 != Dint || result2 != Dint ) { 
                        error_msg( EtypeMismatch, expn->line, "integer expression" );
                    } 
                    return Dint;
                    break;
                case Oand: /* fall through */
                case Oor:
                    result1 = checkExpn( expn->evalue.eop.eopleft );                
                    result2 = checkExpn( expn->evalue.eop.eopright );                
                    /* S30 : check exprs are boolean */
                    if ( result1 != Dbool || result2 != Dbool ) { 
                        error_msg( EtypeMismatch, expn->line, "boolean expression" );
                    } 
                    return Dbool;
                    break;
                case Oless: /* fall through */
                case Olesseq: /* fall through */
                case Ogrtr: /* fall through */
                case Ogrtreq:
                    result1 = checkExpn( expn->evalue.eop.eopleft );                
                    result2 = checkExpn( expn->evalue.eop.eopright );                
                    /* S31 : check exprs are int */
                    if ( result1 != Dint || result2 != Dint ) { 
                        error_msg( EtypeMismatch, expn->line, "integer comparison expression" );
                    } 
                    return Dbool;
                    break;
                case Oneq: /* fall through */
                case Oeq: 
                    result1 = checkExpn( expn->evalue.eop.eopleft );                
                    result2 = checkExpn( expn->evalue.eop.eopright );                
                    if ( result1 != result2 ) { 
                        /* error: type mismatch */
                        error_msg( EtypeMismatch, expn->line, "test for equality expression" );
                    } 
                    return Dbool;
                    break;
                case Osubs: 
                    SLOG( "Found array expression" );
                    if ( symbolLookup( expn->evalue.eop.eopleft->evalue.eident, &symEntryInfo ) != TRUE ) {
                        error_msg( EundeclaredVariable, expn->line, expn->evalue.eop.eopleft->evalue.eident );
                    }

                    /* S38: variable is declared array */
                    result = symEntryInfo.dtype; 
                    if ( symEntryInfo.dctype != DCarray ) {
                        error_msg( EillegalSubscripting, expn->line, NULL );
                    }

                    /* S31 : check expression is type int */
                    result1 = checkExpn( expn->evalue.eop.eopright );
                    if ( result1 != Dint ) {
                        error_msg( EtypeMismatch, expn->line, "array subscript expression.  Must be an integer" );
                    }

                    /* return the type of this array */
                    return result;
                    break;
                default: /* unary ops such as Onop, Onot and Ouminus should never end up here */
                    assert( FALSE );
            }
            break;

            /* S36 : check argument type matches parameter type */
        case Econd:
            result  = checkExpn( expn->evalue.econx.control );                
            result1 = checkExpn( expn->evalue.econx.contrue );                
            result2 = checkExpn( expn->evalue.econx.confalse );                
            if ( result != Dbool ) {
                /* error: expected boolean control */
                error_msg( EinvalidControl, expn->line, NULL );
            } else if ( result1 != result2 ) { 
                /* error: type mismatch */
                error_msg( EtypeMismatch, expn->line, "conditional expression" );
            } 
            return result1;
            break;

        case Efcall: 
            if ( symbolLookup( expn->evalue.efn.efname, &symEntryInfo ) != TRUE ) {
                error_msg( EundeclaredFunc, expn->line, expn->evalue.efn.efname );
            }

            result = symEntryInfo.dtype;
            
            /* S42,S43 : check number arguments match number parameters */
            if ( symEntryInfo.dbound != numArgs( expn->evalue.efn.efargs ) ) {
                error_msg( EparamCount, expn->line, "function" );
                return result;
            }
            args = expn->evalue.efn.efargs;
            parm = symEntryInfo.dfparm;

            /* S36 : check argument type matches parameter type */
            for(i = 1; i <= symEntryInfo.dbound; i++ ) {
                result1 = checkExpn(args->avalue); /* type of parameter */
                result2 = parm->ftype; /* type of argument */

                if( result2 != result1) {
                    error_msg( EtypeMismatch, expn->line, "function argument" );
                }
                parm = parm->fnext;
            }

            return result;
            break;

        case Eskip: /* shouldn't process Skip as an expression */
        default:
            assert( FALSE );
    }
    
    /* All expressions should be handled above.  Nothing should fall through
     * to here */
    assert( FALSE );
    return Dnone; /* Can't happen */
}



/****************************************************************/
/*  Check scope                                                 */
/****************************************************************/
void checkScope( ScopeP scope, commFlagsP flags ){
    ObjectP trace; 
    int bSeenStmt = 0;  /* have we seen a statement in this scope? */

    assert( scope != NULL ); /* error: scope expected */

    trace = scope->scbody; 

    /* check through the elements in the body */
    while ( trace != NULL ) { 
        switch( trace->otype ) {
            case Odecl: /* S02 */
                if ( bSeenStmt == 1 ) {
                    /* error: declaration after statements in scope */
                    error_msg( EdeclAfterStatement, scope->line, NULL );
                } else { 
                    checkDecl( trace->ovalue.decl );
                }
                break;

            case Ostmt: /* S03 */
                bSeenStmt = 1;
                checkStmt( trace->ovalue.stmt, flags );
                break;
            default:
                /* error: expect only statements or declarations in a scope */
                assert( FALSE );
                break;
        }

        trace = trace->onext;
    } 

    if ( bSeenStmt == 0 ) {
        /* error: no statements in scope */
        error_msg( EemptyScope, scope->line, NULL );
    }
}

/****************************************************************/
/*  Check literal                                               */
/*      returns DataType of expression                          */
/****************************************************************/
DataType checkLiteral( LitObj lit ){
    return lit.ltype;
}

/****************************************************************/
/*  Check arguments for a put statement                         */
/****************************************************************/
void checkPutArgs( ArgsP args ){
    ArgsP trace = args;
    DataType result;

    SLOG("processing put arguments"); 
    while( trace != NULL ){
        if ( trace->avalue->etype != Eskip ) {
            result = checkExpn( trace->avalue );
            /* S31 : expr or var is int or text */
            if ( result != Dtext && result != Dint ) {
                /* invalid put args */
                error_msg( EinvalidPutArgs, args->line, NULL );
            }

        }

        trace = trace->anext;
    }
    
}

/****************************************************************/
/*  Check arguments for a get statement                         */
/****************************************************************/
void checkGetArgs( ArgsP args ){
    ArgsP trace = args;
    DataType result;

    SLOG("processing get arguments"); 
    while( trace != NULL ){
        result = checkExpn( trace->avalue );
        /* S31 : expr is int */
        if ( result != Dint ) {
            /* Einvalid get args */
            error_msg( EinvalidGetArgs, args->line, NULL );
        }

        trace = trace->anext;
    }
}

/********************************************************/
/*  check routine signature                             */
/********************************************************/
void checkSignature( FparmP params1 , FparmP params2, int line ) 
{
    while ( params1 != NULL && params2 != NULL ) { 
        if ( params1 ->ftype != params2 ->ftype ) {
            error_msg( EtypeMismatch, line, "routine parameter" );
            return;
        }
        params1     = params1->fnext;
        params2     = params2->fnext;
    }
   
    if ( params1 != NULL || params2 != NULL ) {
        error_msg( EparamCount, line, "routine" );
    } 
}


/*

   S20
   S21
   S24
   S25
   S26 - we handle this by return types.. blah

   S02
   S03 - we do this when building the AST

   S45 - we process all arguments at once
   S19 - we don't keep track of parameter count explicitly 


*/
