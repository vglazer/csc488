
/****************************************************************/
/*                                                              */
/*      codegen.c                                               */
/*      Code Generation module for the CSC488S Project Compiler */
/*                                                              */
/****************************************************************/


/****************************************************************/
/*                                                              */
/*  Code Generation Conventions                                 */
/*                                                              */
/*  To simplify the course project, this code generator is      */
/*  designed to compile directly to pseudo machine memory       */
/*  which is available as the array memory[] in machineDef.h    */
/*                                                              */
/*  It is assumed that the code generator places instructions   */
/*  in memory in locations                                      */
/*      memory[ 0 .. startMSP - 1 ]                             */
/*                                                              */
/*  The code generator may also place intstructions and/or      */
/*  constants in high memory at locations                       */
/*      memory[ startMLP .. MEMORYSIZE - 1 ]                    */
/*                                                              */
/*  During program exection the memory area                     */
/*      memory[ startMSP .. startMLP - 1 ]                      */
/*  is used as a dynamic stack for storing activation records   */
/*  and temporaries used during expression evaluation.          */
/*  A hardware exception (stack overflow) occurs if the pointer */
/*  for this stack reaches the memory limit register (mlp).     */
/*                                                              */
/*  The code generator is responsible for setting the global    */
/*  variables:                                                  */
/*      startPC         initial value for program counter       */
/*      startMSP        initial value for msp                   */
/*      startMLP        initial value for mlp                   */
/*                                                              */
/****************************************************************/

/****************************************************************/
/*  Include common definitions and global variables             */
/****************************************************************/
#include "common.h"
#include "globalvars.h"
#include <string.h>
/****************************************************************/
/*  Include machine definitions for code generation             */
/****************************************************************/
#include "machine.h" 
#include "machineDef.h"

/****************************************************************/
/*  Include symbol table definitions for code generation        */
/****************************************************************/
#include "symbol.h"


/****************************************************************/
/*                                                              */
/*  Internal data structure definitions                         */ 
/*                                                              */
/****************************************************************/

/* Flags for passing information between procedures */
struct Flags_tag {
    int resultBackpatchHead;
    int returnBackpatchHead;
    int exitBackpatchHead;    /* address to head of list        */
    int llEnclosingLoop; /* lexic level of enclosing loop */
    int llEnclosingRoutine; /* lexic level of enclosing routine's body */
} Flags;
typedef struct Flags_tag * FlagsP;

/****************************************************************/
/*                                                              */
/*  Internal procedure declarations                             */ 
/*                                                              */
/****************************************************************/

/* Flag manipulation */
FlagsP codegenInitFlags( );
FlagsP codegenCopyFlags( FlagsP flags );

/* Scopes */
void codegenScope( ScopeP scope, FlagsP flags );
void codegenMajorScopeSetup( ScopeP scope );
void codegenMajorScopeCleanup( int lexiclevel );
void codegenRoutineSetup( ScopeP scope, int lexiclevel, int argcount );
void codegenRoutineCleanup( ScopeP scope, int lexiclevel, int argcount );

/* Declarations */
void codegenDecl( DeclP decl );
void codegenDeclScalar( DeclP decl );
void codegenDeclArray( DeclP decl );
void codegenDeclRoutine( DeclP decl, BOOLEAN isFunction );

/* Routine (function or procedure) calls */
void codegenRoutineCall( char *name, ArgsP args, BOOLEAN isFunction );

/* Statements */
void codegenStmt( StmtP stmt, FlagsP flags ) ;
void codegenStmtList( ObjectP stmts, FlagsP flags );
void codegenStmtAssert( StmtP stmt );
void codegenStmtAssign( StmtP stmt ); 
void codegenStmtIf( StmtP stmt, FlagsP flags); 
void codegenStmtIfElse( StmtP stmt, FlagsP flags );
void codegenStmtWhile( StmtP stmt, FlagsP flags, int incramt, ExpnP incrvar ); 
void codegenStmtDo( StmtP stmt, FlagsP flags );
void codegenStmtFor( StmtP stmt, FlagsP flags );
void codegenStmtExit( StmtP stmt, FlagsP flags ); 
void codegenStmtReturn( StmtP stmt, FlagsP flags );
void codegenStmtResult( StmtP stmt, FlagsP flags );
void codegenStmtPut( StmtP stmt );
void codegenStmtGet( StmtP stmt );
void codegenStmtMajorScope( StmtP stmt, FlagsP flags );

/* Expressions */
void codegenPushExpn( ExpnP expn );
void codegenPushExpnConst( ExpnP expn );
void codegenPushExpnVar( ExpnP expn );
void codegenPushExpnUnary( ExpnP expn );
void codegenPushExpnCond( ExpnP expn );
void codegenPushExpnBinary( ExpnP expn );
void codegenPushExpnBinaryBuiltIn( ExpnP expn, int operator );
void codegenPushExpnBinaryComparison( ExpnP lop, ExpnP rop, int threshold );
void codegenPushExpnBinaryNEq( ExpnP expn ); 
void codegenPushExpnBinarySubs( ExpnP expn );
void codegenPushExpnBinaryAnd( ExpnP expn );
void codegenPushExpnBinaryOr( ExpnP expn );

/****************************************************************/
/*                                                              */
/*  Internal Function definitions                               */
/*                                                              */
/****************************************************************/

