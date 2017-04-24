#ifndef _MACHINE_DEF_H_
/****************************************************************/
/*								*/
/*  Definition of csc488s Pseudo Machine			*/
/*								*/
/*  Interface between the Code Generator and Machine Interpreter*/
/*								*/
/****************************************************************/
#define _MACHINE_DEF_H_

static const char machineDefVersion[] = 
  "$Id: machineDef.h,v 1.1 2003/02/27 14:09:56 g1u Exp $" ;

#include "common.h"

/****************************************************************/
/* Machine Components						*/
/* 	normally these would be contained in the machine module	*/
/*	but this compiler cheats by compiling directly to memory*/
/*	so the code generator needs access to memory.		*/
/*								*/
/* These machine components are visible to the code generator	*/
/* but their real home is in the machine module			*/
/*								*/
/****************************************************************/

	
/********************************************************/
/*  Size of machine memory in words			*/
/********************************************************/
	
#define  MEMORYSIZE  (8192)

/************************************************/
/*  type of machine memory word			*/
/************************************************/
	
typedef  short	MEMORYWORD ;

/************************************************/
/*  Limit on size of integer value		*/
/*  Target machine has 16 bit signed integers 	*/
/*  NOTE that integers are symmetric about  0	*/
/*  AND the least 16-bit integer is used as	*/
/*  a special value for UNDEFINED		*/
/************************************************/
   
#define  MIN_INTEGER  ( - MAX_INTEGER )

/************************************************/
/* most negative 16-bit integer value ( -32768) */
/* is used to denote undefined			*/
/************************************************/

#define  UNDEFINED  ( MIN_INTEGER - 1 )


/************************************************/
/* DISPLAY SIZE determines maximum depth of 	*/
/* static function or procedure nesting 	*/
/************************************************/
	
#define  DISPLAYSIZE  (20)


/************************************************************************/
/*									*/
/*  To guarantee a unique initialization for the arrays that define	*/
/*  machine constants, the following mechanism is used.			*/
/*  In machine.c							*/
/*	EXTERN_MACHINE is defined outside of this file			*/
/*	this causes MACHINE_CONSTS to be defined which in turn		*/
/*      causes initial values to be given to various arrays		*/
/*  In other files (specifically codegen.c)				*/
/*      EXTERN_MACHINE is not defined outside of this file		*/
/*      This causes EXTERN_MACHINE to become extern and			*/
/*	causes initial values for various arrays to be omitted		*/
/*									*/
/************************************************************************/

#define MACHINE_CONSTS
#ifndef EXTERN_MACHINE
#define EXTERN_MACHINE  extern
#undef  MACHINE_CONSTS 
#endif

/****************************************************************/
/*								*/
/*  Machine main memory	   addresses 0 .. MEMORYSIZE -1		*/
/*								*/
/****************************************************************/
	
EXTERN_MACHINE MEMORYWORD memory[ MEMORYSIZE ] ;


/****************************************************************/
/*								*/
/* Machine Instructions						*/
/*								*/
/****************************************************************/

enum InstructionCodes {
	HALT = 0, ADDR,  LOAD, STORE, PUSH, PUSHMT, SETD, POP,    
	POPN,     DUP,   DUPN, BR,    BF,   NEG,    ADD,  SUB,
	MUL,	  DIV,   EQ,   LT,    OR,   SWAP,   READC,PRINTC, 
	READI,    PRINTI,TRON, TROFF 
	}  ; 

/************************************************/
/*  Range of valid instructions			*/
/************************************************/

#define firstInstruction  HALT
#define lastInstruction   TROFF

/************************************************/
/*  Instruction names for dump and trace 	*/
/************************************************/

EXTERN_MACHINE const char *instructionNames[]
#ifdef MACHINE_CONSTS
    = {	"HALT", "ADDR",  "LOAD", "STORE", "PUSH", "PUSHMT", "SETD", "POP",
	"POPN",  "DUP",  "DUPN", "BR",    "BF",   "NEG",    "ADD",  "SUB",
	"MUL",  "DIV",   "EQ",   "LT",    "OR",   "SWAP",   "READC","PRINTC", 
	"READI","PRINTI","TRON", "TROFF" 
      } 
#endif
;

/********************************************************/
/*  Definitions of TRUE and FALSE for the machine	*/
/********************************************************/

#define MACHINE_TRUE	1
#define MACHINE_FALSE	0

/********************************************************/
/*							*/
/*  Table of lengths for each instruction.		*/
/*  NOTE:  length of branch instructions is set to ZERO */
/*	   since they directly change the pc		*/
/*  NOTE:  length of HALT instruction is zero since once*/
/*         we reach halt, updating the pc is meaningless*/
/*							*/
/********************************************************/

EXTERN_MACHINE const unsigned char instructionLength[] 
#ifdef MACHINE_CONSTS
  =   { 0, 3, 1, 1, 2, 1, 2, 1, 1, 1, 1,	/* HALT .. DUPN  */
	0, 0,					/* BR   .. BF   */
	1, 1, 1, 1, 1, 1, 1, 1, 1, 		/* NEG  .. SWAP */
	1, 1, 1, 1, 1, 1			/* READC.. TROFF*/
    }
#endif
      ;

#endif
