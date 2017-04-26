#ifndef _SYMBOL_H_
/********************************************************/
/*                                                      */
/*  symbol.h                                            */
/*  Interface to the symbol table module                */
/*                                                      */
/********************************************************/
#define _SYMBOL_H_

/* Symbol table size */
#define SymbTableSize 200

/* Symbol table entryinfo struct.  Used to pass values *
 * to and from the symbol table */
typedef struct SymbTabEntryInfo_tag {
   char *name;
   int lexic_level;
   int order_number;
   DclType  dctype;
   DataType dtype;
   int dbound;
   FparmP dfparm;
   
} SymbTabEntryInfo;
typedef struct SymbTabEntryInfo_tag *SymbTabEntryInfoP;

 

/********************************************************/
/*  Symbol table initialization                         */
/********************************************************/

void symbolInitialize(void);

/********************************************************/
/*  Symbol table finalization                           */
/********************************************************/

void symbolFinalize(void);

/********************************************************/
/*  Additional functions and procedures in the          */
/*  interface to the symbol table module                */
/*  GO HERE                                             */
/********************************************************/

/* Notify symbol table that we're entering a new scope */
void symbolEnterScope(void);

/* Notify symbol table that we're leaving current scope */
void symbolExitScope(void);

/* Add a new entry to symbol table */
void symbolAddDecl( DeclP decl );

void symbolAddFparm( FparmP dfparm );

int symbolLexicLevel(void);

/* retrieve the last symbol table entry with name sname */
/* (where last means having the highest index) */
BOOLEAN symbolLookup(char *sname, SymbTabEntryInfoP sentryinfo);

/* print the symbol table to FILE *stream */
void symbolDumpTable(FILE *stream);

/* print the symbol table with index i to FILE *stream */
void symbolDumpTableEntry(FILE *stream, SymbTabEntryInfoP entry, int entryIndex);

/* 
 * set the symbol type
 */
void symbolSetType ( char * sname, DclType dctype );
void symbolSetType ( char * sname, DclType dctype );
void symbolSetType ( char * sname, DclType dctype );
void symbolSetType ( char * sname, DclType dctype );
void symbolSetType ( char * sname, DclType dctype );

/* 
 * associate a memory offset with the symbol 
 */
void symbolSetMemOffset( char * sname, int offset );

/* 
 * get memory offset of the symbol 
 */
int  symbolGetMemOffset( char * sname );

#endif