/****************************************************************/  
/*                                                              */
/*  Flag manipulation                                           */
/*                                                              */
/****************************************************************/

/* 
 * Create and return a new copy of the flags structure
 * with the fields initialised to their default values 
 */
FlagsP codegenInitFlags( )
{
    /* Allocate memory */
    FlagsP flags = calloc( 1, sizeof( Flags ) );

    /* Initialize fields to their defailt values */
    flags->exitBackpatchHead    = UNDEFINED;
    flags->resultBackpatchHead  = UNDEFINED;
    flags->returnBackpatchHead  = UNDEFINED;
    flags->llEnclosingLoop      = 0;
    flags->llEnclosingRoutine   = 0;

    return flags;
}

/* 
 * Create and return a new flags structure with the 
 * fields initialized to the specified values 
 */
FlagsP codegenCopyFlags( FlagsP flags )
{
    /* Allocate memory */
    FlagsP newFlags = calloc( 1, sizeof( Flags ) );

    /* Initialise fields to the specified values */
    memcpy( newFlags, flags, sizeof( Flags ) );

    return newFlags;
}

/****************************************************************/
/*                                                              */
/*  Scopes                                                      */
/*                                                              */
/****************************************************************/

/* 
 * Emit code for the contents of the specified scope
 */
void codegenScope( ScopeP scope, FlagsP flags )
{
    ObjectP trace = scope->scbody; 
    while ( trace != NULL ) { 
        switch( trace->otype ) {
            case Odecl:
                codegenDecl( trace->ovalue.decl );
                break;

            case Ostmt:
                codegenStmt( trace->ovalue.stmt, flags );
                break;

            default:
                assert( FALSE ); /* can't happen */
        }
        trace = trace->onext;
    }
}

/* 
 * Emit setup code for a major scope, where by "major scope"
 * we mean any scope not associated with a function or a procedure
 * (all scopes are treated in a uniform fashion).
 */
void codegenMajorScopeSetup( ScopeP scope )
{
    int lexiclevel = symbolLexicLevel( );

    /* back up current display */
    memory[startMSP++] = ADDR;
    memory[startMSP++] = lexiclevel;
    memory[startMSP++] = 0;

    /* update current display */
    memory[startMSP++] = PUSHMT;
    memory[startMSP++] = SETD;
    memory[startMSP++] = lexiclevel;
}

/* 
 * Emit cleanup code for a major scope 
 */
void codegenMajorScopeCleanup( int lexiclevel )
{
    /* Pop local variables */
    memory[startMSP++] = PUSHMT;
    memory[startMSP++] = ADDR;
    memory[startMSP++] = lexiclevel;
    memory[startMSP++] = 0;
    memory[startMSP++] = SUB;
    memory[startMSP++] = POPN;

    /* restore display */
    memory[startMSP++] = SETD;
    memory[startMSP++] = lexiclevel;
}

/*
 * Emit setup code for a routine, where by "routine" we mean 
 * a function or a procedure 
 */
void codegenRoutineSetup( ScopeP scope, int lexiclevel, int argcount )
{
    /* Roll back the display to allow access to locals */
    memory[startMSP++] = PUSHMT;
    memory[startMSP++] = PUSH; 
    memory[startMSP++] = argcount; 
    memory[startMSP++] = SUB;
    memory[startMSP++] = SETD;
    memory[startMSP++] = lexiclevel;
}

/*
 * Emit cleanup code for a routine 
 */
void codegenRoutineCleanup( ScopeP scope, int lexiclevel, int argcount )
{
    /* Deallocate local storage */
    memory[startMSP++] = PUSHMT;
    memory[startMSP++] = ADDR; 
    memory[startMSP++] = lexiclevel; 
    memory[startMSP++] = 0;
    memory[startMSP++] = SUB;
    memory[startMSP++] = POPN;

    /* Return control to caller */
    memory[startMSP++] = BR;
}

/****************************************************************/
/*                                                              */
/*  Declarations                                                */
/*                                                              */
/****************************************************************/

/*
 * Generate the code for a declaration
 */
void codegenDecl( DeclP decl )
{
    SymbTabEntryInfo symEntryInfo; 

    /* initialize symbol table entry */
    symEntryInfo.dctype = DCnone; 

    switch( decl->dctype ) {
        case DCscalar: 
            symbolAddDecl( decl );
	    codegenDeclScalar( decl );
            break;

        case DCarray: 
            symbolAddDecl( decl );
            symbolSetMemOffset( decl->dname, startMSP );
	    codegenDeclArray( decl );
            break;

        case DCfunc:
	    codegenDeclRoutine( decl, TRUE );
            break; 

        case DCproc: 
	    codegenDeclRoutine( decl, FALSE );
            break;

        case DCforward_func:
            if ( ( symbolLookup( decl->dname, &symEntryInfo ) == FALSE )
                    || ( symEntryInfo.dctype != DCforward_func ) 
               ) {
                /* add to the symbol table */
                symbolAddDecl( decl );

                /* push address of routine body */
                memory[startMSP++] = PUSH;
                /* store the function address address in the symbol table */
                symbolSetMemOffset( decl->dname, startMSP );
                memory[startMSP++] = UNDEFINED; 

            } 
            break;

        case DCforward_proc:
            if ( ( symbolLookup( decl->dname, &symEntryInfo ) == FALSE )
                    || ( symEntryInfo.dctype != DCforward_proc ) 
               ) {
                /* add to the symbol table */
                symbolAddDecl( decl );

                /* push address of routine body */
                memory[startMSP++] = PUSH;
                /* store the function address address in the symbol table */
                symbolSetMemOffset( decl->dname, startMSP );
                memory[startMSP++] = UNDEFINED; 
            } 
            break;
        default:
            assert( FALSE ); /* can't happen */
    }
}

