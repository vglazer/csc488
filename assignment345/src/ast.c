
/**********************************************************/
/*                                                        */
/*    ast.c                                               */
/*    Abstract Syntax Tree module for the CSC488S Project */
/*                                                        */
/**********************************************************/


/********************************************************/
/*   Include common definitions                         */
/********************************************************/
#include "common.h"

/********************************************************/
/*   External Definitions                               */
/********************************************************/

/* removed: feb 13 newsgroup instructions
*/
   #include "globalvars.h" 

/********************************************************/
/*   Include interface to this module                   */
/********************************************************/
#include "ast.h"

/********************************************************/
/*   Definitions for the Abstract Syntax Tree           */
/********************************************************/
#include "astDef.h"

/********************************************************/
/*   Internal data for this module                      */
/********************************************************/

/*****************************************/
/* insert your data declarations HERE    */
/*****************************************/
void reverse_obj_list (ObjectP * list);
void reverse_params_list (FparmP * list); 
void reverse_arg_list (ArgsP * list); 
       
void finalizeObject( ObjectP obj );
void finalizeDecl( DeclP decl );
void finalizeStmt( StmtP stmt );
void finalizeExpn( ExpnP expn );
void finalizeScope( ScopeP scope );
void finalizeArg( ArgsP arg );
void finalizeFParm( FparmP param );
void finalizeLit( LitObjP lit );

/********************************************************/
/*                                                      */
/*   Internal functions defined in this file            */
/*                                                      */
/********************************************************/

/********************************************************/
/*  write log messages                                  */
/********************************************************/
void LOG( char * msg );

/********************************************************/
/*  return the number of parameters                     */
/********************************************************/
int numParms( FparmP parms )
{
    int count = 0;

    while ( parms != NULL ) {
        count++;
        parms = parms->fnext;
    }
    return count;
}

/********************************************************/
/*  reverse the order of a list of objects              */
/********************************************************/
void reverse_obj_list (ObjectP * list) 
{
    ObjectP newhead;
    ObjectP ocur;
    ObjectP onext;
    ocur = *list;
    newhead = NULL;
    while (ocur != NULL) { 
        onext = ocur->onext;
        ocur->onext = newhead;
        newhead = ocur;
        ocur = onext; 
    }
    *list = newhead;
}

/********************************************************/
/*  reverse the order of a list of args                 */
/********************************************************/
void reverse_arg_list (ArgsP * list) 
{
    ArgsP newhead;
    ArgsP acur;
    ArgsP anext;
    acur = *list;
    newhead = NULL;
    while (acur != NULL) { 
        anext = acur->anext;
        acur->anext = newhead;
        newhead = acur;
        acur = anext; 
    }
    *list = newhead;
}

/********************************************************/
/*  reverse the order of a list of parameters           */
/********************************************************/
void reverse_params_list (FparmP * list) 
{
    FparmP newhead;
    FparmP fcur;
    FparmP fnext;
    fcur = *list;
    newhead = NULL;
    while (fcur != NULL) { 
        fnext = fcur->fnext;
        fcur->fnext = newhead;
        newhead = fcur;
        fcur = fnext; 
    }
    *list = newhead;
}

/********************************************************/
/*   Function definitions                               */
/********************************************************/

ObjectP mkProgram(ScopeP scope) 
{
    ObjectP obj = calloc(1, sizeof(Object));

    obj->otype = Oscope;
    obj->ovalue.scope = scope;
    obj->onext = NULL;  

    return obj; 
}


