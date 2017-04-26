%{
/************************************************************************/
/*                                                                      */
/*      parser.y                                                        */
/*      This is the parser for the 488S course project.                 */
/************************************************************************/


/************************************************************************/
/*      Definitions common to all modules in the compiler               */
/************************************************************************/

#include "common.h"

/************************************************************************/
/*      Global variables                                                */
/************************************************************************/

#include "globalvars.h"

extern int yylex();             /* lexical analysis     */

/************************************************************************/
/*     Functions introduced in this file                                */
/************************************************************************/

void yyerror(char* message);   /* how to report errors */


%}
%union {
        int number;
        char *str;
        tokentype token;
        LitObjP litP;
        ScopeP scopeP;
        StmtP stmtP;
        FparmP fparmP;
        DeclP declP;
        ExpnP expnP;
        ArgsP argsP;
        ObjectP objectP; 
        }

%token <token> IDENT 	TEXTCONST
%token <number> INTCONST 
%token  BOOL    DO      ELSE    EXIT    FI      
%token  GET     IF      INT     OD      
%token  PROC    PUT     SKIP    RESULT  RETURN 
%token  THEN    TFALSE  TTRUE   UNTIL   WHILE 
%token  FOR	    TO	    BY	    ROF	  
%token  FORWARD ASSERT 

%start program

%type <number> byPart
%type <litP> constant
%type <scopeP> scope
%type <stmtP> statement procStatement ifstatement 
%type <fparmP>  parameters
%type <declP> declaration routineBody
%type <declP> typeAndIdent type 
%type <expnP> expression conjunction negation compareExpr arithExpr term
%type <expnP> factor primary functionCall variable 
%type <argsP> arguments outputs inputs input output
%type <objectP> program statements declarations optDeclarations

%%
program         : scope
                    { parseTree = mkProgram ($1); } 
                ;

statements      :  statement
                    { $$ = mkStmt_list(NULL, $1);}  
                |  statements statement
                    { $$ = mkStmt_list($1, $2);}
                ;
             
declarations    :  declaration 
                    { $$ = mkDecl_list(NULL, $1); }
                |  declarations declaration 
                    { $$ = mkDecl_list($1, $2); }
                ;
                
scope           :  '{' optDeclarations statements '}'
                    { $$ = mkScope ($2, $3, line); } 
                ;
                
optDeclarations :  declarations
                    { $$ = $1; }
                |  /* empty */
                    { $$ = NULL; } 
                ;
                        
statement       :  variable  '='  expression
                    { $$ = mkStmt_assign( $1, $3, line); }
                |  ifstatement
                    { $$ = $1; }  
                |  WHILE  expression DO statements OD
                    { $$ = mkStmt_while( $2, $4, line); }
                |  DO statements UNTIL expression
                    { $$ = mkStmt_do( $2, $4, line); }
                |  EXIT
                    { $$ = mkStmt_exit(line); }
                |  RETURN
                    { $$ = mkStmt_return(line); }
                |  RESULT  expression
                    { $$ = mkStmt_result($2, line); }
                |  PUT outputs
                    { $$ = mkStmt_put($2, line); }
                |  GET inputs
                    { $$ = mkStmt_get($2, line); }
                |  procStatement
                    { $$ = $1; }
                |  scope
                    { $$ = mkStmt_scope($1, line); }
                |  FOR variable ':''=' expression TO expression byPart statements ROF
                    { $$ = mkStmt_for( $2, $5, $7, $8, $9, line ); }
                |  ASSERT expression
                    { $$ = mkStmt_assert($2, line); }
                ;

byPart		    :  BY INTCONST 
                    { $$ = $2; }
                |  BY '-' INTCONST
                    { $$ = -$3; }
                |
                    { $$ = 1; }
                ;

procStatement   :  IDENT
                    { $$ = mkStmt_proc($1, NULL, line); }
                |  IDENT '(' arguments ')'
                    { $$ = mkStmt_proc($1, $3, line); }
                ;

ifstatement     :  IF expression  THEN statements ELSE statements FI
                    { $$ = mkStmt_if ($2, $4, $6, line); } 
                |  IF expression  THEN statements FI
                    { $$ = mkStmt_if ($2, $4, NULL, line); } 
                ;

declaration     :  typeAndIdent 
                    { $$ = mkDecl_scalar($1, NULL); }
                |  typeAndIdent '=' expression
                    { $$ = mkDecl_scalar($1, $3); }
                |  typeAndIdent '[' INTCONST ']' 
                    { $$ = mkDecl_arr($1, $3); }
                |  typeAndIdent routineBody 
                    { $$ = mkDecl_func ($1, $2); }
                |  PROC IDENT   routineBody 
                    { $$ = mkDecl_proc ($2, $3); }
                ;

routineBody     :  '(' parameters ')' ':' scope 
                    { $$ = mkDecl_body($2, $5, line); }
                |  '(' parameters ')' ':' FORWARD 
                    { $$ = mkDecl_body($2, NULL, line); }
                |  ':' FORWARD 
                    { $$ = mkDecl_body(NULL, NULL, line); }
                |  ':' scope
                    { $$ = mkDecl_body(NULL, $2, line); }
                ;

