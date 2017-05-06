###########################################################################
#	
#	Makefile
#
#       Makefile version:  $Id: Makefile,v 1.12 2003/04/08 04:48:44 g9macrya Exp $
#
#	makefile for compiler  - csc488s course project compiler
#
#	This file describes the dependencies between the
#	various compiler components.
###########################################################################

###########################################################################
#	
#	This make file provides the following targets
#
#	make	all			Build the complete compiler (debuggable version)
#	make	opt			Build the complete compiler (optimized version)
#	make	compiler	Build the complete compiler
#	make 	depend		Invoke makedepend to build dependency information for C files
#	make	clean		Remove all derived files
#	make 	test        Run tests
#	make    postman		Build PostScript version of compiler man page
#	make	textman		Build text version of compiler man page
#	
#	make    scanner		Build the scanner module
#	make	parser		Build the parser module
#	make	ast			Build the AST module
#	make	main		Build the main driver program
#	make	semantics	Build the semantics analysis module
#	make    codegen		Build the code generation module
#	make	machine		Build the machine interpreter module
#
###########################################################################

###########################################################################
#	
#	Define some useful constants
#
###########################################################################

#   Make command
MAKE  = make

#   Manual processing commands and flags
PSROFF	= groff
PSFLAGS = -man -Tps
NROFF	= groff
NFLAGS	= -man -Tascii

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
CFLAGS = -g -Wall -ansi -pedantic -std=gnu99 -Werror
CFLAGS_DBG = -g
CFLAGS_OPT = -O2 -DNDEBUG

PLATFORM = $(shell uname -s)
ifeq ($(PLATFORM),Darwin)
	FLEXLIB = l
	MAKEDEP = gcc -M
	PSTOPDF = pstopdf
else
	FLEXLIB = fl
	MAKEDEP = makedepend
	PSTOPDF = ps2pdf
endif
#	Linker flags
LINKFLAGS = -l$(FLEXLIB)

###########################################################################
#	
#	Define files that make up the compiler source
#	
###########################################################################

HINTERFACES = ast.h semantics.h symbol.h machine.h 
HCOMMON     = common.h globalvars.h
HAST	    = astDef.h  
HMACHINE    = machineDef.h
 
HFILES  =  $(HINTERFACES) $(HTABLES) $(HCOMMON) $(HMACHINE) $(HAST)
  
CFILES = main.c scanner.c parser.tab.c semantics.c codegen.c symbol.c machine.c ast.c astDump.c

OFILES = main.o scanner.o parser.o semantics.o codegen.o symbol.o machine.o ast.o astDump.o

###########################################################################
#	
#   Name for the executable compiler
#	
###########################################################################

TARGET	=  compiler488

###########################################################################
#	
#	Dependencies for the compiler
#	
###########################################################################

all:	CFLAGS += $(CFLAGS_DBG)
all:	depend compiler astTest
compiler: $(TARGET)

opt:	CFLAGS += $(CFLAGS_OPT)
opt:	all

$(TARGET): $(OFILES)
	$(CC) -o $(TARGET) $(OFILES) $(LINKFLAGS)

main.o: main.c $(HINTERFACES) $(HCOMMON) 
	$(CC) -c $(CFLAGS) main.c

scanner.o: scanner.c ${HCOMMON}
	${CC} ${CFLAGS} -c scanner.c

scanner.c: scanner.l parser.tab.h $(HCOMMON)
	${FLEX} ${FLEXFLAGS} scanner.l

parser.output: parser.tab.c

parser.o: parser.tab.c ${HCOMMON}
	${CC} -c ${CFLAGS} -o parser.o parser.tab.c

parser.tab.c: parser.y $(HCOMMON)
	${BISON} ${BISONFLAGS} parser.y

parser.tab.h: parser.tab.c

ast.o: ast.c ast.h $(HAST) ${HCOMMON} 
	$(CC) -c $(CFLAGS) ast.c

astDump.o: astDump.c ast.h $(HAST) ${HCOMMON} 
	$(CC) -c $(CFLAGS) astDump.c
	
astTest.o: astTest.c ast.h ${HCOMMON}
	$(CC) -c $(CFLAGS) astTest.c

semantics.o: semantics.c semantics.h symbol.h ${HCOMMON} 
	$(CC) -c $(CFLAGS) semantics.c

codegen.o: codegen.c codegen.h machine.h $(HMACHINE) $(HCOMMON) 
	$(CC) -c $(CFLAGS) codegen.c

symbol.o: symbol.c symbol.h $(HCOMMON)
	$(CC) -c $(CFLAGS) symbol.c

machine.o: machine.c machine.h $(HMACHINE) $(HCOMMON)
	$(CC) -c $(CFLAGS) machine.c

machineDef.h: common.h 

tables: parser.tab.c parser.output

astTest: astTest.o astDump.o ast.o symbol.o
	$(CC) -o astTest astTest.o astDump.o ast.o symbol.o $(LINKFLAGS) 

###########################################################################
#	remove all derived files
###########################################################################

clean:
	rm -f compiler compiler488 $(OFILES) astTest.o
	rm -f compiler488.txt compiler488.ps compiler488.pdf
	rm -f parser.tab.[ch] parser.output scanner.c *~
	rm -f .depend .*.swp tags

###########################################################################
# Run tests
###########################################################################
test:
	cd ../test && bash ./run_codegentests.sh && cd -

###########################################################################
# Run tests
###########################################################################
tags:
	ctags -R

###########################################################################
#	
#  Make printable versions of compiler488.man 
#	
###########################################################################

postman:  $(TARGET).ps		#  Postscript
pdfman:  $(TARGET).pdf		#  PDF

MANFILE = ../doc/compiler488.man

$(TARGET).ps: $(MANFILE)
	$(PSROFF) $(PSFLAGS) $(MANFILE) > $(TARGET).ps

$(TARGET).pdf: $(TARGET).ps
	$(PSTOPDF) $(TARGET).ps

textman:  $(TARGET).txt		#  ASCII text

$(TARGET).txt: $(MANFILE)
	$(NROFF)  $(NFLAGS)  $(MANFILE) > $(TARGET).txt

###########################################################################
#	
#  Make dependencies for source files 
#	
###########################################################################

.depend:  scanner.c parser.tab.c
	touch  .depend
	

depend: .depend
	$(MAKEDEP)  $(CFILES) 
	
# DO NOT DELETE THIS LINE -- make depend depends on it.
