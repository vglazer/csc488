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
#	make	all			Build the complete compiler
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

all:	depend compiler
compiler: $(TARGET)

$(TARGET): $(OFILES)	
	$(CC) -o $(TARGET) $(OFILES) $(LINKFLAGS)

main main.o : main.c $(HCOMMON) machine.h semantics.h codegen.h
	$(CC) -c $(CFLAGS) main.c

scanner scanner.o:  ${HCOMMON} scanner.c
	${CC} ${CFLAGS} -c -o scanner.o scanner.c

scanner.c: scanner.l parser.tab.h 
	${FLEX} ${FLEXFLAGS} scanner.l

parser.output: parser.tab.c

parser parser.o:  parser.tab.c
	${CC} -c ${CFLAGS} -o parser.o parser.tab.c

parser.tab.c: parser.y  $(HCOMMON)
	${BISON} ${BISONFLAGS} parser.y

parser.tab.h: parser.tab.c

ast ast.o:	ast.h astDef.h ast.c
	$(CC) -c $(CFLAGS) ast.c
	
astDump astDump.o:	ast.h astDef.h astDump.c
	$(CC) -c $(CFLAGS) astDump.c
	
semantics semantics.o : semantics.h semantics.c ${HCOMMON}  symbol.h 	 
	$(CC) -c $(CFLAGS) semantics.c

codegen codegen.o : codegen.h codegen.c  $(HCOMMON) machineDef.h machine.h
	$(CC) -c $(CFLAGS) codegen.c

symbol symbol.o : symbol.h symbol.c $(HCOMMON)
	$(CC) -c $(CFLAGS) symbol.c

machine machine.o :  machine.h machine.c $(HCOMMON) machineDef.h 
	$(CC) -c $(CFLAGS) machine.c

machineDef.h:     common.h 


tables: parser.tab.c parser.output

astTest: astDump.o ast.o symbol.o astTest.c 
	$(CC) -o astTest astTest.c astDump.o ast.o symbol.o

###########################################################################
#	remove all derived files
###########################################################################

clean:
	rm -f compiler compiler488 $(OFILES) 
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

main.o: common.h /usr/include/stdio.h /usr/include/features.h
main.o: /usr/include/stdc-predef.h /usr/include/libio.h
main.o: /usr/include/_G_config.h /usr/include/wchar.h /usr/include/assert.h
main.o: /usr/include/string.h /usr/include/xlocale.h /usr/include/stdlib.h
main.o: /usr/include/alloca.h /usr/include/getopt.h globalvars.h ast.h
main.o: astDef.h semantics.h codegen.h machine.h
scanner.o: /usr/include/stdio.h /usr/include/features.h
scanner.o: /usr/include/stdc-predef.h /usr/include/libio.h
scanner.o: /usr/include/_G_config.h /usr/include/wchar.h
scanner.o: /usr/include/string.h /usr/include/xlocale.h /usr/include/errno.h
scanner.o: /usr/include/stdlib.h /usr/include/alloca.h common.h
scanner.o: /usr/include/assert.h /usr/include/ctype.h /usr/include/endian.h
scanner.o: globalvars.h ast.h astDef.h parser.tab.h /usr/include/unistd.h
scanner.o: /usr/include/getopt.h
parser.tab.o: common.h /usr/include/stdio.h /usr/include/features.h
parser.tab.o: /usr/include/stdc-predef.h /usr/include/libio.h
parser.tab.o: /usr/include/_G_config.h /usr/include/wchar.h
parser.tab.o: /usr/include/assert.h /usr/include/string.h
parser.tab.o: /usr/include/xlocale.h /usr/include/stdlib.h
parser.tab.o: /usr/include/alloca.h globalvars.h ast.h astDef.h
semantics.o: common.h /usr/include/stdio.h /usr/include/features.h
semantics.o: /usr/include/stdc-predef.h /usr/include/libio.h
semantics.o: /usr/include/_G_config.h /usr/include/wchar.h
semantics.o: /usr/include/assert.h /usr/include/string.h
semantics.o: /usr/include/xlocale.h /usr/include/stdlib.h
semantics.o: /usr/include/alloca.h globalvars.h ast.h astDef.h semantics.h
semantics.o: symbol.h codegen.h
codegen.o: common.h /usr/include/stdio.h /usr/include/features.h
codegen.o: /usr/include/stdc-predef.h /usr/include/libio.h
codegen.o: /usr/include/_G_config.h /usr/include/wchar.h
codegen.o: /usr/include/assert.h /usr/include/string.h /usr/include/xlocale.h
codegen.o: /usr/include/stdlib.h /usr/include/alloca.h globalvars.h ast.h
codegen.o: astDef.h machine.h machineDef.h symbol.h
symbol.o: common.h /usr/include/stdio.h /usr/include/features.h
symbol.o: /usr/include/stdc-predef.h /usr/include/libio.h
symbol.o: /usr/include/_G_config.h /usr/include/wchar.h /usr/include/assert.h
symbol.o: /usr/include/string.h /usr/include/xlocale.h /usr/include/stdlib.h
symbol.o: /usr/include/alloca.h globalvars.h ast.h astDef.h symbol.h
machine.o: common.h /usr/include/stdio.h /usr/include/features.h
machine.o: /usr/include/stdc-predef.h /usr/include/libio.h
machine.o: /usr/include/_G_config.h /usr/include/wchar.h
machine.o: /usr/include/assert.h /usr/include/string.h /usr/include/xlocale.h
machine.o: /usr/include/stdlib.h /usr/include/alloca.h globalvars.h ast.h
machine.o: astDef.h machineDef.h
ast.o: common.h /usr/include/stdio.h /usr/include/features.h
ast.o: /usr/include/stdc-predef.h /usr/include/libio.h
ast.o: /usr/include/_G_config.h /usr/include/wchar.h /usr/include/assert.h
ast.o: /usr/include/string.h /usr/include/xlocale.h /usr/include/stdlib.h
ast.o: /usr/include/alloca.h globalvars.h ast.h astDef.h
astDump.o: common.h /usr/include/stdio.h /usr/include/features.h
astDump.o: /usr/include/stdc-predef.h /usr/include/libio.h
astDump.o: /usr/include/_G_config.h /usr/include/wchar.h
astDump.o: /usr/include/assert.h /usr/include/string.h /usr/include/xlocale.h
astDump.o: /usr/include/stdlib.h /usr/include/alloca.h globalvars.h ast.h
astDump.o: astDef.h