typeAndIdent    :  type IDENT 
                    { $$ = mkDecl_var_ident($1, $2); } 
                ;

parameters      :  typeAndIdent
                    { $$ = mkFparm_list(NULL, $1); } 
                |  parameters ',' typeAndIdent
                    { $$ = mkFparm_list($1, $3); } 
                ;

arguments       :  expression 
                    { $$ = mkArg_list_expn(NULL, $1); }
                |  arguments ',' expression
                    { $$ = mkArg_list_expn($1, $3); }
                ;

type            :  INT
                    { $$ = mkDecl_var_type(Dint, line); }
                |  BOOL
                    { $$ = mkDecl_var_type(Dbool, line); }
                ;

output          :  expression
                    { $$ = mkArg_expn($1); }
                |  TEXTCONST
                    { $$ = mkArg_str($1, line); }
                |  SKIP
                    { $$ = mkArg_skip(line); }
                ;

outputs         :  output    
                    { $$ = $1; }
                |       outputs ',' output                
                    { $$ = mkArg_list_args($1, $3); }
                ;
    
inputs          :  input                 
                    { $$ = $1; }
                |  inputs  ',' input
                    { $$ = mkArg_list_args($1, $3); }
                ;

input           :  variable
                    { $$ = mkArg_expn($1); }
                ;

expression      :  conjunction
                    { $$ = $1; }
                |  expression      '|'     conjunction              
                    { $$ = mkExpn_oper($1, Oor, $3, line); } 
                ;
                
conjunction     :  negation
                    { $$ = $1; }
                |  conjunction     '&'     negation
                    { $$ = mkExpn_oper($1, Oand, $3, line); } 
                ;
                
negation        :  compareExpr
                    { $$ = $1; }
                |  '!' negation 
                    { $$ = mkExpn_oper(NULL, Onot, $2, line); } 
                ;
                
compareExpr     :  arithExpr
                    { $$ = $1; }
                |  arithExpr       '='     arithExpr        
                    { $$ = mkExpn_oper($1, Oeq, $3, line); } 
                |  arithExpr       '#'     arithExpr        
                    { $$ = mkExpn_oper($1, Oneq, $3, line); } 
                |  arithExpr       '>'     arithExpr   
                    { $$ = mkExpn_oper($1, Ogrtr, $3, line); } 
                |  arithExpr       '>' '=' arithExpr        
                    { $$ = mkExpn_oper($1, Ogrtreq, $4, line); } 
                |  arithExpr       '<'     arithExpr        
                    { $$ = mkExpn_oper($1, Oless, $3, line); } 
                |  arithExpr       '<' '=' arithExpr   
                    { $$ = mkExpn_oper($1, Olesseq, $4, line); } 
                ;
                                 
arithExpr       :  term
                    { $$ = $1; }
                |  arithExpr       '+'     term            
                    { $$ = mkExpn_oper($1, Oadd, $3, line); } 
                |  arithExpr       '-'     term
                    { $$ = mkExpn_oper($1, Osub, $3, line); } 
                ;
                                
term            :  factor
                    { $$ = $1; }
                |  term    '*'     factor          
                    { $$ = mkExpn_oper($1, Omul, $3, line); } 
                |  term    '/'     factor
                    { $$ = mkExpn_oper($1, Odiv, $3, line); } 
                ;
                
factor          :  primary
                    { $$ = $1; }
                |  '-' factor      
                    { $$ = mkExpn_oper(NULL, Ouminus, $2, line); } 
                ;
                                
primary         :  '('     expression      ')'
                    { $$ = $2; } //???
                |  '('     expression '?'  expression ':' expression ')'
                    { $$ = mkExpn_cond($2, $4, $6, line); }
                |  variable
                    { $$ = $1; }
                |  functionCall
                    { $$ = $1; }
                |  constant
                    { $$ = mkExpn_const($1, line); }
                ;

functionCall    :  IDENT '(' arguments ')'   
                    { $$ = mkExpn_func($1, $3, line); }
                ;  /* IDENT without args indistinguishable from variable */

constant        :  INTCONST
                    { $$ = mkLit(Dint, $1); }
                |  TTRUE
                    { $$ = mkLit_bool(Dbool, 1); }
                |  TFALSE
                    { $$ = mkLit_bool(Dbool, 0); }
                ;

variable        :  IDENT           /* Could be function call */
                    { $$ = mkExpn_var($1, line); }
                |  IDENT '[' arithExpr ']'
                    { $$ = mkExpn_var_arr($1, $3, line); }
                ;


%%

/************************************************************************/
/*     Functions to print error messages                                */
/************************************************************************/

void yyerror(char* message) {
        fprintf(errorFile,"Parse error (line %d) - %s\n", line , message );
        errorOccurred = TRUE;
}