/* 
 * Scalars 
 */ 
void codegenDeclScalar( DeclP decl )
{
   if (decl->dinitexpn == NULL) 
   {
       memory[startMSP++] = PUSH;
       memory[startMSP++] = UNDEFINED;
   }
   else
       codegenPushExpn( decl->dinitexpn );
}

/* 
 * Arrays 
 */
void codegenDeclArray( DeclP decl )
{
    int i;

    for (i = 0; i < decl->dbound; i++) {
        memory[startMSP++] = PUSH;
        memory[startMSP++] = UNDEFINED;
    }
}

/* 
 * Routines (functions and procedures)
 */
void codegenDeclRoutine( DeclP decl, BOOLEAN isFunction )
{
    FparmP param;
    int addr1, addr2;
    FlagsP flags;
    SymbTabEntryInfo symEntryInfo; 

    /* initialize symbol table entry */
    symEntryInfo.dctype = DCnone; 

    /* allocate and initialise flags */
    flags = codegenInitFlags( );

    if ( ( symbolLookup( decl->dname, &symEntryInfo ) == FALSE )
            || ( ( symEntryInfo.dctype != DCforward_proc ) 
              && ( symEntryInfo.dctype != DCforward_func ) )  
       ) {
            
        symbolAddDecl( decl );

        /* push address of routine body */
        memory[startMSP++] = PUSH;
        addr1              = startMSP;
        memory[startMSP++] = UNDEFINED; 

    } else {
        symbolSetType( decl->dname, decl->dctype );
        addr1 = symbolGetMemOffset( decl->dname );
    }

    /* enter routine scope */
    symbolEnterScope( );


    /* set up jump over routine declaration */
    memory[startMSP++] = PUSH;
    addr2              = startMSP;
    memory[startMSP++] = UNDEFINED; 
    memory[startMSP++] = BR;

    /* backpatch address of routine body */
    memory[addr1]      = startMSP;

    /* emit setup code */
    codegenRoutineSetup( decl->dobject->ovalue.scope, symbolLexicLevel( ),
                         decl->dbound );
    /* add parameter declarations to the symbol table */
    param = decl->dfparm;
    while ( param != NULL ) {
        symbolAddFparm( param );
        param = param->fnext;
    }

    /* emit code for routine body */
    flags->llEnclosingRoutine = symbolLexicLevel( );
    codegenScope( decl->dobject->ovalue.scope, flags );

    /* backpatch all result / return statements in routine body */
    if ( isFunction == TRUE ) 
        while ( flags->resultBackpatchHead != UNDEFINED ) {
            int resultAddr = flags->resultBackpatchHead; 
            flags->resultBackpatchHead = memory[resultAddr];
            memory[resultAddr] = startMSP;
        } 
    else 
        while ( flags->returnBackpatchHead != UNDEFINED ) {
            int returnAddr             = flags->returnBackpatchHead; 
            flags->returnBackpatchHead = memory[returnAddr];
            memory[returnAddr]         = startMSP;
        }
   
    /* emit cleanup code */
    codegenRoutineCleanup( decl->dobject->ovalue.scope, symbolLexicLevel( ), 
                           decl->dbound );

    /* backpage jump over routine declaration */
    memory[addr2] = startMSP;

    /* exit function scope */
    symbolExitScope( );

    /* deallocate flags */
    free( flags );
}

/****************************************************************/
/*                                                              */
/*  Routine (i.e. function or procedure) calls                  */
/*                                                              */
/****************************************************************/

/*
 * Emit code for a routine (i.e. function or procedure) call
 */
void codegenRoutineCall( char *name, ArgsP args, BOOLEAN isFunction )
{
   SymbTabEntryInfo symEntryInfo;
   int addr1;
   ArgsP curr;
   
   /* lookup routine's lexic level */
   symbolLookup( name, &symEntryInfo );

   if (isFunction == TRUE)
   {
       /* allocate space for return value */
       memory[startMSP++] = PUSH;
       memory[startMSP++] = UNDEFINED;
   }
   
   /* back up display */
   memory[startMSP++] = ADDR;
   memory[startMSP++] = symEntryInfo.lexic_level + 1;
   memory[startMSP++] = 0; 

    /* push return address (to be backpatched later) */
    memory[startMSP++] = PUSH;
    addr1              = startMSP;
    memory[startMSP++] = UNDEFINED;

    curr = args;
    while ( curr != NULL ) {
        codegenPushExpn( curr->avalue );    
        curr = curr->anext;
    }

    /* branch to routine code */
    memory[startMSP++] = ADDR;
    memory[startMSP++] = symEntryInfo.lexic_level;
    memory[startMSP++] = symEntryInfo.order_number;
    memory[startMSP++] = LOAD;
    memory[startMSP++] = BR;

    /* backpatch return address */
    memory[addr1]      = startMSP;

    /* restore display */
    memory[startMSP++] = SETD;
    memory[startMSP++] = symEntryInfo.lexic_level + 1;
}

