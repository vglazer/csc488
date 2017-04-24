#ifndef _AST_H_
#define _AST_H_
/********************************************************/
/*                                                      */
/*  Interface to the Abstract Syntax Tree Module        */
/*                                                      */
/********************************************************/


   /******************************************************/
   /* Hidden type for Abstact Syntax Tree                */
   /******************************************************/

typedef  void *  ASTtype ;

/********************************************************/
/*                                                      */
/*  External functions exported by AST module           */
/*                                                      */
/********************************************************/

   /*****************************************************/
   /* Initialize Abstact Syntax Tree                    */
   /*****************************************************/

void astInitialize( ASTtype * newTree ) ;

   /*****************************************************/
   /* Dump Abstact Syntax Tree                          */
   /*****************************************************/

void astDump( const ASTtype dumpTree ) ;

   /*****************************************************/
   /* Finalize Abstact Syntax Tree                      */
   /*****************************************************/

void astFinalize( ASTtype * deadTree ) ;

   /*****************************************************/
   /* More AST external functions GO HERE               */
   /*****************************************************/

/********************************************************/
/*                                                      */
/*      Definitions used to name things in the          */
/*      Abstract Syntax Tree                            */
/*                                                      */
/********************************************************/

/********************************************************/
/* types of objects                                     */
/********************************************************/

typedef enum ObjType_tag {
  Onone ,       /* None, unknown                        */
  Odecl ,       /* declaration                          */
  Oexpn ,       /* expression                           */
  Oscope ,      /* scope                                */
  Ostmt         /* statement                            */
} ObjType;

/********************************************************/
/* types of data                                        */
/********************************************************/

typedef enum DataType_tag {
  Dnone ,       /* unknown                              */
  Dbool,        /* boolean                              */
  Dint ,        /* integer                              */
  Dtext ,       /* text( string )                       */
  Dvoid         /* void ( procedure )                   */
} DataType;

/********************************************************/
/* types of declarations                                */
/********************************************************/

typedef enum DclType_tag {
  DCnone ,       /* unknown                             */
  DCscalar ,     /* scalar variable                     */
  DCarray ,      /* array                               */
  DCfunc ,       /* function                            */
  DCproc,        /* procedure                           */
  DCforward_func,/* forward function                    */
  DCforward_proc /* forward procedure                   */
} DclType;

/********************************************************/
/* types of expressions                                 */
/********************************************************/

typedef enum ExpnType_tag {
  Enone ,       /* None, unknown                        */
  Econst ,      /* constant                             */
  Evar ,        /* variable                             */
  Eunary ,      /* unary operator                       */
  Ebinary,      /* binary operator                      */
  Econd ,       /* conditional expression               */
  Efcall,       /* function call                        */
  Eskip         /* skip                                 */
} ExpnType;

/********************************************************/
/* statement types                                      */
/********************************************************/

typedef enum StmtType_tag {
  Snone ,       /* none, unknown                        */
  Sassign ,     /* assignment                           */
  Sif ,         /* if without else                      */
  Sifelse ,     /* if with else                         */
  Swhile ,      /* while                                */
  Sdo,          /* do until                             */
  Sexit ,       /* exit                                 */
  Sreturn ,     /* return                               */
  Sresult ,     /* result                               */
  Sput ,        /* put                                  */
  Sget ,        /* get                                  */
  Sassert , 	/* assert                               */
  Scall ,       /* procedure call                       */
  Sscope,       /* scope                                */
  Sfor		    /* for					                */
} StmtType;

/********************************************************/
/* types of operators                                   */
/********************************************************/

typedef enum OperType_tag {
  Onop ,        /* None, unknown                        */
  Oand ,        /* Boolean and                          */
  Oor ,         /* Boolean or                           */
  Onot ,        /* Boolean not                          */
  Oeq ,         /* Equality                             */
  Oneq ,        /* Inequality                           */
  Oless ,       /* Less                                 */
  Olesseq ,     /* Less or equal                        */
  Ogrtr ,       /* Greater                              */
  Ogrtreq ,     /* Greater or equal                     */
  Ouminus ,     /* Unary minus                          */
  Oadd,         /* Addition                             */
  Osub,         /* Subtraction                          */
  Omul,         /* Multiplication                       */
  Odiv,         /* Divide                               */
  Osubs         /* subscript                            */
} OperType;

#include "astDef.h"

/****************************************************************************/
/*                                                                          */
/****************************************************************************/
ObjectP mkProgram( ScopeP scope );

/****************************************************************************/
/*  build a scope                                                           */
/****************************************************************************/
ScopeP mkScope( ObjectP decls, ObjectP stmts, int linenum );

/****************************************************************************/
/*  build a list of statements                                              */ 
/*      stmt_list - NULL if only a single statement in the list             */ 
/*                  otherwise stmt becomes the head of the list             */
/****************************************************************************/
ObjectP mkStmt_list( ObjectP stmt_list, StmtP stmt );

/****************************************************************************/
/*  build a list of declarations                                            */
/*      decl_list - NULL if only a single declaration in the list           */
/*                  otherwise decl becomes the head of the list             */
/****************************************************************************/
ObjectP mkDecl_list( ObjectP decl_list, DeclP decl );

/****************************************************************************/
/*  build an assignment statement                                           */
/****************************************************************************/
StmtP mkStmt_assign( ExpnP var, ExpnP expr, int linenum );

/****************************************************************************/
/*  build a while statement                                                 */
/****************************************************************************/
StmtP mkStmt_while( ExpnP expr, ObjectP stmts, int linenum );

/****************************************************************************/
/*  build a do statement                                                    */
/****************************************************************************/
StmtP mkStmt_do( ObjectP stmts, ExpnP expr, int linenum );

