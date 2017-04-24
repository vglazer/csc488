###########################################################################
#	
#	Makefile
#
#       Makefile version:  $Id: Makefile,v 1.1 2003/01/17 19:49:05 dw Exp $
#
#	makefile for Assignment 2  - csc488s course project compiler
#
#	This file describes the dependencies between the
#	various compiler components.
###########################################################################


###########################################################################
#	
#	Define some useful constants
#
###########################################################################


#   Make command
MAKE  = make

#   Flex and Bison commands
FLEX = flex
FLEXFLAGS = -oscanner.c -Cae -I
BISON = bison
BISONFLAGS = --debug --file-prefix=parser --defines --verbose

###########################################################################
# 	Compiler and Compiler Flags
#	Compiler REQUIRES ANSI C compiler
###########################################################################

CC = gcc
#	-g needed for debuging
#	-Wall	for really thorough checking
CFLAGS = -g -Wall -ansi -pedantic
#	Linker flags 
LINKFLAGS = 
# on OS X the flex library is called l rather than fl. go figure.
PLATFORM = $(shell uname -s)
ifeq ($(PLATFORM),Darwin)
	FLEXLIB = l
else
	FLEXLIB = fl
endif
LINKFLAGS = -l$(FLEXLIB)

###########################################################################
#	
#   Name for the executable compiler
#	
###########################################################################

TARGET	=  compiler488
OFILES  =  main2.o scanner.o parser.o
HCOMMON =  globalvars.h

###########################################################################
#	
#	Dependencies for the compiler
#	
###########################################################################

all:	compiler
compiler: $(TARGET)

$(TARGET): $(OFILES)	
	$(CC) -o $(TARGET) $(OFILES) $(LINKFLAGS)

main main2.o : main2.c $(HCOMMON) 
	$(CC) -c $(CFLAGS) main2.c

scanner scanner.o:  ${HCOMMON} scanner.c
	${CC} ${CFLAGS} -c -o scanner.o scanner.c

scanner.c: scanner.l parser.tab.h 
	${FLEX} ${FLEXFLAGS} scanner.l

parser.output: parser.tab.c

parser parser.o:  parser.tab.c
	${CC} -c ${CFLAGS} -o parser.o parser.tab.c

parser.tab.c: parser2.y  $(HCOMMON)
	${BISON} ${BISONFLAGS} parser2.y

parser.tab.h: parser.tab.c

tables: parser.tab.c parser.output

test:
	bash ./runTests.sh

###########################################################################
#	remove all derived files
###########################################################################

clean:
	rm -f compiler compiler488 $(OFILES) 
	rm -f parser.tab.[ch] parser.output scanner.c