/****************************************************************/
/*                                                              */
/*  Statements                                                  */
/*                                                              */
/****************************************************************/

/* 
 * Emit code for a list of statements 
 */
void codegenStmtList( ObjectP stmts, FlagsP flags ) 
{
    while ( stmts != NULL ) {
        codegenStmt( stmts->ovalue.stmt, flags ); 
        stmts = stmts->onext;
    }
}

/* 
 * Emit code for a single statement
 */
void codegenStmt( StmtP stmt, FlagsP flags ) 
{
    switch ( stmt->stype ) {
        case Sassign:
            codegenStmtAssign( stmt );
            break;

        case Sif:
            codegenStmtIf( stmt, flags ); 
            break;

        case Sifelse:
            codegenStmtIfElse( stmt, flags );
            break;

        case Swhile:
            codegenStmtWhile( stmt, flags, 0, NULL ); 
            break;

        case Sdo:
           codegenStmtDo( stmt, flags );
           break;

        case Sfor:
           codegenStmtFor( stmt, flags );
           break;

        case Sexit:
            codegenStmtExit( stmt, flags ); 
            break;

        case Sreturn: 
            codegenStmtReturn( stmt, flags );
            break;

        case Sresult:  
            codegenStmtResult( stmt, flags );
            break;

        case Sput:
            codegenStmtPut( stmt );
            break;

        case Sget:
            codegenStmtGet( stmt );
            break;

        case Sassert:
            codegenStmtAssert( stmt );
            break;

        case Scall:
            codegenRoutineCall( stmt->pname, stmt->sargs, FALSE );
            break;

        case Sscope:
            codegenStmtMajorScope( stmt, flags );
            break;

        default:
            assert( FALSE ); /* can't happen */
    }
}

/* 
 * Assignment statements 
 */
void codegenStmtAssign( StmtP stmt )
{ 
    SymbTabEntryInfo symEntryInfo; 

    if ( stmt->sexpn1->etype == Ebinary ) {
        symbolLookup( stmt->sexpn1->evalue.eop.eopleft->evalue.eident, 
	              &symEntryInfo );
        memory[startMSP++] = ADDR;
        memory[startMSP++] = symEntryInfo.lexic_level;

        /* if the index expn is a constant */
        if ( stmt->sexpn1->evalue.eop.eopright->etype == Econst ) {
            memory[startMSP++] = symEntryInfo.order_number
                                + stmt->sexpn1->evalue.eop.eopright->evalue.elitVal.lvalue.num;
        } else {
            memory[startMSP++] = symEntryInfo.order_number;
            codegenPushExpn( stmt->sexpn1->evalue.eop.eopright );
            memory[startMSP++] = ADD;
        }
        codegenPushExpn( stmt->sexpn2 );

    } else {
        symbolLookup( stmt->sexpn1->evalue.eident, &symEntryInfo );
        memory[startMSP++] = ADDR;
        memory[startMSP++] = symEntryInfo.lexic_level;
        memory[startMSP++] = symEntryInfo.order_number;
        codegenPushExpn( stmt->sexpn2 );
    }
    memory[startMSP++] = STORE;
}

/*
 * If statements  
 */
void codegenStmtIf( StmtP stmt, FlagsP flags ) 
{
    int addr1;
    ObjectP stmts;

    codegenPushExpn( stmt->sexpn1 );
    memory[startMSP++] = PUSH;
    addr1              = startMSP;
    memory[startMSP++] = UNDEFINED; /* addr of :exit */
    memory[startMSP++] = BF;

    stmts = stmt->sstmt1;
    while ( stmts != NULL ) {
        codegenStmt( stmts->ovalue.stmt, flags ); 
        stmts = stmts->onext;
    }
    memory[addr1] = startMSP; /* backpatch :exit */
}

/*
 * If...Else statements 
 */
void codegenStmtIfElse( StmtP stmt, FlagsP flags)
{
    int addr1, addr2;
    ObjectP stmts;

    codegenPushExpn( stmt->sexpn1 );
    memory[startMSP++] = PUSH;
    addr1              = startMSP;
    memory[startMSP++] = UNDEFINED; /* addr of :condf */
    memory[startMSP++] = BF;

    stmts = stmt->sstmt1;
    while ( stmts != NULL ) {
        codegenStmt( stmts->ovalue.stmt, flags ); 
        stmts = stmts->onext;
    }

    memory[startMSP++] = PUSH;
    addr2              = startMSP;
    memory[startMSP++] = UNDEFINED; /* addr of :exit */
    memory[startMSP++] = BR;

    memory[addr1]      = startMSP; /* backpatch :condf */

    stmts = stmt->sstmt2;
    while ( stmts != NULL ) {
        codegenStmt( stmts->ovalue.stmt, flags ); 
        stmts = stmts->onext;
    }

    memory[addr2] = startMSP; /* backpatch :exit */
}

/*
 * While loops 
 */