/****************************************************************************/
/*  build a for statement                                                   */
/****************************************************************************/
StmtP mkStmt_for( ExpnP var, ExpnP initexpr, ExpnP loopexpr, int incramt, 
                  ObjectP stmts, int linenum );

/****************************************************************************/
/*  build an exit statement                                                 */
/****************************************************************************/
StmtP mkStmt_exit( int linenum );

/****************************************************************************/
/*  build a return statement                                                */
/****************************************************************************/
StmtP mkStmt_return( int linenum );

/****************************************************************************/
/*  build a result statement                                                */
/****************************************************************************/
StmtP mkStmt_result( ExpnP expr, int linenum );

/****************************************************************************/
/*  build a put statement                                                   */
/****************************************************************************/
StmtP mkStmt_put( ArgsP args, int linenum );

/****************************************************************************/
/*  build a get statement                                                   */
/****************************************************************************/
StmtP mkStmt_get( ArgsP args, int linenum );

/****************************************************************************/
/*  build an assert statement                                               */
/****************************************************************************/
StmtP mkStmt_assert( ExpnP expn, int linenum );

/****************************************************************************/
/*  build a minor scope statement                                           */
/****************************************************************************/
StmtP mkStmt_scope( ScopeP scope, int linenum );

/****************************************************************************/
/*  build a proccedure call statement                                       */
/*      args - NULL if no arguments                                         */
/****************************************************************************/
StmtP mkStmt_proc( tokentype ident, ArgsP args, int linenum );

/****************************************************************************/
/*  build an if statement                                                   */
/*      else_stmts - NULL if this is not an 'if-then-else' statement        */
/****************************************************************************/
StmtP mkStmt_if( ExpnP expr, ObjectP then_stmts, ObjectP else_stmts, 
        int linenum );

/****************************************************************************/
/*  build an incomplete declaration                                         */
/****************************************************************************/
DeclP mkDecl_var_type( DataType dataType, int linenum );

/****************************************************************************/
/*  fill-in declaration indentifier name                                    */
/****************************************************************************/
DeclP mkDecl_var_ident( DeclP decl, tokentype ident );

/****************************************************************************/
/*  make declaration a scalar declaration                                   */
/*      expects declaration to have identifier and type                     */ 
/****************************************************************************/
DeclP mkDecl_scalar( DeclP decl, ExpnP expninit );

/****************************************************************************/
/*  make declaration an array declaration                                   */
/*      expects declaration to have identifier and type                     */ 
/****************************************************************************/
DeclP mkDecl_arr( DeclP decl, int size );

/****************************************************************************/
/*  build an incomplete declaration of an function/procedure body           */
/****************************************************************************/
DeclP mkDecl_body( FparmP params, ScopeP scope, int linenum );

/****************************************************************************/
/*  make declaration a procedure declaration                                */
/****************************************************************************/
DeclP mkDecl_proc( tokentype ident, DeclP body );

/****************************************************************************/
/*  make declaration a function declaration                                 */
/****************************************************************************/
DeclP mkDecl_func( DeclP decl, DeclP body ); 

/****************************************************************************/
/*  build a list of formal paramters from declarations                      */ 
/*      parms - NULL if only a single parameter in the list                 */ 
/*              otherwise the parameter for decl becomes head of the list   */
/****************************************************************************/
FparmP mkFparm_list( FparmP parms, DeclP decl ); 

/****************************************************************************/
/*  build a list of arguments                                               */ 
/*      args - NULL if only a single argument in the list                   */ 
/*             otherwise arg becomes head of the list                       */
/****************************************************************************/
ArgsP mkArg_list_args( ArgsP args, ArgsP arg );

/****************************************************************************/
/*  build a list of arguments from an expression                            */ 
/*      args - NULL if only a single argument in the list                   */ 
/*             otherwise the argument for expr becomes the head of the list */
/****************************************************************************/
ArgsP mkArg_list_expn( ArgsP args, ExpnP expr );

/****************************************************************************/
/*  build an argument from an expression                                    */ 
/****************************************************************************/
ArgsP mkArg_expn( ExpnP expr );

/****************************************************************************/
/*  build an argument from a string                                         */ 
/*      NOTE: this is used only for arguments in a put statement            */
/****************************************************************************/
ArgsP mkArg_str( tokentype text, int linenum );

/****************************************************************************/
/*  build a skip argument                                                   */ 
/****************************************************************************/
ArgsP mkArg_skip( int linenum );

/****************************************************************************/
/*  build a binary expression                                               */ 
/****************************************************************************/
ExpnP mkExpn_oper( ExpnP left, OperType operType, ExpnP right, int linenum );

/****************************************************************************/
/*  build a conditional expression                                          */ 
/****************************************************************************/
ExpnP mkExpn_cond( ExpnP cond, ExpnP mid, ExpnP right, int linenum );

/****************************************************************************/
/*  build an function call expression                                       */ 
/****************************************************************************/
ExpnP mkExpn_func( tokentype ident, ArgsP args, int linenum );

/****************************************************************************/
/*  build a constant expression                                             */ 
/****************************************************************************/
ExpnP mkExpn_const( LitObjP lit, int linenum );

/****************************************************************************/
/*  build a variable expression                                             */ 
/****************************************************************************/
ExpnP mkExpn_var( tokentype ident, int linenum );

/****************************************************************************/
/*  build an array expression                                               */ 
/****************************************************************************/
ExpnP mkExpn_var_arr( tokentype ident, ExpnP expr, int linenum );

/****************************************************************************/
/*  build an integer literal                                                */ 
/****************************************************************************/
LitObjP mkLit( DataType dataType, int constant );

/****************************************************************************/
/*  build an boolean literal                                                */ 
/****************************************************************************/
LitObjP mkLit_bool( DataType dataType, int boolValue );

#endif
