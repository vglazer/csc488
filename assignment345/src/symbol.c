/********************************************************/
/*							                            */
/*    symbol.c						                    */
/*    Symbol table module for the CSC488S Project	    */
/*							                            */
/********************************************************/

     
/********************************************************/
/*   Include common definitions				            */
/********************************************************/
#include "common.h"
#include "globalvars.h"

/********************************************************/
/*   Include interface to this module			        */
/********************************************************/
#include  "symbol.h"

/* Symbol table entry (internal representation) */
typedef struct SymbTabEntry_tag {
    char *sname;    /* symbol name */    
    int sllevel;
    int sonumber;
    DclType  dctype;
    DataType dtype;
    int dbound;
    FparmP dfparm;
    int mem_offset; /* memory offset */
} SymbTabEntry;
typedef struct SymbTabEntry_tag *SymbTabEntryP;

/* The symbol table */
static SymbTabEntryP SymbTable;

/* Static nesting depth of current scope and order of current variable within 
   current scope */
static int lexicLevel;
static int orderNumber;

/* Index of top symbol table entry */
static int top;

/********************************************************/
/*							                            */
/*   Utility procedures and functions used by the 	    */
/*   symbol table module GO HERE			            */
/*							                            */
/********************************************************/

/* 
 * set the symbol type
 * TODO: don't duplicate lookup code
 */
void symbolSetType( char * sname, DclType dctype)
{
    int i;
    SymbTabEntryP entry; 

    for (i = top - 1; i >= 0; i--) {
         entry = &SymbTable[i];
         if (strcmp(entry->sname, sname) == 0) {
            /* set the type */
            entry->dctype = dctype;
         } 
    }
    
}


/* 
 * associate a memory offset with the symbol 
 * TODO: don't duplicate lookup code
 */
void symbolSetMemOffset( char * sname, int offset )
{
    int i;
    SymbTabEntryP entry; 

    for (i = top - 1; i >= 0; i--) {
         entry = &SymbTable[i];
         if (strcmp(entry->sname, sname) == 0) {
            /* set the offset */
            entry->mem_offset = offset;
         } 
    }
}

/* 
 * get memory offset the symbol 
 * TODO: don't duplicate lookup code
 */
int symbolGetMemOffset( char * sname)
{
    int i;
    SymbTabEntryP entry; 

    for (i = top - 1; i >= 0; i--) {
         entry = &SymbTable[i];
         if (strcmp(entry->sname, sname) == 0) {
            /* return the offset */
            return entry->mem_offset;
         } 
    }

    return -1; /* we won't get here */
}

/* Convert internal entry representation to public format */
void convertEntry(SymbTabEntryP src, SymbTabEntryInfoP dest)
{
    dest->name = src->sname;
    dest->lexic_level = src->sllevel;
    dest->order_number = src->sonumber; 
    dest->dctype = src->dctype;
    dest->dtype = src->dtype;
    dest->dbound = src->dbound;
    dest->dfparm = src->dfparm;
}

/********************************************************/
/*							                            */
/*   Procedures and functions exported by the 		    */
/*   symbol table module				                */
/*							                            */
/********************************************************/

/*   Initialize the symbol table			*/
void symbolInitialize(void) 
{
    SymbTable = calloc(SymbTableSize, sizeof(SymbTabEntry)); 
    lexicLevel = -1;
    orderNumber = top = 0;
}

int symbolLexicLevel(void) 
{
    return lexicLevel;
}

/*   Finalize the symbol table				*/
void symbolFinalize(void)
{
    free(SymbTable);    
}

/* Notify symbol table that we're entering a new scope  */
void symbolEnterScope(void)
{
    lexicLevel++;
    orderNumber = 0;
}

/* Notify symbol table that we're leaving current scope */
void symbolExitScope(void)
{  

    /* Dump error info if requested */
    if (traceSymbols)
        symbolDumpTable(traceFile);
    if (dumpSymbols)
        symbolDumpTable(dumpFile);
 
    if (top != 0) {
       while (top > 0 && SymbTable[top-1].sllevel == lexicLevel) 
          top--;

       if (top == 0 || SymbTable[top - 1].sllevel != lexicLevel-1) 
          /* don't try to obtain order number from topmost *
           * entry at the previous lexical level if it     *
           * doesn't exist */
          orderNumber = 0;
       else {
            SymbTabEntryP prevEntry = &SymbTable[top - 1];  
           /* routine decls take up one word on the stack */
           if (prevEntry->dctype == DCproc || prevEntry->dctype == DCfunc) {
               orderNumber = prevEntry->sonumber + 1;
           } else {
                orderNumber = prevEntry->sonumber + prevEntry->dbound;
           }
       }
    }

    lexicLevel--;
}