ScopeP mkScope(ObjectP decls, ObjectP stmts, int linenum) 
{
    ObjectP ocur; /* variable used for iterating through lists*/
    ScopeP scope = calloc(1, sizeof(ScopeObj));  /* scope we're creating */
    DclType tmpDclType;

    scope->line = linenum;
    scope->scparent = NULL;
    
    reverse_obj_list(&decls);
    reverse_obj_list(&stmts);

    if (decls != NULL) {
        scope->scbody = decls;    
        /* iterate through the decls list, setting the scope *
         * of all the decls to this scope                    */
        ocur = scope->scbody;
        while (ocur->onext != NULL){
            /* set parent pointers */
            tmpDclType = ocur->ovalue.decl->dctype;
            if ( tmpDclType == DCfunc || tmpDclType == DCproc) {
                ocur->ovalue.decl->dobject->ovalue.scope->scparent = scope;
            }
            ocur->ovalue.decl->dscope = scope;
            ocur = ocur->onext;
        }
        /* now ocur points to the last object in the decls list.* 
         * attach the stmts list to the end of the decls list   */
        tmpDclType = ocur->ovalue.decl->dctype;
        if (tmpDclType == DCfunc || tmpDclType == DCproc) {
            ocur->ovalue.decl->dobject->ovalue.scope->scparent = scope;
        }
        ocur->ovalue.decl->dscope = scope;
        ocur->onext = stmts;
    } else {
        scope->scbody = stmts;    
    }

    ocur = stmts;

    /* iterate through the stmts list, checking for statmenets *
     * of type scope.  For each such object, set its parent    *
     * scope to this scope */ 
    while (ocur != NULL){ 
        if (ocur->ovalue.stmt->stype == Sscope)
            ocur->ovalue.stmt->sscope->scparent = scope;
       ocur = ocur->onext;
    } 
    return scope;
}

ObjectP mkStmt_list(ObjectP stmt_list, StmtP stmt)
{
    ObjectP obj = calloc(1, sizeof(Object));

    obj->otype = Ostmt;
    obj->ovalue.stmt = stmt;
    /* The list of statements are linked in the reverse order to which they
     * occured. */
    obj->onext = stmt_list;

    return obj;
} 

ObjectP mkDecl_list(ObjectP decl_list, DeclP decl)
{
    ObjectP obj = calloc(1, sizeof(Object));

    obj->otype = Odecl;
    obj->ovalue.decl = decl;
    /* The list of declarations are linked in the reverse order to which they
     * occured. */
    obj->onext = decl_list;

    return obj;
}

StmtP mkStmt_assign(ExpnP var, ExpnP expr, int linenum)
{
    StmtP stmt = calloc(1, sizeof(StmtObj));
    
    stmt->line = linenum;

    stmt->stype = Sassign;
    stmt->sexpn1 = var;
    stmt->sexpn2 = expr;

    return stmt;
}

StmtP mkStmt_while(ExpnP expr, ObjectP stmts, int linenum)
{
    StmtP stmt = calloc(1, sizeof(StmtObj));
    stmt->line = linenum; 

    reverse_obj_list(&stmts);

    stmt->stype = Swhile;
    stmt->sexpn1 = expr;
    stmt->sstmt1 = stmts; 
    return stmt;
}

StmtP mkStmt_do(ObjectP stmts, ExpnP expr, int linenum)
{
    StmtP stmt = calloc(1, sizeof(StmtObj));
    stmt->line = linenum;

    reverse_obj_list(&stmts);

    stmt->stype = Sdo;
    stmt->sexpn1 = expr;
    stmt->sstmt1 = stmts; 
    return stmt;
}

StmtP mkStmt_for( ExpnP var, ExpnP initexpr, ExpnP loopexpr, int incramt, 
                  ObjectP stmts, int linenum)
{
    StmtP stmt = calloc(1, sizeof(StmtObj));
    reverse_obj_list(&stmts);
 
    stmt->line = linenum;
    stmt->stype = Sfor;
    stmt->incramt = incramt;
    stmt->sexpn1 = loopexpr;
    stmt->sexpn2 = var;
    stmt->sexpn3 = initexpr;
    stmt->sstmt1 = stmts;

    return stmt; 
}

StmtP mkStmt_exit(int linenum)
{
    StmtP stmt = calloc(1, sizeof(StmtObj));

    stmt->line = linenum;
    stmt->stype = Sexit;
    
    return stmt;
}

StmtP mkStmt_return(int linenum)
{
    StmtP stmt = calloc(1, sizeof(StmtObj));
    stmt->line = linenum;

    stmt->stype = Sreturn;
    
    return stmt;
}

StmtP mkStmt_result(ExpnP expr, int linenum)
{
    StmtP stmt = calloc(1, sizeof(StmtObj));
    stmt->line = linenum;

    stmt->stype = Sresult;
    stmt->sexpn1 = expr;
    
    return stmt;
}

StmtP mkStmt_put(ArgsP args, int linenum)
{
    StmtP stmt = calloc(1, sizeof(StmtObj));
    stmt->line = linenum;

    reverse_arg_list(&args);

    stmt->stype = Sput;
    stmt->sargs = args;
    
    return stmt;
}