void codegenStmtWhile( StmtP stmt, FlagsP flags, int incramt, ExpnP incrvar)
{
    int addr1, addr2;
    FlagsP backupFlags;

    addr1               = startMSP; /* :evalexp */
    codegenPushExpn( stmt->sexpn1 ); 
    addr2               = startMSP + 1; /* addr of UNDEFINED */
    memory[startMSP++]  = PUSH;
    memory[startMSP++]  = UNDEFINED; /* addr of :exit */
    memory[startMSP++]  = BF;

    /* backup flag members we're about to change */
    backupFlags = codegenCopyFlags( flags );

    flags->exitBackpatchHead = UNDEFINED;
    flags->llEnclosingLoop   = symbolLexicLevel();

    /* process the list of contained statements */
    codegenStmtList( stmt->sstmt1, flags );

    /* if an increment is specified, add code */
    /* for it here.                           */
    if (incramt != 0)
    {
        SymbTabEntryInfo symEntryInfo;
        assert(incrvar != NULL);
        symbolLookup( incrvar->evalue.eident, &symEntryInfo );
        memory[startMSP++] = ADDR;
        memory[startMSP++] = symEntryInfo.lexic_level;
        memory[startMSP++] = symEntryInfo.order_number;
        codegenPushExpn( incrvar );
        memory[startMSP++] = PUSH;
        memory[startMSP++] = incramt;
        memory[startMSP++] = ADD;
        memory[startMSP++] = STORE;
    }

    /* jump to evaluate the expression */ 
    memory[startMSP++] = PUSH;
    memory[startMSP++] = addr1;  
    memory[startMSP++] = BR;

    /* backpatch :exit */
    memory[addr2]      = startMSP; 

    /* backpatch all exit statements in loop body */
    while ( flags->exitBackpatchHead != UNDEFINED ) {
        int exitAddr             = flags->exitBackpatchHead; 
        flags->exitBackpatchHead = memory[exitAddr];
        memory[exitAddr]         = startMSP;
    }

    /* restore flag members we changed */
    flags->exitBackpatchHead = backupFlags->exitBackpatchHead;
    flags->llEnclosingLoop   = backupFlags->llEnclosingLoop;
}

/*
 * Do loops 
 */
void codegenStmtDo( StmtP stmt, FlagsP flags )
{
    int addr1;   
    FlagsP backupFlags;
 
    addr1 = startMSP; /* :body */

    /* backup flag members we're about to change */
    backupFlags = codegenCopyFlags( flags );

    flags->exitBackpatchHead = UNDEFINED;
    flags->llEnclosingLoop   = symbolLexicLevel();

    /* process the list of contained statements */
    codegenStmtList( stmt->sstmt1, flags );

    codegenPushExpn( stmt->sexpn1 ); 
    memory[startMSP++] = PUSH;
    memory[startMSP++] = addr1;  /* addr of :body */
    memory[startMSP++] = BF;     

    /* backpatch all exit statements in loop body */
    while( flags->exitBackpatchHead != UNDEFINED ) {
        int exitAddr             = flags->exitBackpatchHead; 
        flags->exitBackpatchHead = memory[exitAddr];
        memory[exitAddr]         = startMSP;
    }

    /* restore flag members we changed */
    flags->exitBackpatchHead = backupFlags->exitBackpatchHead;
    flags->llEnclosingLoop   = backupFlags->llEnclosingLoop;
}

/*
 * For loops
 */
void codegenStmtFor( StmtP stmt, FlagsP flags )
{
    StmtObj assignstmt;
    StmtObj whilestmt;
    ExpnObj looptestexpn;

    /* first, generate the code for the assign */
    assignstmt.line  = stmt->line;
    assignstmt.stype = Sassign;
    assignstmt.sexpn1= stmt->sexpn2;
    assignstmt.sexpn2= stmt->sexpn3;
    codegenStmtAssign(&assignstmt);

    /* Translate this for loop into a while      */
    /* by adding in the boolean loop exit expn   */
    /* and an increment operation performed at   */
    /* the end of each operation.                */
    looptestexpn.etype = Ebinary;
    looptestexpn.evalue.eop.eoper  = stmt->incramt >= 0 ? Olesseq : Ogrtreq;
    looptestexpn.evalue.eop.eopleft= stmt->sexpn2;
    looptestexpn.evalue.eop.eopright=stmt->sexpn1;
    looptestexpn.line = stmt->line;
    whilestmt.line  = stmt->line;
    whilestmt.stype = Swhile;
    whilestmt.sexpn1= &looptestexpn;
    whilestmt.sstmt1= stmt->sstmt1;
    codegenStmtWhile( &whilestmt, flags, stmt->incramt, stmt->sexpn2 );
}

void codegenStmtExit( StmtP stmt, FlagsP flags )
{
    int ll; 

    /* emit code to properly back out of nested scopes */
    for (ll = symbolLexicLevel( ); ll > flags->llEnclosingLoop; ll-- ) 
        codegenMajorScopeCleanup( ll );       

    /* setup :exit branch */
    memory[startMSP++] = PUSH;
    if ( flags->exitBackpatchHead == UNDEFINED ) {
        flags->exitBackpatchHead = startMSP;
        memory[startMSP++]       = UNDEFINED; 
    } else {
        memory[startMSP]         = flags->exitBackpatchHead;
        flags->exitBackpatchHead = startMSP++;
    }
    memory[startMSP++] = BR;
}
 
