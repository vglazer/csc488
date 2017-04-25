#ifndef _GLOBAL_VARS_H_
#define _GLOBAL_VARS_H_
/********************************************************/
/*  globalvars.h                                        */
/*  CSC488S Project Compiler Shared Global Variables    */
/*                                                      */
/*  This file contains the definition of global         */
/*  variables that are used for communication among     */
/*  the various compiler modules                        */
/*                                                      */
/********************************************************/

    /************************************************/
    /*  These variables are extern except in main.c */
    /************************************************/
    
#ifndef  EXTERN_GLOBALS
#define  EXTERN_GLOBALS  extern
#endif

/********************************************************/
/*                                                      */
/*  FILE global variables.                              */
/*  Used to specify sinks for compiler output and       */
/*  sources for compiler input                          */
/*                                                      */
/********************************************************/

    /*************************************************/
    /*  File used as an input source for the compiler*/
    /*************************************************/

EXTERN_GLOBALS FILE * inputFile ;

    /************************************************/
    /*  File used as a sink for execution output    */
    /************************************************/

EXTERN_GLOBALS FILE * outputFile ;

    /************************************************/
    /*  File used as a sink for error messages      */
    /************************************************/

EXTERN_GLOBALS FILE *  errorFile ;

    /************************************************/
    /*  File used as a sink for table dumps         */
    /************************************************/

EXTERN_GLOBALS FILE *  dumpFile ;

    /************************************************/
    /*  File used as a sink for trace output        */
    /************************************************/

EXTERN_GLOBALS  FILE * traceFile ;

    /********************************************************/
    /*  File used as input source during program execution  */
    /********************************************************/

EXTERN_GLOBALS  FILE * runInputFile ;

/********************************************************/
/*                                                      */
/*  Scanner global variables.                           */
/*  Contain information produced by the scanner         */
/*  and used by other modules                           */
/*                                                      */
/********************************************************/

    /*************************************************/
    /*  Current line number in input.  Set by scanner*/
    /*************************************************/

EXTERN_GLOBALS int line ;


/********************************************************/
/*                                                      */
/*  Flag used to indicate that an error occurred.       */
/*  Used to suprress subsequent processing              */
/*                                                      */
/********************************************************/

EXTERN_GLOBALS BOOLEAN errorOccurred ;


/*********************************************************/
/*                                                       */
/*  Trace flags, set by main.c, used to cause            */
/*  various optional tracing of compiler actions         */
/*                                                       */
/*********************************************************/

    /************************************************/
    /*  Cause trace of source program as it is read */
    /************************************************/

EXTERN_GLOBALS BOOLEAN traceSource ;
    
    /************************************************/
    /*  Cause trace of scanner token stream         */
    /************************************************/

EXTERN_GLOBALS BOOLEAN traceTokens ;    

    /************************************************/
    /*  Cause trace of parser                       */
    /************************************************/

extern  int  yydebug ;
#define  traceParse yydebug


#endif
