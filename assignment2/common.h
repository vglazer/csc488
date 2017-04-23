#ifndef _COMMON_H_
/********************************************************/
/* common.h							*/
/* common definitions that are used throughout the	*/
/* compiler						*/
/*							*/
/********************************************************/
#define _COMMON_H_

static char commonVersion[] =
	"$Id: common.h,v 1.1 2003/01/17 19:49:05 dw Exp $" ;

/************************************************/
/*  C libraries that are widely used		*/
/************************************************/

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

/*******************/
/*  Special types  */
/*******************/

#ifndef BOOLEAN
  #define BOOLEAN  unsigned char 

  #ifndef TRUE
  #define TRUE  1
  #endif

  #ifndef FALSE
  #define FALSE 0
  #endif

#endif

/************************************************/
/*  Limits on the source language.		*/
/*  Enforced in scanner.c			*/
/************************************************/

   /*********************************************/
   /*  Limit on size of identifier		*/
   /*********************************************/

#define  MAX_IDENTIFIER  32

   /*********************************************/
   /*  Limit on size of text (string)		*/
   /* 255 characters plus 1 for terminating null*/
   /*********************************************/

#define  MAX_TEXT   256

   /*********************************************/
   /*  Limit on value of integer constant	*/
   /*  16 bit integers				*/
   /*********************************************/

#define  MAX_INTEGER   32767

typedef union {char* str; int num;} tokentype;

#endif