void codegenStmtReturn( StmtP stmt, FlagsP flags )
{
    int ll;

    /* emit code to properly back out of nested scopes */
    for (ll = symbolLexicLevel(); ll > flags->llEnclosingRoutine; ll-- ) 
         codegenMajorScopeCleanup( ll );       

    /* clean up procedure scope and return control to the caller */
    memory[startMSP++] = PUSH;
    if ( flags->returnBackpatchHead == UNDEFINED ) {
        flags->returnBackpatchHead = startMSP;
        memory[startMSP++]         = UNDEFINED; 
    } else {
        memory[startMSP]           = flags->returnBackpatchHead;
        flags->returnBackpatchHead = startMSP++;
    }
    memory[startMSP++] = BR;  
}

void codegenStmtResult( StmtP stmt, FlagsP flags )
{
    int ll;

    /* store the result in its spot just before the beginning of
     * the function on the stack (specifically at display[llfn] - 3) */
    memory[startMSP++] = ADDR;
    memory[startMSP++] = flags->llEnclosingRoutine;
    memory[startMSP++] = 0;
    memory[startMSP++] = PUSH;
    memory[startMSP++] = 3;
    memory[startMSP++] = SUB;
    codegenPushExpn( stmt->sexpn1 ); 
    memory[startMSP++] = STORE;

    /* emit code to properly back out of nested scopes */
    for (ll = symbolLexicLevel(); 
         ll > flags->llEnclosingRoutine; ll-- ) 
         codegenMajorScopeCleanup( ll );       

    /* clean up function scope and return control to the caller */
    memory[startMSP++] = PUSH;
    if ( flags->resultBackpatchHead == UNDEFINED ) {
        flags->resultBackpatchHead = startMSP;
        memory[startMSP++] = UNDEFINED; 
    } else {
        memory[startMSP] = flags->resultBackpatchHead;
        flags->resultBackpatchHead = startMSP++;
    }
    memory[startMSP++] = BR; 
}

/*
 * Major scopes 
 */
void codegenStmtMajorScope( StmtP stmt, FlagsP flags )
{
    /* enter major scope */
    symbolEnterScope( );

    /* setup major scope */ 
    codegenMajorScopeSetup( stmt->sscope );

    /* emit code for major scope */
    codegenScope( stmt->sscope, flags );

    /* cleanup major scope */
    codegenMajorScopeCleanup( symbolLexicLevel( ) );

    /* exit major scope */
    symbolExitScope( );
}

/*
 * Get statements (input)
 */
void codegenStmtGet( StmtP stmt )
{
    ArgsP args = stmt->sargs;
    ExpnP expn;
    SymbTabEntryInfo entry;

    while ( args != NULL )  {
        expn               = args->avalue;
        assert( expn->etype == Evar );
        symbolLookup( expn->evalue.eident, &entry );

        memory[startMSP++] = ADDR;
        memory[startMSP++] = entry.lexic_level;
        memory[startMSP++] = entry.order_number;
        memory[startMSP++] = READI;
        memory[startMSP++] = STORE;

        args               = args->anext;
    }
}

/*
 * Put statements (output) 
 */
void codegenStmtPut( StmtP stmt )
{
    ArgsP args = stmt->sargs;
    ExpnP expn;
    char *c;

    while ( args != NULL )  {
        expn = args->avalue;
        if (expn->etype == Eskip) {
            memory[startMSP++] =  PUSH; 
            memory[startMSP++] =  ( int )'\n'; 
            memory[startMSP++] =  PRINTC;

        } else if ( expn->etype == Econst) {

            if ( expn->evalue.elitVal.ltype == Dtext ) {
                c = expn->evalue.elitVal.lvalue.str + 1;
                while ( *(c + 1) != '\0') {
                    memory[startMSP++] =  PUSH; 
                    memory[startMSP++] =  ( int )*c;
                    memory[startMSP++] =  PRINTC;
                    c++;
                }    

            } else { 
                codegenPushExpn( expn );
                memory[startMSP++] =  PRINTI;
            }

        } else {
            /* complex expression that is an integer */
            codegenPushExpn( expn );
            memory[startMSP++] =  PRINTI;
        }

        args = args->anext;
    }
}

/*
 * Assert statements
 */
void codegenStmtAssert( StmtP stmt )
{
    /* first, branch around cleanup code */
    int haltaddr   = startMSP + 3, 
        branchaddr = startMSP + 1;
    memory[startMSP++] = PUSH;
    memory[startMSP++] = UNDEFINED; /* will be patched later */
    memory[startMSP++] = BR;
    /* cleanup and halt */
    memory[startMSP++] = HALT;
    /* patch undefined address */
    memory[branchaddr]   = startMSP;
    /* emit code for testing assertion expn */  
    codegenPushExpn( stmt->sexpn1 );
    memory[startMSP++] = PUSH;
    memory[startMSP++] = haltaddr;
    memory[startMSP++] = BF;
}


/****************************************************************/
/*                                                              */
/*  Expressions                                                 */
/*                                                              */
/****************************************************************/

/* 
 * Emit code for pushing an expression onto the stack
 */