StmtP mkStmt_get(ArgsP args, int linenum)
{
    StmtP stmt = calloc(1, sizeof(StmtObj));
    stmt->line = linenum;

    reverse_arg_list(&args);

    stmt->stype = Sget;
    stmt->sargs = args;

    return stmt;
}

StmtP mkStmt_assert( ExpnP expn, int linenum )
{
    StmtP stmt = calloc(1, sizeof(StmtObj));
    stmt->line = linenum;

    stmt->stype = Sassert;
    stmt->sexpn1 = expn;
    
    return stmt;
}

StmtP mkStmt_scope(ScopeP scope, int linenum)
{
    StmtP stmt = calloc(1, sizeof(StmtObj));
    stmt->line = linenum;

    stmt->stype = Sscope;
    stmt->sscope = scope;
    
    return stmt;
}


StmtP mkStmt_proc(tokentype ident, ArgsP args, int linenum)
{
    StmtP stmt = calloc(1, sizeof(StmtObj));
    stmt->line = linenum;

    reverse_arg_list(&args);

    stmt->stype = Scall;
    stmt->sargs = args;
    stmt->pname = ident.str;
    return stmt;
}

StmtP mkStmt_if(ExpnP expr, ObjectP then_stmts, ObjectP else_stmts, int
        linenum)
{
    StmtP stmt = calloc(1, sizeof(StmtObj));
    stmt->line = linenum;

    reverse_obj_list(&then_stmts);
    reverse_obj_list(&else_stmts);

    if (else_stmts) {
        stmt->stype = Sifelse;
    } else {
        stmt->stype = Sif;
    }

    stmt->sexpn1 = expr;
    stmt->sstmt1 = then_stmts;
    stmt->sstmt2 = else_stmts;

    return stmt;
}


DeclP mkDecl_var_type(DataType dataType, int linenum)
{
    DeclP decl = calloc(1, sizeof(DeclObj));
    
    decl->line = linenum;
    decl->dctype = DCnone;
    decl->dtype = dataType;

    return decl;
}

DeclP mkDecl_var_ident(DeclP decl, tokentype ident)
{
    decl->dname = ident.str;
    
    return decl;
}


DeclP mkDecl_arr(DeclP decl, int size)
{
    decl->dctype = DCarray;
    decl->dbound = size;
    
    return decl; 
}

/* copy body to decl */
DeclP mkDecl_func(DeclP decl, DeclP body)
{
    if ( body->dobject->ovalue.scope == NULL ) {
        decl->dctype = DCforward_func;
    } else {
        decl->dctype = DCfunc;
    }
    decl->dfparm    = body->dfparm; 
    decl->dobject   = body->dobject;  
    decl->dbound    = body->dbound;
    free(body);
    return decl;
} 

DeclP mkDecl_scalar(DeclP decl, ExpnP initexpn)
{
    decl->dctype = DCscalar;
    decl->dbound = 1;  /* size of scalar */
    decl->dinitexpn = initexpn;
    return decl;
}

DeclP mkDecl_proc(tokentype ident, DeclP body)
{
    if ( body->dobject->ovalue.scope == NULL ) {
        body->dctype = DCforward_proc;
    } else {
        body->dctype = DCproc;
    }
    body->dtype = Dvoid;
    body->dname = ident.str;
    
    return body;
}


DeclP mkDecl_body(FparmP params, ScopeP scope, int linenum)
{
    DeclP decl = calloc(1, sizeof(DeclObj));
    ObjectP obj = calloc(1, sizeof(Object));

    decl->line = linenum; 

    reverse_params_list(&params);

    decl->dctype = DCnone;
    decl->dtype = Dnone;
    decl->dfparm = params;
    decl->dbound = numParms( params );

    obj->otype = Oscope;
    obj->ovalue.scope = scope;
    obj->onext = NULL;
    decl->dobject = obj;
    
    return decl;
}



FparmP mkFparm_list(FparmP parms, DeclP decl)
{
    FparmP newParm = calloc(1, sizeof(FParms));

    newParm->fname = decl->dname;
    newParm->ftype = decl->dtype;
    newParm->fnext = parms; /* could be NULL */

    free(decl);
    return newParm;
}