/* Add a new entry to symbol table */
void symbolAddDecl(DeclP declP)
{
    SymbTable[top].sname = declP->dname;
    SymbTable[top].sonumber = orderNumber;
    SymbTable[top].sllevel = lexicLevel;
    SymbTable[top].dctype = declP->dctype;
    SymbTable[top].dtype = declP->dtype;
    SymbTable[top].dbound = declP->dbound;
    SymbTable[top].dfparm = declP->dfparm;

    /* routine decls take up one word on the stack */
    if (declP->dctype == DCproc || declP->dctype == DCfunc) {
        /* note: this bit of code is somewhat redunant since if this is a
         * declaration for a routine then we're about to jump in and out of
         * it's scope, and so the the symbolExitScope code is ultimately
         * responsible for restoring the orderNumber correctly 
         */
        orderNumber++;
    } else {
        orderNumber += declP->dbound;
    }

    top++;
}

void symbolAddFparm( FparmP dfparm )
{
    SymbTable[top].sname = dfparm->fname;
    SymbTable[top].sonumber = orderNumber;
    SymbTable[top].sllevel = lexicLevel;
    SymbTable[top].dctype = DCscalar;
    SymbTable[top].dtype = dfparm->ftype;
    SymbTable[top].dbound = 1;  /* size of a scalar */
    SymbTable[top].dfparm = NULL;

    orderNumber++;  /* parameters are only scalars */
    top++;
}

/* retrieve the last symbol table entry with name sname */
/* (where last means having the highest index) */
BOOLEAN symbolLookup(char *sname, SymbTabEntryInfoP sentryinfo)
{
    int i;
    SymbTabEntryP entry; 
    assert(sentryinfo != NULL); 

    for (i = top - 1; i >= 0; i--) {
         entry = &SymbTable[i];
         if (strcmp(entry->sname, sname) == 0) {
              convertEntry(entry, sentryinfo);
              return TRUE; 
         } 
    }

    return FALSE;
}

/* print the symbol table to FILE *stream */
void symbolDumpTable(FILE *stream)
{
    int i; 
    fprintf(stream, "*** Dumping Symbol Table ***\n");
    fprintf(stream, "lexicLevel:%d\n", lexicLevel);
    fprintf(stream, "orderNumber:%d\n", orderNumber);
    fprintf(stream, "SymbTableSize:%d\n", SymbTableSize);
    fprintf(stream, "top:%d\n\n", top);
    fprintf(stream, "%15s%15s%15s%15s", "Entry#","sname","skind","stype");
    fprintf(stream, "%15s%15s%15s\n", "snumelems","slevel","sonumber");

    for (i = 0; i < top; i++) {
        SymbTabEntryInfo entryinfo;
        convertEntry(&SymbTable[i], &entryinfo);
        symbolDumpTableEntry(stream, &entryinfo, i);
    }

    fprintf(stream, "*** Done Symbol Table ***\n");
}

/* print the symbol table with index i to FILE *stream */
void symbolDumpTableEntry(FILE *stream, SymbTabEntryInfoP entry, int entryIndex)
{
   static const char *skindstrings[7] = { "none", "scalar", "array",
"function", "proc", "f forward", "p forward" };
   static const char *stypestrings[5] = { "none", "boolean", "int", "text", "void"};
   fprintf(stream, "%15d", entryIndex);
   fprintf(stream, "%15s", entry->name);
   fprintf(stream, "%15s", skindstrings[entry->dctype]);
   fprintf(stream, "%15s", stypestrings[entry->dtype]);
   fprintf(stream, "%15d", entry->dbound);
   fprintf(stream, "%15d", entry->lexic_level);
   fprintf(stream, "%15d\n", entry->order_number);
}

