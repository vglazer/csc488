
%{

/********************************************************/
/*  Common Definitions                                  */
/********************************************************/

#include "common.h"
#include <ctype.h>

/********************************************************/
/*                                                      */
/*  Global variables.                                   */
/*                                                      */
/********************************************************/

#include "globalvars.h"

/********************************************************/
/*  Include token definitions from the parser           */
/********************************************************/

#include "parser.tab.h"

#define yyinput input

/********************************************************/
/* Macros to implement tracing of lexical analysis      */
/********************************************************/

#define TS_TEXT do {if (traceSource) fprintf (traceFile, "%s", yytext);} while(0)
#define TS_CHAR( ech ) do {if (traceSource) fprintf (traceFile, "%c", ech); } while(0)
#define TT_TOKEN( tok ) \
    do { \
        if( traceTokens ) { \
            if( isprint( tok ) ) { \
                fprintf(traceFile,"token %d ( %c )\n", tok, tok  ); \
            } else { \
                fprintf(traceFile,"token %d\n", tok ); \
            } \
        } \
    } while(0)


/********************************************************/
/*  Constants and utility variables                     */
/********************************************************/

#define NL  '\n'

int intValue ;  /* holds value of integer constants         */
int textLeng ;  /* hold length of string constant (text)    */
int identLeng ; /* holds length of identifier               */
%}

/********************************************************/
/*  Definitions for long tokens                         */
/*  Identifiers, integer constants, text                */
/********************************************************/

Digit       [0-9]
Id      [A-Za-z][A-Za-z0-9_]*
Qstring     \"[^\"\n]*[\"\n]

%%


\%  {   
    /********************************************************/
    /*  Comment handling                                    */
    /********************************************************/

        /* comment to end of line or EOF */
        int inCh  = yyinput();
        while( inCh != NL && inCh != EOF ) {
            TS_CHAR( inCh );
            inCh = yyinput();
        };
        if( inCh == EOF )   /* program ended with comment */
            yyterminate();
        else {
            TS_CHAR( inCh );
            line++ ;
            if( traceSource || traceTokens )
                fprintf( traceFile, "\nLINE %d\n", line );
        }
       }
                 
{Digit}+ {  
    /********************************************************/
    /*  Integer constant handling                           */
    /********************************************************/

        TS_TEXT;    /*  integer constant  */     
            intValue = atoi(yytext);
            if( intValue > MAX_INTEGER ) {
                fprintf( errorFile,
        "\nLexical Error (line %d) - integer constant (%d) too large\n",
                line, yylval.token.num );
                intValue = MAX_INTEGER ;
            errorOccurred = TRUE;
            }
            yylval.token.num = intValue ;
            if( traceTokens )
                fprintf( traceFile, "token %d ( %d )\n", INTCONST, intValue );
            return(INTCONST);
          }
                 
{Qstring} {
    /********************************************************/
    /*  String constant (text) handling                     */
    /********************************************************/

    TS_TEXT;    /* quoted string - text */
    if (yytext[yyleng-1] == NL) {
        fprintf(errorFile, 
             "\nLexical Error (line %d) - character string not terminated\n", 
                  line);
        line++; 
            if (traceSource)
                   fprintf(traceFile,"%d\t",line);
    }
    if( yyleng > MAX_TEXT ) {
        fprintf( errorFile ,
      "Lexical Error (line %d) - character string too long (truncated)\n",
          line );
        textLeng = MAX_TEXT ;
            errorOccurred = TRUE;
    }else
        textLeng = yyleng ;  
    yylval.token.str = (char *) malloc(textLeng + 1);
    if( ! yylval.token.str ) {
       fprintf(errorFile ,
       "Lexical Error (line %d) - unable to allocate memory for string of length %d\n",
        line, textLeng );
             errorOccurred = TRUE;
     } else {    
            strncpy(yylval.token.str, yytext, textLeng);
            yylval.token.str[textLeng] = '\0' ; /* force termination */
         }
         if( traceTokens )
            fprintf( traceFile, "token %d ( %s )\n", 
        TEXTCONST, yylval.token.str );
     return(TEXTCONST);
    }

