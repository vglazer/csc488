#ifndef _COMMON_H_
/********************************************************/
/* common.h                                             */
/* common definitions that are used throughout the      */
/* compiler                                             */
/*                                                      */
/********************************************************/
#define _COMMON_H_

/************************************************/
/*  C libraries that are widely used            */
/************************************************/

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

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
/*  Limits on the source language.              */
/*  Enforced in scanner.c                       */
/************************************************/

/************************************************/
/*  Limit on length of identifier               */
/************************************************/

#define  MAX_IDENTIFIER  32

/************************************************/
/*  Limit on range of integer values            */
/* ASSUMES machine memory is 16-bit short int   */
/* Note that range is symmetric about zero so   */
/* -32768 can be used for UNDEFINED value       */
/************************************************/

#define  MAX_INTEGER   32767

/************************************************/
/*  Limit on length  of text (string)           */
/* 255 characters plus 1 for terminating null   */
/************************************************/

#define  MAX_TEXT   256

/************************************************/
/*  Type used to carry values on parse stack    */
/************************************************/

typedef union {
    char* str;   /* text values */
    int num;    /* integer constants */ 
} tokentype;

#endif
