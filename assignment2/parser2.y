%{
/****************************************************************************/
/*  parser.y                                                                */
/*  This is the parser for the 488S course project.                         */
/*                                                                          */
/*  Group 5: Jacob Tseng, Ryan Maclean, Jon Pipitone and Victor Glazer      */
/****************************************************************************/



/*****************************************************************************/
/*  Definitions common to all modules in the compiler                        */
/*****************************************************************************/

#include "common.h"

/*****************************************************************************/
/*  Global variables                                                         */
/*****************************************************************************/

#include "globalvars.h"

extern int yylex();             /* lexical analysis */

/*****************************************************************************/
/*  Functions introduced in this file                                        */
/*****************************************************************************/

void yyerror(char* message);   /* how to report errors */


%}
%union {int number;
	tokentype token;
        }

/*****************************************************************************/
/*  Symbolic names for lexical tokens                                        */
/*****************************************************************************/

%token  IDENT	INTCONST	TEXTCONST
%token	BOOL	DO	ELSE	EXIT	FI
%token	GET	IF	INT	OD
%token	PROC	PUT	SKIP 	RESULT
%token	RETURN 	THEN	TFALSE	TTRUE	
%token	UNTIL	WHILE

/*****************************************************************************/ 
/*  GTE, LTE and UMINUS denote the >=, <= and unary - operators              */
/*  respectively; ! and unary minus are declared to be                       */
/*  right-associative, more or less arbitrarily.                             */
/*****************************************************************************/ 

%left '|' 
%left '&'
%right '!'
%nonassoc '=' '#' '<' '>' GTE LTE 
%left '+' '-'
%left '*' '/'
%right UMINUS

%start program

%%

/*****************************************************************************/
/*  Project language grammar                                                 */
/*****************************************************************************/

/* Start symbol and scope */
program:  scope ;

scope:  '{' declaration_list statement_list '}'  
	|	'{' statement_list  '}'
	;


/* Declarations */
declaration_list: declaration_list declaration
    |   declaration
    ;

declaration:  type IDENT  
	|	type IDENT '['  INTCONST  ']' 
	|	type IDENT ':'  scope 
	|	type IDENT '('  parameters  ')'  ':' scope 
	|	PROC IDENT ':' scope  
	|	PROC IDENT '(' parameters ')' ':' scope 
	;


/* Statements */
statement_list:  statement_list statement
    | statement
    ;

statement:  variable  '='  expression 
	|	IF expression  THEN statement_list FI 
	|	IF expression  THEN statement_list  ELSE statement_list  FI 
	|	WHILE expression DO statement_list OD 
	|	DO statement_list UNTIL expression 
	|	EXIT 
	|	RETURN 
	|	RESULT expression 
	|	PUT output_list
	|	GET input 
	|	IDENT  
	|	IDENT  '('  arguments  ')' 
	|	scope  
	; 


/* Input and output */
output_list: output_list ',' output
    |   output
    ;

output:  expression 
	|	TEXTCONST 
	|	SKIP 
	;

input:  input ',' variable
    |   variable
    ;


/* Types, variables, arguments and parameters */
type:  INT 
	|   BOOL 
	;

variable:  IDENT 
	|	IDENT  '['  expression  ']' 
	;

arguments:  arguments ',' expression 
	|	expression
	;

parameters:  parameters  ',' type IDENT  
    |   type IDENT
	;


/* Expressions. Precedence and associativity rules are automatically enforced
   by the %left, %right and %nonassoc declarations. The %prec modifier is used
   to differentiate between the precedence of the binary and unary minus 
   operators. Note that 'variable' will match any identifier in this context,
   including function and parameter names */
expression:  INTCONST 
	|	'-'  expression %prec UMINUS 
	|	expression  '+'  expression 
	|	expression  '-'  expression 
	|	expression  '*'  expression 
	|	expression  '/'  expression 
	|	TTRUE 
	|	TFALSE 
	|	'!'  expression
	|	expression '&'  expression 
	|	expression '|'  expression 
	|	expression '=' expression 
	|	expression '#' expression 
	|	expression '<'  expression 
	|	expression '<' '=' expression %prec LTE 
	|	expression '>'  expression 
	|	expression '>' '='  expression %prec GTE
	|	'('  expression  ')'  
	|	'('  expression  '?' expression ':' expression  ')'  
	|	variable   
	|	IDENT  '('  arguments  ')'   
	;

%%

/*****************************************************************************/
/*  Functions to print error messages                                        */
/*****************************************************************************/

void yyerror(char* message) {
        fprintf(errorFile,"Parse error (line %d) - %s\n", line , message );
}
