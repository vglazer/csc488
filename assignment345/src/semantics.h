#ifndef _SEMANTICS_H_
#define _SEMANTICS_H_

#include "astDef.h"
/****************************************************************/
/*								*/
/*	semantics.h						*/
/*      Interface to the					*/
/*	Semantics Analysis Module for CSC488S Project Compiler	*/
/*								*/
/****************************************************************/

static const char semanticsInterfaceVersion[] = 
	"$Id: semantics.h,v 1.2 2003/02/27 16:45:54 g1u Exp $" ;

/****************************************************************/
/*                                                              */ 
/*      Internal definitions                                    */
/*                                                              */ 
/****************************************************************/
#define MIN_ARR 0               /* minimum array index */ 
#define MAX_ARR MAX_INTEGER     /* maximum array index */

/********************************************************/
/*  semanticsInitialize 				*/
/*	called by main before semantic analysis		*/
/********************************************************/

void semanticsInitialize() ;

/********************************************************/
/*  semanticsFinalize 					*/
/*	 called by main after semantic analysis		*/
/********************************************************/

void semanticsFinalize() ;

/********************************************************/
/*   MORE FUNCTIONS for doing semantic analysis		*/
/*   GO HERE						*/
/********************************************************/

/****************************************************************/
/*  start the semantic checker                                  */
/****************************************************************/
void semanticsCheck(ASTtype tree);

#endif