ArgsP mkArg_list_args(ArgsP args, ArgsP arg)
{
    arg->anext = args;

    return arg;
}

ArgsP mkArg_list_expn(ArgsP arg, ExpnP expr)
{
    ArgsP newArg = calloc(1, sizeof(Args));

    newArg->avalue = expr;
    newArg->anext = arg; /* could be NULL */
    newArg->line = expr->line;

    return newArg;
}

ArgsP mkArg_expn(ExpnP expr)
{
    ArgsP arg = calloc(1, sizeof(Args));

    arg->avalue = expr;
    arg->anext = NULL; 
    arg->line = expr->line;
    return arg;
}

ArgsP mkArg_str(tokentype text, int linenum)
{
    LitObjP lit = calloc(1, sizeof(LitObj));
    ArgsP arg = calloc(1, sizeof(Args));
    ExpnP expn; 
    
    lit->ltype = Dtext;
    lit->lvalue.str = text.str ;

    expn = mkExpn_const(lit, linenum);

    arg->avalue = expn;
    arg->anext = NULL;
    arg->line = linenum;

    return arg;
}

ArgsP mkArg_skip(int linenum)
{
    ExpnP expn = calloc(1, sizeof(ExpnObj));
    ArgsP arg = calloc(1, sizeof(Args));

    expn->line = linenum;
    arg->line = linenum;

    /* Make a  new expression of type Eskip, evalue doesn't apply */
    expn->etype = Eskip;

    arg->avalue = expn;
    arg->anext = NULL;

    return arg;
}

ExpnP mkExpn_oper(ExpnP left, OperType operType, ExpnP right, int linenum)
{
    ExpnP newExpn = calloc(1, sizeof(ExpnObj));

    if (left == NULL) {
        newExpn->etype = Eunary;
    } else {
        newExpn->etype = Ebinary;
    } 
    newExpn->evalue.eop.eoper = operType;
    newExpn->evalue.eop.eopleft = left;
    newExpn->evalue.eop.eopright = right;
    newExpn->line = linenum;

    return newExpn;
}

ExpnP mkExpn_cond(ExpnP cond, ExpnP mid, ExpnP right, int linenum)
{
    ExpnP newExpn = calloc(1, sizeof(ExpnObj));

    newExpn->etype = Econd; 
    newExpn->evalue.econx.control = cond;
    newExpn->evalue.econx.contrue = mid;
    newExpn->evalue.econx.confalse= right;
    newExpn->line = linenum;

    return newExpn;
}

ExpnP mkExpn_func(tokentype ident, ArgsP args, int linenum)
{
    ExpnP expn = calloc(1, sizeof(ExpnObj));

    reverse_arg_list(&args);

    expn->etype = Efcall;
    expn->evalue.efn.efname = ident.str;
    expn->evalue.efn.efargs = args;
    expn->line = linenum;

    return expn;
}

ExpnP mkExpn_const(LitObjP lit, int linenum)
{
    ExpnP expn = calloc(1, sizeof(ExpnObj));

    expn->etype = Econst;
    expn->evalue.elitVal = *lit;
    expn->line = linenum;

    free(lit);
    return expn;
}

ExpnP mkExpn_var(tokentype ident, int linenum)
{
    ExpnP expn = calloc(1, sizeof(ExpnObj));

    expn->etype = Evar;
    expn->evalue.eident = ident.str;
    expn->line = linenum;
    
    return expn;
}

/* left operand is the ident
 *  right operand is the index expression 
 */
ExpnP mkExpn_var_arr(tokentype ident, ExpnP expr, int linenum)
{
    ExpnP newExpn = calloc(1, sizeof(ExpnObj));
    ExpnP identExpn = mkExpn_var(ident, linenum);

    newExpn->etype = Ebinary;
    newExpn->evalue.eop.eoper    = Osubs;
    newExpn->evalue.eop.eopleft  = identExpn;
    newExpn->evalue.eop.eopright = expr;
    newExpn->line = linenum;
    return newExpn;
}

LitObjP mkLit(DataType dataType, int constant)
{
    LitObjP lit = calloc(1, sizeof(LitObj));

    lit->ltype = dataType;
    lit->lvalue.num = constant;

    return lit;
}

LitObjP mkLit_bool(DataType dataType, int boolValue)
{
    return mkLit(dataType, boolValue);      
}

