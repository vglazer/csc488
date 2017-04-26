/************************************************************************/
/*                                                                      */
/*  main2.c                                                             */
/*  This is the main driver program for  CSC488S Assignment 2           */
/*  It handles compiler command line options and                        */
/*  initialization, calls the parser end then exits.                    */
/************************************************************************/

/************************************************************************/
/*  Common definitions                                                  */
/************************************************************************/

#include "common.h"

#include <getopt.h>

/************************************************************************/
/*  Shared global variables for  main2.c                                */
/************************************************************************/
      
    /************************************************/
    /*  File used as a sink for error messages      */
    /************************************************/

FILE *  errorFile ;

    /************************************************/
    /*  File used as a sink for dumps               */
    /************************************************/

FILE *  dumpFile ;

    /************************************************/
    /*  File used as a sink for trace output        */
    /************************************************/

FILE * traceFile ;

/********************************************************/
/*                                                      */
/*  Scanner global variables.                           */
/*  Contain information produced by the scanner         */
/*  and used by other modules                           */
/*                                                      */
/********************************************************/

    /**************************************************/
    /*  Current line number in input.  Set by scanner */
    /**************************************************/

int line ;

    

/***********************************************************/
/*                                                         */
/*  Control flags, set by main.c, used to cause            */
/*  various optional compiler actions                      */
/*                                                         */
/***********************************************************/

    /***************************************************/
    /*  Flag used to indicate that an error occurred.  */
    /***************************************************/

BOOLEAN errorOccurred ;

    /************************************************/
    /*  Cause trace of source program as it is read */
    /************************************************/

BOOLEAN traceSource ;
    
    /************************************************/
    /*  Cause trace of scanner token stream         */
    /************************************************/

BOOLEAN traceTokens ;   

    /************************************************/
    /*  Cause trace of parser                       */
    /************************************************/

#define  traceParser yydebug 
extern int yydebug ;
extern int yyparse(void); /* parsing */

/************************************************************************/
/*   Interface to the parser                                            */
/************************************************************************/

extern FILE*   yyin; 

/************************************************************************/
/* Default values for various files                                     */
/* Note assumption that default files are not closed at                 */
/* the end of compilation                                               */
/************************************************************************/

#define DEFAULT_ERROR_FILE      stderr
#define DEFAULT_DUMP_FILE       stdout
#define DEFAULT_TRACE_FILE      stdout


 
/************************************************************************/
/* Internal utility procedure for opening optional files                */
/************************************************************************/

static FILE * fileOpen( char * fileName, char * fileMode, 
                                      FILE * defaultFile ){
    /****************************************************/
    /*  open file "fileName" with mode "mode"           */
    /*  print error message and return default file     */
    /*  if open fails                                   */
    /****************************************************/
    FILE * fTemp ;
    if( (fTemp = fopen( fileName, fileMode )) != NULL) 
         /* successfully opened the file */
         return fTemp ;
    else {
        fprintf(errorFile, "Unable to open file %s\n", fileName );
        return defaultFile ;
    }   
}

/*********************************************************/
/*                                                       */  
/* Main program for the Compiler                         */
/*                                                       */
/*********************************************************/

int main( int argc , char *argv[] ){
    
    /****************************************************/
    /*  Initialize global variables to default values   */
    /*  before processing arguments and flags           */
    /****************************************************/
    
    char optch ;    /* sub option character */
    int  ch    ;    /* option character */
        
    /*  Initialize files  */

    yyin    = stdin ;
    errorFile   = DEFAULT_ERROR_FILE ;
    dumpFile    = DEFAULT_DUMP_FILE ;
    traceFile   = DEFAULT_TRACE_FILE ;

    /*  Initialize control flags  */

    errorOccurred    = FALSE ;
    traceSource      = FALSE ;
    traceTokens      = FALSE ;
    traceParser      = FALSE ;
    
    
    /********************************************/
    /*  Process compiler option flags           */
    /********************************************/

    opterr = 0 ;    /* suppress getopt error messages */
    while( (ch = getopt( argc, argv, "T:E:R:")) != EOF){
        /****************************************/
        /* process  -ch  option                 */
        /****************************************/

        switch( ch ){
        
        case 'T' :  /* Trace options  -Tilp */
                while( (optch = *optarg++) ){
                    /* process to end of option letters */
                    switch( optch ) {
                    case 'i' :  traceSource = TRUE ;
                            break ;
                    case 'l' :  traceTokens = TRUE;     
                            break ;
                    case 'p' :  traceParser = TRUE;
                            break ;
                    default:    fprintf(errorFile, 
                           "Invalid trace option %c ignored\n", optch );
                           break ;
                    } ;
                };
                break ;
                
        case 'E'  : /* Alternative error message file */
                errorFile = fileOpen( optarg, "w", DEFAULT_ERROR_FILE );
                break ;
                
    case 'R' :  /* Alternative sink for traces */
            traceFile = fileOpen( optarg, "w", DEFAULT_TRACE_FILE );
            break ;
            
        case '?' :  /* getopt error flag */
    default:    /* Anything else */
            fprintf(stderr,"Unknown option character %c (ignored)\n",
                optch );
            break ;

        } ; /* end switch( ch ) */
      };
     /* end of -ch argument processing */

    /************************************************************/
    /*  Check for user supplied source file                     */
    /************************************************************/

    if( optind < argc ) {
        /* user supplied source file name on input line */
        yyin = fileOpen( argv[optind] , "r", stdin );
        fprintf(stdout, "Compiling from source file %s\n", argv[optind] );
     };

    /********************************************************/
    /*  Compilation Starts Here                             */
    /********************************************************/
            
    fprintf(stdout,"Scanning and Parsing  begins\n");



    /*********************************************/
    /*  Scan and parse the program              */
    /*********************************************/

    yyparse();
  

    fprintf(stdout, "\nScanning and Parsing  Ends\n" );

    /********************************************/ 
    /*  Clean up files if necessary             */
    /********************************************/

    if( errorFile != DEFAULT_ERROR_FILE )
        fclose( errorFile );
    if( dumpFile != DEFAULT_DUMP_FILE )
        fclose( dumpFile );
    if( traceFile != DEFAULT_TRACE_FILE )
        fclose( traceFile );
  
    exit(0);
}

