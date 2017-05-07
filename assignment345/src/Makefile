###########################################################################
#	
#	Makefile for compiler  - csc488s course project compiler
#
#	This file describes the dependencies between the various compiler components
#
###########################################################################

###########################################################################
#	
#	This make file provides the following targets
#
#	make	all			Build the compiler and AST test, generate manual (debuggable)
#	make	opt			Build the compiler and AST test, generate manual (optimized)
#	make	compiler	Build the complete compiler
#	make 	test        Run tests
#	make    postman		Build PostScript version of compiler man page
#	make	textman		Build text version of compiler man page
#	make	clean		Remove all derived files
#
###########################################################################

###########################################################################
#	
#	Define some useful constants
#
###########################################################################

#   Make command
MAKE = make

#   Manual processing commands and flags
PSROFF = groff
PSFLAGS = -man -Tps
NROFF = groff
NFLAGS = -man -Tascii

#   Flex and Bison commands
FLEX = flex
FLEXFLAGS = -oscanner.c -Cae -I
BISON = bison
BISONFLAGS = --debug --file-prefix=parser --defines --verbose

###########################################################################
#
# 	Compiler and Compiler Flags (an ANSI C compiler is required)
#
###########################################################################

CC = gcc
CFLAGS = -Wall -ansi -pedantic -Werror -std=gnu99 
CFLAGS_DBG = -g
CFLAGS_OPT = -O2 -DNDEBUG

PLATFORM = $(shell uname -s)
ifeq ($(PLATFORM),Darwin)
	FLEXLIB = l
	PSTOPDF = pstopdf
else
	FLEXLIB = fl
	PSTOPDF = ps2pdf
endif

#	Linker flags
LINKFLAGS = -l$(FLEXLIB)

###########################################################################
#	
#	Define files that make up the compiler source
#	
###########################################################################

HCOMMON = common.h globalvars.h
CFILES = main.c scanner.c parser.tab.c semantics.c codegen.c symbol.c machine.c ast.c astDump.c
OFILES = main.o scanner.o parser.o semantics.o codegen.o symbol.o machine.o ast.o astDump.o

###########################################################################
#	
#  Targets for building the executables 
#	
###########################################################################

all:	CFLAGS += $(CFLAGS_DBG)
all:	compiler ast_test textman pdfman tags

opt:	CFLAGS += $(CFLAGS_OPT)
opt:	all

TARGET = compiler488
AST_TEST_TARGET = astTest

compiler: $(TARGET)
ast_test: $(AST_TEST_TARGET)

$(TARGET): $(OFILES)
	$(CC) -o $(TARGET) $(OFILES) $(LINKFLAGS)

$(AST_TEST_TARGET): astTest.o astDump.o ast.o symbol.o
	$(CC) -o $(AST_TEST_TARGET) astTest.o astDump.o ast.o symbol.o $(LINKFLAGS) 

###########################################################################
#
#    Flex and Bison-related targets
#
###########################################################################

scanner.c: scanner.l parser.tab.h $(HCOMMON)
	$(FLEX) $(FLEXFLAGS) scanner.l

# this target links scanner.c with parser.tab.c
parser.tab.h: parser.tab.c

parser.tab.c: parser.y $(HCOMMON)
	$(BISON) $(BISONFLAGS) parser.y

###########################################################################
#	
#    Object files
#	
###########################################################################

ast.o: ast.c ast.h astDef.h $(HCOMMON) 
	$(CC) -c $(CFLAGS) ast.c

astDump.o: astDump.c ast.h astDef.h $(HCOMMON) 
	$(CC) -c $(CFLAGS) astDump.c
	
astTest.o: astTest.c ast.h $(HCOMMON)
	$(CC) -c $(CFLAGS) astTest.c

codegen.o: codegen.c codegen.h machine.h machineDef.h $(HCOMMON) 
	$(CC) -c $(CFLAGS) codegen.c

machine.o: machine.c machine.h machineDef.h $(HCOMMON)
	$(CC) -c $(CFLAGS) machine.c

main.o: main.c ast.h machine.h semantics.h symbol.h $(HCOMMON) 
	$(CC) -c $(CFLAGS) main.c

parser.o: parser.tab.c $(HCOMMON)
	$(CC) -c $(CFLAGS) -o parser.o parser.tab.c

scanner.o: scanner.c $(HCOMMON)
	$(CC) $(CFLAGS) -c scanner.c

semantics.o: semantics.c semantics.h symbol.h $(HCOMMON) 
	$(CC) -c $(CFLAGS) semantics.c

symbol.o: symbol.c symbol.h $(HCOMMON)
	$(CC) -c $(CFLAGS) symbol.c

###########################################################################
#
#    Run tests
#
###########################################################################

test:
	cd ../test && bash ./run_codegentests.sh && cd - && ./$(AST_TEST_TARGET)

###########################################################################
#
#    Generate ctags
#
###########################################################################

tags: *.h *.c
	ctags -R

###########################################################################
#	
#    Generate man files in various formats
#	
###########################################################################

postman: $(TARGET).ps		#  Postscript
pdfman: $(TARGET).pdf		#  PDF

MANFILE = ../doc/$(TARGET).man

$(TARGET).ps: $(MANFILE)
	$(PSROFF) $(PSFLAGS) $(MANFILE) > $(TARGET).ps

$(TARGET).pdf: $(TARGET).ps
	$(PSTOPDF) $(TARGET).ps

textman: $(TARGET).txt		#  ASCII text

$(TARGET).txt: $(MANFILE)
	$(NROFF) $(NFLAGS) $(MANFILE) > $(TARGET).txt

###########################################################################
#
#    Remove all generated files
#
###########################################################################

clean:
	rm -f $(TARGET) $(AST_TEST_TARGET)
	rm -f scanner.c parser.tab.[ch] *.o parser.output
	rm -f $(TARGET).{pdf,ps,txt}
	rm -f .depend tags
