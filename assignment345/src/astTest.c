/************************************************************************/
/*      Definitions common to all modules in the compiler               */
/************************************************************************/

#include "common.h"
#define EXTERN_GLOBALS
#include "globalvars.h"
#include "ast.h"
#include "symbol.h"

#define DEFAULT_DUMP_FILE       stdout
#define VERBOSITY               1

void runTest(char * desc, int (*test)());
void LOG(char *);
void DESC(char *);

/************************************************************************/
/*                               AST Tests                              */
/************************************************************************/


int testMkProgram(void) 
{
    DESC("create an empty scope");

    LOG("creating a scope...");
    ScopeP scope = malloc(sizeof(ScopeObj));
    scope->scbody = NULL;

    LOG("creating an program for the scope..");
    ObjectP obj = mkProgram(scope);
    LOG("about to dump the tree");
    astDump(obj);

    LOG("free allocated memory");
    free(scope);
    free(obj);
    return 1;
}

int testMkStmt_list(void) 
{
    DESC("create list of statments: assignment, while");

    LOG("creating an assign stmt...");
    StmtP stmt1 = calloc(1, sizeof(StmtObj));
    stmt1->stype = Sassign;

    LOG("creating a while stmt...");
    StmtP stmt2 = calloc(1, sizeof(StmtObj));
    stmt2->stype = Swhile;

    LOG("creating a list with the first stmt..");
    ObjectP stmt_list = mkStmt_list(NULL, stmt1);
    LOG("adding the second stmt to the list..");
    stmt_list = mkStmt_list(stmt_list, stmt2);
    LOG("dump the tree..");
    astDump(stmt_list);
    
    LOG("free allocated memory");
    free(stmt1);
    free(stmt2);
    free(stmt_list->onext);
    free(stmt_list);
    return 1;
}

int testMkDecl_list(void) 
{
    DESC("create a list of declarations: scalar, func");

    LOG("creating an scalar decl...");
    DeclP decl1 = calloc(1, sizeof(DeclObj));
    decl1->dctype = DCscalar;
    LOG("creating func decl...");
    DeclP decl2 = calloc(1, sizeof(DeclObj));
    decl2->dctype = DCfunc;

    LOG("creating a list with the first decl..");
    ObjectP decl_list = mkDecl_list(NULL, decl1);
    LOG("adding the second decl to the list..");
    decl_list = mkDecl_list(decl_list, decl2);
    LOG("dump the tree..");
    astDump(decl_list);

    LOG("free allocated memory");
    free(decl1);
    free(decl2);
    free(decl_list);
    return 1;
}

int testMkExpn_oper_binary(void) {
    DESC("create a binary expression");
    LOG("create the two sub-expressions");
    ExpnP left_expn = malloc(sizeof(ExpnObj)); 
    left_expn->etype = Enone;
    ExpnP right_expn = malloc(sizeof(ExpnObj)); 
    right_expn->etype = Enone;
    int type = Oadd;

    LOG("create the combined expression");
    ExpnP newExpn = mkExpn_oper(left_expn, type, right_expn);

    LOG("create an Object to wrap the expn");
    ObjectP obj = malloc(sizeof(Object));
    obj->otype = Oexpn; 
    obj->ovalue.expn = newExpn;
    obj->onext = NULL;

    LOG("dump the tree..");
    astDump(obj);


    LOG("free allocated memory");
    free(left_expn);
    free(right_expn);
    free(newExpn);
    free(obj);
    return 1;
}

int testMkExpn_oper_unary(void) 
{
    DESC("create a unary expression");
    LOG("create a sub-expressions");
    ExpnP right_expn = malloc(sizeof(ExpnObj)); 
    right_expn->etype = Enone;
    int type = Ouminus;

    LOG("create the combined expression");
    ExpnP newExpn = mkExpn_oper(NULL, type, right_expn);

    LOG("create an Object to wrap the expn");
    ObjectP obj = malloc(sizeof(Object));
    obj->otype = Oexpn; 
    obj->ovalue.expn = newExpn;
    obj->onext = NULL;

    LOG("dump the tree..");
    astDump(obj);

    LOG("free allocated memory");
    free(right_expn);
    free(newExpn);
    free(obj);
    return 1;
}

void test_symbolTable()
{
    SymbTabEntryP entry;
    /* manual testing of symbol table fns. */
    symbolInitialize();
    symbolEnterScope();
    symbolAdd("i", VAR, SCALAR_INT, 0); 
    symbolAdd("j", VAR, SCALAR_INT, 0); 
    symbolAdd("b", VAR, SCALAR_BOOL, 0);
    symbolAdd("a", VAR, ARRAY_INT, 20);
    symbolAdd("foo", PROC, NONE, 0); 
    symbolDumpTable(dumpFile);
    symbolEnterScope(); 
    symbolAdd("i", VAR, SCALAR_INT, 0); 
    symbolAdd("j", VAR, SCALAR_INT, 0);
    symbolAdd("bar", FUNC, NONE, 0);
    symbolEnterScope(); 
    symbolAdd("l", VAR, SCALAR_INT, 0);
    symbolAdd("j", VAR, SCALAR_INT, 0);
    symbolDumpTable(dumpFile);
    entry = symbolLookup("i");
    fprintf(dumpFile, "Looking up symbol i, result:\n");
    symbolDumpTableEntry(dumpFile, entry,-1); 
    symbolExitScope();
    symbolAdd("k", VAR, SCALAR_INT, 0);
    symbolDumpTable(dumpFile);
    symbolExitScope();
    symbolAdd("k", VAR, SCALAR_INT, 0);
    symbolDumpTable(dumpFile);
    symbolExitScope();
    symbolDumpTable(dumpFile);
    symbolFinalize();
}

/************************************************************************/
/*                           Test Framework                             */
/************************************************************************/


int main( int argc , char *argv[] ){
    dumpFile   	= DEFAULT_DUMP_FILE ;
    test_symbolTable();
/*
    runTest("test mkProgram", testMkProgram);
    runTest("test mKStmt_list", testMkStmt_list);
    runTest("test mKDecl_list", testMkDecl_list);
    runTest("test mkExpn_oper: binary expn", testMkExpn_oper_binary); 
    runTest("test mkExpn_operr: unary expn", testMkExpn_oper_unary); 
    */
}

void runTest(char * desc, int (*test)()) 
{
    printf("\n******************* [ %s ] ******************* \n", desc);
    test();
}

void DESC(char * msg)
{
    fprintf(stderr, "DESCRIPTION: %s\n", msg);
}

void LOG(char * msg)
{
    if (VERBOSITY > 0) {
        fprintf(stderr, "\t%s\n", msg);
    }
}

