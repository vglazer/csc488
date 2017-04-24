#ifndef _GLOBAL_VARS_H_
#define _GLOBAL_VARS_H_
/********************************************************/
/*  globalvars.h					*/
/*  CSC488S Project Compiler Shared Global Variables	*/
/*  							*/
/*  This file contains the definition of global		*/
/*  variables that are used for communication among 	*/
/*  the various compiler modules			*/
/*							*/
/********************************************************/

	/************************************************/
	/*  These variables are extern except in main.c	*/
	/************************************************/
	
#ifndef  EXTERN_GLOBALS
#define  EXTERN_GLOBALS  extern
#endif


/********************************************************/
/*							*/
/*	FILE global variables.				*/
/*	Used to specify sinks for compiler output and	*/
/*	sources for compiler input			*/
/*							*/
/********************************************************/

/********************************************************/
/*  File used as an input source for the compiler	*/
/********************************************************/

EXTERN_GLOBALS FILE * inputFile ;

/********************************************************/
/*  File used as a sink for execution output		*/
/********************************************************/

EXTERN_GLOBALS FILE * outputFile ;

/********************************************************/
/*  File used as a sink for error messages		*/
/********************************************************/

EXTERN_GLOBALS FILE *  errorFile ;

/********************************************************/
/*  File used as a sink for table dumps			*/
/********************************************************/

EXTERN_GLOBALS FILE *  dumpFile ;

/********************************************************/
/*  File used as a sink for trace output		*/
/********************************************************/

EXTERN_GLOBALS  FILE * traceFile ;

/********************************************************/
/*  File used as input source during program execution 	*/
/********************************************************/

EXTERN_GLOBALS  FILE * runInputFile ;

/********************************************************/
/*							*/
/*	Scanner global variables.			*/
/*	Contain information produced by the scanner 	*/
/*	and used by other modules			*/
/*							*/
/********************************************************/

/********************************************************/
/*  Current line number in input.  Set by scanner	*/
/********************************************************/

EXTERN_GLOBALS int line ;


/********************************************************/
/*							*/
/*	Code generator global variables			*/
/*	These global variables are used by the 		*/
/*	code generator module to communcate the		*/
/*	initial value of the machine state to the	*/
/*	pseudo machine interpreter.			*/
/*	See codegen.c for discussion of usage		*/
/*	conventions.					*/
/*							*/
/********************************************************/

/********************************************************/
/* initial value for program counter i.e. address	*/
/*  of first instruction in the	compiled program	*/
/********************************************************/

EXTERN_GLOBALS int startPC ;

/********************************************************/
/* initial value for memory stack pointer (msp)		*/
/********************************************************/

EXTERN_GLOBALS  int startMSP ;

/********************************************************/
/* initial value for memory limit pointer (mlp)		*/
/********************************************************/

EXTERN_GLOBALS  int startMLP ;
	

/********************************************************/
/*							*/
/*  Flag used to indicate that an error occurred.  	*/
/*  Used to suprress subsequent processing		*/
/*							*/
/********************************************************/

EXTERN_GLOBALS BOOLEAN errorOccurred ;


/********************************************************/
/* Flag used to suppress execution of compiled program	*/
/* Useful during code generation development to save	*/
/* time and effort.					*/
/********************************************************/

EXTERN_GLOBALS BOOLEAN  suppressExecution ;


/********************************************************/
/*							*/
/*	Trace flags, set by main.c, used to cause	*/
/*	optional tracing of compiler actions	   	*/
/*							*/
/********************************************************/

/********************************************************/
/*  Cause trace of source program as it is read		*/
/********************************************************/

EXTERN_GLOBALS BOOLEAN traceSource ;
	
/********************************************************/
/*  Cause trace of scanner token stream			*/
/********************************************************/

EXTERN_GLOBALS BOOLEAN traceTokens ;	

/********************************************************/
/*  Cause trace of parser				*/
/********************************************************/

extern  int  yydebug ;   /* use existing Bison variable */
#define  traceParse yydebug

/********************************************************/
/*  Cause trace of semantic analysis			*/
/********************************************************/

EXTERN_GLOBALS  BOOLEAN  traceSemantics ;

/********************************************************/
/*  Cause trace of symbol table module			*/
/********************************************************/

EXTERN_GLOBALS  BOOLEAN  traceSymbols ;

/********************************************************/
/*  Cause trace of abstract syntax tree module  	*/
/********************************************************/

EXTERN_GLOBALS  BOOLEAN  traceAST ;

/********************************************************/
/*  Cause trace of code generation			*/
/********************************************************/

EXTERN_GLOBALS  BOOLEAN  traceCodeGen ;		

/********************************************************/
/* Cause dump of instructions in memory before 		*/
/* execution						*/
/********************************************************/

EXTERN_GLOBALS BOOLEAN  dumpInstructions ;

/********************************************************/
/* Cause execution to be traced				*/
/********************************************************/

EXTERN_GLOBALS BOOLEAN  traceExecution ;

/********************************************************/
/*							*/
/*	Dump flags, set by main.c, used to cause	*/
/*	optional dumping  of various compiler 		*/
/*	data structures.				*/
/*							*/
/********************************************************/

/********************************************************/
/*  Cause dump of abstract syntax tree after  Pass 1	*/
/********************************************************/

EXTERN_GLOBALS  BOOLEAN  dumpAST1 ;

/********************************************************/
/*  Cause dump of abstract syntax tree after Pass 2	*/
/********************************************************/

EXTERN_GLOBALS  BOOLEAN  dumpAST2 ;

/********************************************************/
/*  Cause dump of symbol table information		*/
/********************************************************/

EXTERN_GLOBALS  BOOLEAN  dumpSymbols ;

/********************************************************/
/*							*/
/*	Global variable pointing at 			*/
/*	Abstract Syntyax tree				*/
/*	Used to communicate between passes		*/
/*							*/
/********************************************************/

#include "ast.h"

EXTERN_GLOBALS  ASTtype parseTree ;

#endif
