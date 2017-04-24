/************************************************************************/
/*									*/
/*	main.c								*/
/*	This is the main driver program for the CSC488S course project	*/
/*	compiler.  It handles compiler command line options and		*/
/*	initialization, calls the parser to run the compilation		*/
/*	and then invokes the pseudo machine interpreter to execute	*/
/*	the compiled program.						*/
/*									*/
/* 	This is the version of the main driver program for		*/
/*	Assignments 3 ..6						*/
/*									*/
/************************************************************************/


/************************************************************************/
/* The compiler has the following parts:		         	*/
/*	common definitions      	common.h                 	*/
/*      global variable declarations    globalvars.h             	*/
/*      abstract syntax tree module     ast.h, ast.c             	*/ 
/*      scanner module                  scanner.c                	*/
/*	symbol table module		symbol.h, symbol.c	 	*/
/*	parser module			parser.tab.h, parser.c	 	*/
/*	semantics analysis module	semantics.c	         	*/
/*	code generator module		codegen.c		 	*/
/*	machine interpreter module	machineDef.h, machine.h  	*/
/*					machine.c 			*/
/************************************************************************/

/************************************************************************/
/*	Definitions common to all modules in the compiler		*/
/************************************************************************/

#include "common.h"

/************************************************************************/
/*	Interfaces required for main.c					*/
/************************************************************************/

#include  <getopt.h>

/************************************************************************/
/*	Global variables (globalvars.h)                			*/
/* 	This is the home for these variables				*/
/************************************************************************/

#define EXTERN_GLOBALS
#include "globalvars.h"
 
/************************************************************************/
/* Include interface definitions for external functions called by main	*/
/************************************************************************/

   /*************************************************************/
   /*  Include interface to abstract syntax tree		*/
   /*************************************************************/
   
#include "ast.h"

   /*************************************************************/
   /*  Include interface to semantics analysis			*/
   /*************************************************************/
   
#include "semantics.h"

   /*************************************************************/
   /*  Include interface to code generation			*/
   /*************************************************************/
   
#include "codegen.h"

   /*************************************************************/
   /*  Include interface to machine interpreter			*/
   /*************************************************************/
   
#include "machine.h"


/************************************************************************/
/*   Interface to the scanner and parser				*/
/************************************************************************/

extern FILE*   yyin;    /* Flex input file	  */
extern void yyparse() ;	/* Bison generated parser */


/************************************************************************/
/* Default values for various files					*/
/* NOTE assumption that default files are not closed at 		*/
/* the end of compilation						*/
/************************************************************************/

#define DEFAULT_INPUT_FILE  	stdin 
#define DEFAULT_OUTPUT_FILE 	stdout
#define DEFAULT_ERROR_FILE  	stderr
#define DEFAULT_DUMP_FILE   	stdout
#define DEFAULT_TRACE_FILE  	stdout
#define DEFAULT_RUN_INPUT_FILE  stdin

 
/************************************************************************/
/* Internal utility procedure for opening optional files 		*/
/************************************************************************/

static FILE * fileOpen( char * fileName, char * fileMode, 
                                      FILE * defaultFile ){
    /****************************************************/
    /*  open file "fileName" with mode "mode"		*/
    /*  print error message and return default file	*/
    /*  if open fails					*/
    /****************************************************/
    FILE * fTemp ;
    if( (fTemp = fopen( fileName, fileMode )) != NULL) 
         /* successfully opened the file	*/
         return fTemp ;
    else {
    	fprintf(errorFile, "Unable to open file %s\n", fileName );
    	return defaultFile ;
    };   
}

/*********************************************************/
/*							 */  
/* Main program for the Compiler			 */
/*							 */
/*********************************************************/