/**************************/
/*  initialization        */
/**************************/

void astInitialize(ASTtype *  newTree ) {

  *newTree = NULL ;
  
  /**********************************************/
  /*  More AST initialization goes HERE         */
  /**********************************************/
  
}

/**************************/
/*  finalization          */
/**************************/

void astFinalize(ASTtype *  deadTree ) {

    /**********************************************/
    /*  Any finalization of the AST goes here      */
    /**********************************************/  
    finalizeObject( (ObjectP)deadTree );
}


void finalizeObject( ObjectP obj ) 
{
    if ( !obj )
        return; 

    LOG("finalizing an object");

    finalizeObject( obj->onext );

    switch ( obj->otype ) {

        case Odecl:     finalizeDecl( obj->ovalue.decl );       
                        break;

        case Ostmt:     finalizeStmt( obj->ovalue.stmt );
                        break;

        case Oexpn:     finalizeExpn( obj->ovalue.expn );
                        break;

        case Oscope:    finalizeScope( obj->ovalue.scope );
                        break ;

        default:
                        assert( FALSE );        /* unknown type */

    }

    free( obj );
}

void finalizeDecl( DeclP decl ) 
{
    if ( !decl ) 
        return;

    LOG("finalizing a declaration");

    free( decl->dname );
    finalizeFParm( decl->dfparm );
    finalizeObject( decl->dobject ); 

    free( decl );
}

void finalizeStmt( StmtP stmt )
{
    if ( !stmt ) 
        return;

    LOG("finalizing a statement");

    finalizeExpn( stmt->sexpn1 );
    finalizeExpn( stmt->sexpn2 );
    finalizeObject( stmt->sstmt1 );
    finalizeObject( stmt->sstmt2 );
    finalizeArg( stmt->sargs );
    finalizeScope( stmt->sscope );
    free( stmt->pname );

    free( stmt );
}

void finalizeExpn( ExpnP expn )
{
    if ( !expn )
        return;

    LOG("finalizing an expression");

    switch( expn->etype ){

        case Enone:  return;    
                     break ;

        case Eunary: /* fall through */ 
        case Ebinary:  
                     finalizeExpn( expn->evalue.eop.eopleft );
                     finalizeExpn( expn->evalue.eop.eopright );
                     break ;

        case Efcall: free( expn->evalue.efn.efname );
                     finalizeArg( expn->evalue.efn.efargs );
                     break ;

        case Econst: if (expn->evalue.elitVal.ltype == Dtext )
                         free( expn->evalue.elitVal.lvalue.str );
                     break ;

        case Evar:   free( expn->evalue.eident );
                     break ;

        case Econd:  finalizeExpn( expn->evalue.econx.control );
                     finalizeExpn( expn->evalue.econx.contrue );
                     finalizeExpn( expn->evalue.econx.confalse );
                     break ;

        case Eskip:  /* do nothing */ 
                     break;

        default:
                     assert( FALSE );
    }
    
    free( expn );
}


void finalizeScope( ScopeP scope )
{
    if ( !scope ) 
        return;

    LOG("finalizing a scope");

    finalizeObject( scope->scbody );

    free( scope );
}


void finalizeArg( ArgsP arg )
{
    if ( !arg )
        return;

    LOG("finalizing an argument");

    finalizeExpn( arg->avalue );
    finalizeArg( arg->anext );

    free( arg );
}

void finalizeFParm( FparmP param )
{
    if ( !param )
        return;

    LOG("finalizing a paramter");

    free( param->fname );
    finalizeFParm( param->fnext );

    free( param );
}

void finalizeLit( LitObjP lit )
{
    if ( !lit )
        return;

    LOG("finalizing a literal");

    switch (lit->ltype )
    {
        case Dint: /* fall through */
        case Dbool: /* fall through */
        case Dvoid: /* fall through */
        case Dnone:
            /* do nothing */
            break;
        case Dtext:
            free( lit->lvalue.str );
            break;
        default:
            assert( FALSE );
    }
    free( lit );

}

/****
* Logging 
*
*****/

void LOG( char * msg )
{
    if (traceAST == TRUE )
        fprintf(traceFile, "AST: %s\n", msg);        
}

/*********************************/
/*        end of file ast.c      */
/*********************************/

