#ifndef _MACHINE_H_
/****************************************************************/
/*								*/
/*	machine.h						*/
/*	Interface to the CSC488S machine interpreter		*/
/*								*/
/****************************************************************/
#define _MACHINE_H_

static const char machineInterfaceVersion[] = 
   "$Id: machine.h,v 1.1 2003/02/27 14:09:56 g1u Exp $" ;

/********************************/
/* Initialize the machine	*/
/********************************/

void  machineInitialize();

/********************************/
/*  Execute compiled program	*/
/********************************/

void  machineExecute() ;

#endif

