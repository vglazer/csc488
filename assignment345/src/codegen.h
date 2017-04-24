#ifndef _codegen_H_
#define _codegen_H_
/****************************************************************/
/*								*/
/*	codegen.h						*/
/*      Interface to the					*/
/*	codegen Analysis Module for CSC488S Project Compiler	*/
/*								*/
/****************************************************************/

static const char * codegen_h_RCSId = "$RCSfile: codegen.h,v $ $Revision: 1.8 $ $Date: 2003/03/27 01:19:14 $" ;


/****************************************************************/
/*  Include common definitions and global variables             */
/****************************************************************/
#include "common.h"
#include "globalvars.h"
#include "astDef.h"


/********************************************************/
/*  codegenInitialize 					*/
/*	called by the parser before Pass 2		*/
/********************************************************/

void codegenInitialize() ;

/********************************************************/
/*  codegenFinalize 					*/
/*	 called by the parser after Pass 2		*/
/********************************************************/

void codegenFinalize() ;

/********************************************************/
/*   codegenBegin                                       */
/*      start code generation pass                      */
/********************************************************/
void codegenBegin( ASTtype tree );

#endif