void codegenPushExpn( ExpnP expn )
{
    switch( expn->etype ) {
        case Econst:
            codegenPushExpnConst( expn );
            break;

        case Evar:
            codegenPushExpnVar( expn );
            break;    

        case Eunary:
            codegenPushExpnUnary( expn );
            break;

        case Ebinary:
            codegenPushExpnBinary( expn );
            break;

        case Econd:
            codegenPushExpnCond( expn );
            break;

        case Efcall:
            codegenRoutineCall( expn->evalue.efn.efname, 
                                expn->evalue.efn.efargs, TRUE ); 
            break;
        default:
            assert( FALSE ); /* can't happen */
    }
}

/* 
 * Constants 
 */
void codegenPushExpnConst( ExpnP expn )
{
    memory[startMSP++] =  PUSH; 
    memory[startMSP++] =  expn->evalue.elitVal.lvalue.num;
}

/* 
 * Variables 
 */
void codegenPushExpnVar( ExpnP expn )
{
    SymbTabEntryInfo symEntryInfo; 
    symbolLookup( expn->evalue.eident, &symEntryInfo );
    memory[startMSP++] = ADDR;
    memory[startMSP++] = symEntryInfo.lexic_level;
    memory[startMSP++] = symEntryInfo.order_number;
    memory[startMSP++] = LOAD;
}

/* 
 * Unary expressions: not (!) and unary minus (-) 
 */
void codegenPushExpnUnary( ExpnP expn )
{
    /* not */
    if ( expn->evalue.eop.eoper == Onot ) {
        codegenPushExpn( expn->evalue.eop.eopright );
        memory[startMSP++] =  NEG; 
        memory[startMSP++] =  PUSH; 
        memory[startMSP++] =  1; 
        memory[startMSP++] =  ADD; 

    /* unary minus */
    } else if ( expn->evalue.eop.eoper == Ouminus ) {
        codegenPushExpn( expn->evalue.eop.eopright );
        memory[startMSP++] =  NEG; 
    }
}

/* 
 * Binary expressions 
 */
void codegenPushExpnBinary( ExpnP expn )
{
    switch ( expn->evalue.eop.eoper ) {
        case Oadd:
            codegenPushExpnBinaryBuiltIn( expn, ADD );
            break;

        case Osub: 
            codegenPushExpnBinaryBuiltIn( expn, SUB ); 
            break;

        case Omul:
            codegenPushExpnBinaryBuiltIn( expn, MUL ); 
            break;

        case Odiv:
            codegenPushExpnBinaryBuiltIn( expn, DIV ); 
            break;

        case Oeq:
            codegenPushExpnBinaryBuiltIn( expn, EQ ); 
            break;

        case Oless: 
            codegenPushExpnBinaryBuiltIn( expn, LT );  
            break;

        case Olesseq:
            codegenPushExpnBinaryComparison( expn->evalue.eop.eopleft, 
                                             expn->evalue.eop.eopright, 1 );
            break;

        case Ogrtr: 
            codegenPushExpnBinaryComparison( expn->evalue.eop.eopright, 
                                             expn->evalue.eop.eopleft, 0 ); 
            break;

        case Ogrtreq:
            codegenPushExpnBinaryComparison( expn->evalue.eop.eopright, 
                                             expn->evalue.eop.eopleft, 1 ); 
            break;

        case Oneq:
            codegenPushExpnBinaryNEq( expn ); 
            break;

        case Osubs: 
            codegenPushExpnBinarySubs( expn );
            break;

        case Oand: 
            codegenPushExpnBinaryAnd( expn );
            break;

        case Oor: 
            codegenPushExpnBinaryOr( expn );
            break;

        default:
            assert( FALSE ); /* can't happen */
    }
}

/*
 * Built in binary operators: add (+), subtract (-), multiply (*), 
 * divide (/), equality comparison (=) and less than (<) 
 */
void codegenPushExpnBinaryBuiltIn( ExpnP expn, int operator )
{
    codegenPushExpn( expn->evalue.eop.eopleft );
    codegenPushExpn( expn->evalue.eop.eopright );
    memory[startMSP++] = operator; 
}

/* 
 * Less than or equal to (<=), greater than (>) and greater than or 
 * equal to (>=); note that less than (<) is built in so it's handled by
 * codegenPushExpnBuiltIn( )
 */
void codegenPushExpnBinaryComparison( ExpnP lop, ExpnP rop, int threshold )
{
    codegenPushExpn( lop );
    codegenPushExpn( rop );
    memory[startMSP++] = SUB; 
    memory[startMSP++] = PUSH; 
    memory[startMSP++] = threshold; 
    memory[startMSP++] = LT; 
}

/*
 * Not equal to (#)
 */ 
void codegenPushExpnBinaryNEq( ExpnP expn ) 
{
    codegenPushExpn( expn->evalue.eop.eopleft );
    codegenPushExpn( expn->evalue.eop.eopright );
    memory[startMSP++] = EQ; 
    memory[startMSP++] = NEG; 
    memory[startMSP++] = PUSH; 
    memory[startMSP++] = 1; 
    memory[startMSP++] = ADD; 
}

/*
 * Array subscripting 
 */ 