int main( int argc , char *argv[] ){
    
    /****************************************************/
    /*  Initialize global variables to default values	*/
    /*  before processing arguments and flags		*/
    /****************************************************/
    
    char optch ;	/* sub option character 	*/
    int  ch    ;	/* option character 		*/
        
    /*  Initialize files				*/

    inputFile  	= DEFAULT_INPUT_FILE ;
    outputFile 	= DEFAULT_OUTPUT_FILE ;
    errorFile  	= DEFAULT_ERROR_FILE ;
    dumpFile   	= DEFAULT_DUMP_FILE ;
    traceFile  	= DEFAULT_TRACE_FILE ;
    runInputFile = DEFAULT_RUN_INPUT_FILE ;

    /*  Initialize control flags			*/

    errorOccurred    = FALSE ;
    traceSource	     = FALSE ;
    traceAST         = FALSE ;
    dumpAST1         = FALSE ;
    dumpAST2	     = FALSE ;
    yydebug          = FALSE ;
    traceSemantics   = FALSE ;
    traceSymbols     = FALSE ;
    dumpSymbols      = FALSE ;
    traceCodeGen     = FALSE ;
    traceExecution   = FALSE ;
    dumpInstructions = FALSE ;
    suppressExecution = FALSE ;
    
    
    /********************************************/
    /*	Process compiler option flags 		*/
    /********************************************/

    opterr = 0 ;	/* suppress getopt error messages */
    while( (ch = getopt( argc, argv, "XD:T:E:O:R:I:U:")) != EOF){
	/****************************************/
    	/* process  -ch  option			*/
    	/****************************************/

    	switch( ch ){
    	
    	case 'D' :  /* Dump options  -Dabxy	*/
    		    while(( optch = *optarg++ ) ){
    		        /* process to end of option letters */
    		        switch( optch ) {
    		        case 'a' :  dumpAST1 = TRUE ;
    		    		    break ;
    		        case 'b' :  dumpAST2 = TRUE ;
    		    		    break ;
    		        case 'x':   dumpInstructions = TRUE ;
    		    		    break ;
    		        case 'y':   dumpSymbols = TRUE ;
    		    		    break ;
    		        default:    fprintf(errorFile, 
    		    		   "Invalid dump option %c ignored\n", optch );
    		    		   break ;
    		    	} ;
    		    };
    		    break ;
    		    
    	case 'T' :  /* Trace options  -Tacilpsxy 	*/
    		    while(( optch = *optarg++ )){
    		    	/* process to end of option letters */
    		        switch( optch ) {
			case 'a' :  traceAST = TRUE;	    
    		        	    break ;
    		        case 'c' :  traceCodeGen = TRUE ;
    		    		    break ;
    		        case 'i' :  traceSource = TRUE ;
    		        	    break ;
			case 'l' :  traceTokens = TRUE;	    
    		        	    break ;
    		        case 'p' :  traceParse = TRUE;
    		    		    break ;
    		    	case 's' :  traceSemantics = TRUE ;
    		    		    break ;
    		        case 'x':   traceExecution = TRUE ;
    		    		    break ;
    		        case 'y':   traceSymbols = TRUE ;
    		    		    break ;
    		        default:    fprintf(errorFile, 
    		    		   "Invalid trace option %c ignored\n", optch );
    		    		   break ;
    		    	} ;
    		    };
    		    break ;
    		    
    	case 'O' :  /* Alternative output file */
    		    outputFile = fileOpen( optarg, "w", DEFAULT_OUTPUT_FILE );
    		    break ;
    		    
    	case 'E'  : /* Alternative error message file */
		    /* USE freopen to catch fprintf(stderr,... in yyparse */
		    if((errorFile = freopen( optarg, "w", DEFAULT_ERROR_FILE )) == NULL ){
		        fprintf(DEFAULT_ERROR_FILE, 
			    "Unable to open error file %s\n", optarg );
			errorFile = DEFAULT_ERROR_FILE ;
		    };
    		    break ;
    		    
   	case 'R' :  /* Alternative sink for  traces */
   		    traceFile = fileOpen( optarg, "w", DEFAULT_TRACE_FILE );
   		    break ;
   		    
        case 'U' :  /* Alternative sink for dumps	*/
        	    dumpFile = fileOpen( optarg, "w", DEFAULT_DUMP_FILE );
        	    break ;
        	    
        case 'I' :  /* Alternative input during execution	*/
        	    runInputFile = fileOpen( optarg, "r", DEFAULT_RUN_INPUT_FILE);
        	    break ;
        	    
        case 'X' :  /* supress execution flag		*/
        	    suppressExecution = TRUE ;
        	    break ;
        	    
        case '?' :	/* getopt error flag	*/
   	default:	/* Anything else	*/
   		    fprintf(stderr,"Unknown option character %c (ignored)\n",
   		    	optch );
   		    break ;

        } ;	/* end switch( ch )	*/
      };
     /* end of -ch argument processing  */

    /************************************************************/
    /*  Check for user supplied source file			*/
    /************************************************************/

    if( optind < argc ) {
        /*  user supplied source file name on input line */
        if((inputFile = fopen( argv[optind] , "r")) != NULL ){
	    yyin = inputFile;
            fprintf(outputFile, 
	    	"Compiling from source file %s\n", argv[optind] );
	}else{
	    fprintf(errorFile,"Unable to open source file %s\n",
	    	argv[optind] );
	    exit(1)  ;
	}
     }; 	/* else compile from stdin */

/********************************************************/
/*  Compilation Starts Here				*/
/********************************************************/
     	    
    fprintf(outputFile,"Compilation begins\n");


   /*********************************************/
   /*   Initialize the abstract syntax tree	*/
   /*********************************************/
   astInitialize( & parseTree );
   astDump( parseTree );
   
   /*********************************************/
   /*  Additional initialization code for 	*/
   /*  Pass 1 GOES HERE				*/
   /*********************************************/
   line = 1;

   /*********************************************/
   /*  Pass 1 Scan and parse the program	*/
  /**********************************************/

    yyparse();
  
  /**********************************************/
  /*  Any finalization code for			*/
  /*  Pass 1 GOES HERE				*/
  /**********************************************/

  if( dumpAST1 )
	astDump( parseTree  ) ;

  if( errorOccurred ) {
  	fprintf( errorFile, 
	   "\nPass two supressed do to errors\n");
	exit( 2 );
  }
  
  /**********************************************/
  /*   Initialize semantics analysis		*/
  /**********************************************/
   semanticsInitialize();
   
  /**********************************************/
  /*   Initialize code generation		*/
  /**********************************************/
   codegenInitialize();
   
  /**********************************************/
  /*  Additional initialization code for 	*/
  /*  Pass 2 GOES HERE				*/
  /**********************************************/

    /********************************************/
    /*  Code to invoke semantic analysis        */
    /********************************************/   
    semanticsCheck( parseTree );

    /********************************************/
    /*  Code to invoke code generation          */
    /********************************************/   
    codegenBegin( parseTree );

  /**********************************************/
  /*   Finalize semantics analysis		*/
  /**********************************************/
   semanticsFinalize();
   
  /**********************************************/
  /*   Finalize code generation			*/
  /**********************************************/
  codegenFinalize();

  /**********************************************/
  /*  Additional finalization code for		*/
  /*  Pass 2 GOES HERE				*/
  /**********************************************/  
   
  if( dumpAST2 ) 
  	astDump( parseTree ) ;
	
  /**********************************************/
  /*   Finalize abstract syntax tree		*/
  /**********************************************/
  astFinalize( parseTree );

  fprintf(outputFile, "\nCompilation Ends\n" );

  /******************************************************/
  /*  Conditional Execution of Compiled Program		*/
  /******************************************************/
     
  if( errorOccurred  ){
	fprintf(outputFile, 
          "Execution suppressed due to compilation errors\n");
	exit(3) ;
  } else
    	/***************************************************/
    	/*  Execute the program in machine memory	   */
    	/*  machineExecute handles suppressExecution flag  */
    	/*  so it can dump the compiled code if requested. */
    	/***************************************************/
    	
	machineExecute();
   
    /********************************************/ 
    /*  Clean up files if necessary		*/
    /********************************************/

    if( inputFile != DEFAULT_INPUT_FILE )
    	fclose( inputFile );
    if( errorFile != DEFAULT_ERROR_FILE )
     	fclose( errorFile );
    if( dumpFile != DEFAULT_DUMP_FILE )
    	fclose( dumpFile );
    if( traceFile != DEFAULT_TRACE_FILE )
    	fclose( traceFile );
    if( outputFile != DEFAULT_OUTPUT_FILE )
    	fclose( outputFile );
    if( runInputFile != DEFAULT_RUN_INPUT_FILE )
    	fclose( runInputFile );
  
    exit(0);	/* end of successful compilation */
}