boolean { TS_TEXT;  TT_TOKEN( BOOL );   return BOOL ;   }
do  { TS_TEXT;  TT_TOKEN( DO );     return DO ; }
else    { TS_TEXT;  TT_TOKEN( ELSE );   return ELSE ;   }
exit    { TS_TEXT;  TT_TOKEN( EXIT );   return EXIT ;   }
false   { TS_TEXT;  TT_TOKEN( TFALSE ); return TFALSE ; }
fi  { TS_TEXT;  TT_TOKEN( FI );     return FI ; }
get { TS_TEXT;  TT_TOKEN( GET );    return GET ;    }
if  { TS_TEXT;  TT_TOKEN( IF );     return IF ; } 
int { TS_TEXT;  TT_TOKEN( INT );    return INT ;    }
od  { TS_TEXT;  TT_TOKEN( OD );     return OD ; }
proc    { TS_TEXT;  TT_TOKEN( PROC );   return PROC ;   }
put { TS_TEXT;  TT_TOKEN( PUT );    return PUT ;    }
skip    { TS_TEXT;  TT_TOKEN( SKIP );   return SKIP ;   }
result  { TS_TEXT;  TT_TOKEN( RESULT ); return RESULT ; }
return  { TS_TEXT;  TT_TOKEN( RETURN ); return RETURN ; }
then    { TS_TEXT;  TT_TOKEN( THEN );   return THEN ;   }
true    { TS_TEXT;  TT_TOKEN( TTRUE );  return TTRUE ;  }
until   { TS_TEXT;  TT_TOKEN( UNTIL );  return UNTIL ;  }
while   { TS_TEXT;  TT_TOKEN( WHILE );  return WHILE ;  }
for     { TS_TEXT;  TT_TOKEN( FOR );    return FOR ;    }
to  { TS_TEXT;  TT_TOKEN( TO );     return TO ;     }
by  { TS_TEXT;  TT_TOKEN( BY );     return BY ; }
rof { TS_TEXT;  TT_TOKEN( ROF );    return ROF ;    }
forward { TS_TEXT;  TT_TOKEN( FORWARD );    return FORWARD; }
assert  { TS_TEXT;  TT_TOKEN( ASSERT ); return ASSERT;  }


[+\-*/()\[\]\{\},<=>:\?!\&\|#]  { 
    /*********************************************************/
    /*  single character special character handling          */
    /*********************************************************/

    TS_CHAR( yytext[0]); 
        TT_TOKEN( (int)yytext[0] ); 
    return yytext[0] ;
    }

{Id}    { 
    /********************************************************/
    /*  Identfier handling                                  */
    /********************************************************/

      TS_TEXT;  /* Identifers - not reserved words */
      if( yyleng > MAX_IDENTIFIER ) {
        fprintf( errorFile ,
        "Lexical Error (line %d) - identifier too long (truncated)\n",
          line );
        identLeng = MAX_IDENTIFIER ;
            errorOccurred = TRUE;
       } else
        identLeng = yyleng ;     
      yylval.token.str = (char *) malloc(identLeng + 1);
      if( ! yylval.token.str ) {
          fprintf(errorFile ,
          "Lexical Error (line %d) - unable to allocate memory for identifier of length %d\n",
            line, identLeng );
                errorOccurred = TRUE;
        } else {    
                strncpy(yylval.token.str, yytext, identLeng);
                yylval.token.str[identLeng] = '\0' ;    /* force termination */
            }
      if( traceTokens )
        fprintf( traceFile, "token %d ( %s )\n",
            IDENT, yylval.token.str );
      return(IDENT);
    }
    
\n  {
    /********************************************************/
    /*  End of line (newline) handling                      */
    /********************************************************/

     int c;     /* newLine character - end of line */
     TS_CHAR( NL );
     c = yyinput();
     if (c != EOF) {
            unput(c); 
                line++; 
                if (traceSource || traceTokens )
            fprintf( traceFile, "\nLINE %d\n", line );
          } else 
            yyterminate();
    }
    
<<EOF>> {   
    /********************************************************/
    /*  End of file handling                                */
    /********************************************************/

        yyterminate() ;
    }
    
[ \t]   { 
    /********************************************************/
    /*  Whitespace (blank and tab) handling                 */
    /********************************************************/

      TS_CHAR( yytext[0] );}    /* blanks and tabs   */


[@$^;'] { 
    /********************************************************/
    /*  Illegal characters handling                         */
    /*  Characters not allowed outside of strings           */
    /********************************************************/

      TS_CHAR( yytext[0] );
      fprintf(errorFile, "\nLexical Error (line %d) - %c is an illegal character\n",
             line, yytext[0]);
          errorOccurred = TRUE;
    }

%%