void codegenPushExpnBinarySubs( ExpnP expn )
{
    SymbTabEntryInfo symEntryInfo;
 
    symbolLookup( expn->evalue.eop.eopleft->evalue.eident, 
                  &symEntryInfo );
    
    memory[startMSP++] = ADDR;
    memory[startMSP++] = symEntryInfo.lexic_level;

    /* if the index expn is a constant */
    if ( expn->evalue.eop.eopright->etype == Econst ) {
        memory[startMSP++] = symEntryInfo.order_number
                           + expn->evalue.eop.eopright->evalue.elitVal.lvalue.num;
    } else {
        /* otherwise */ 
        memory[startMSP++] = symEntryInfo.order_number;
        codegenPushExpn( expn->evalue.eop.eopright );
        memory[startMSP++] = ADD;
   } 
    memory[startMSP++] = LOAD;
}

/*
 * And (&) -- our implementation is short-circuiting
 */ 
void codegenPushExpnBinaryAnd( ExpnP expn )
{
    int addr1, addr2;
    
    codegenPushExpn( expn->evalue.eop.eopleft );
    memory[startMSP++] = PUSH; 
    addr1              = startMSP; /* record startMSP here */
    memory[startMSP++] = UNDEFINED; /* replaced with exp1f: addr */
    memory[startMSP++] =  BF; 
    codegenPushExpn( expn->evalue.eop.eopright );
    memory[startMSP++] = PUSH; 
    addr2              = startMSP;
    memory[startMSP++] = UNDEFINED; /* replaced with exit: addr */
    memory[startMSP++] =  BR; 
                    
    memory[addr1]      = startMSP; /* back patch to exp1f */

    memory[startMSP++] = PUSH; 
    memory[startMSP++] = 0; 

    memory[addr2]      = startMSP; /* back patch to exp1f */
}

/* 
 * Or (|) -- our implementation is short-circuiting
 */
void codegenPushExpnBinaryOr( ExpnP expn )
{
    int addr1, addr2;    

    codegenPushExpn( expn->evalue.eop.eopleft );
    memory[startMSP++] =  NEG; 
    memory[startMSP++] =  PUSH; 
    memory[startMSP++] =  1; 
    memory[startMSP++] =  ADD; 
    memory[startMSP++] = PUSH; 
    addr1              = startMSP; /* record startMSP here */
    memory[startMSP++] = UNDEFINED; /* replaced with exp1f: addr */
    memory[startMSP++] =  BF; 
    codegenPushExpn( expn->evalue.eop.eopright );
    memory[startMSP++] = PUSH; 
    addr2              = startMSP;
    memory[startMSP++] = UNDEFINED; /* replaced with exit: addr */
    memory[startMSP++] =  BR; 
                    
    memory[addr1]      = startMSP; /* back patch to exp1f */

    memory[startMSP++] = PUSH; 
    memory[startMSP++] = 1; 

    memory[addr2]      = startMSP; /* back patch to exp1f */
}

/* 
 * Ternary operator (?) 
 */
void codegenPushExpnCond( ExpnP expn )
{
    int addr1, addr2; 

    codegenPushExpn( expn->evalue.econx.control );
    memory[startMSP++] = PUSH; 
    addr1              = startMSP; /* record startMSP here */
    memory[startMSP++] = UNDEFINED; /* replaced with exp1f: addr */
    memory[startMSP++] =  BF; 
    codegenPushExpn( expn->evalue.econx.contrue );
    memory[startMSP++] = PUSH; 
    addr2              = startMSP;
    memory[startMSP++] = UNDEFINED; /* replaced with exit: addr */
    memory[startMSP++] =  BR; 

    memory[addr1]      = startMSP; /* back patch to exp1f */
    codegenPushExpn( expn->evalue.econx.confalse );
    memory[addr2]      = startMSP; /* back patch to exp1f */
}

/****************************************************************/
/*                                                              */
/*  External function definitions                               */
/*                                                              */
/****************************************************************/

/*
 * Initialize code generation pass
 */
void codegenInitialize( ){
    int i;

    machineInitialize( );
    startPC = startMSP = startMLP = 0;
    /* initialize display: emit code that sets the display at each 
     * lexic level to the start of the stack */
    for ( i = 0; i < DISPLAYSIZE; i++ ) {
        memory[startMSP++] = PUSH;
        memory[startMSP++] = 0; /* replace with addr of start of stack */
        memory[startMSP++] = SETD;
        memory[startMSP++] = i;
    }

    return ;
}

/*
 * Finalise code generation pass (prepare program for execution)
 */
void codegenFinalize( ) {
    int i; 

    memory[startMSP++] = HALT ; 

    /* backpatch display initialisation */
    for ( i = 0; i < DISPLAYSIZE; i++ ) 
        memory[i*4 + 1] = startMSP;

    startPC  = 0 ;
    startMLP = MEMORYSIZE -1 ;

    return ;
}

/*
 * Begin code generation pass
 */
void codegenBegin( ASTtype tree ) 
{
    ObjectP trace = ( ObjectP )tree; 
    FlagsP flags  = codegenInitFlags( );

    /* enter program scope */
    symbolEnterScope( );

    /* setup program scope */
    codegenMajorScopeSetup( trace->ovalue.scope );

    /* emit code for program scope */
    codegenScope( trace->ovalue.scope, flags );

    /* cleanup program scope */
    codegenMajorScopeCleanup( symbolLexicLevel( ) );

    /* exit program scope */
    symbolExitScope( );
    
    /* deallocate flags */
    free( flags );
}

