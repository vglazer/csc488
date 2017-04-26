#ifndef _ASTDEF_H_
#define _ASTDEF_H_

/********************************************************/
/*                                                      */
/*      astDef.h                                        */
/*      Definition of the real data structures          */
/*      for the Abstract Syntax Tree                    */
/*                                                      */
/********************************************************/

/********************************************************/
/*                                                      */
/*  Include interface to ast module                     */
/*                                                      */
/********************************************************/

/* removed: feb 13 newgroup instructions
   #include  "ast.h" */

/********************************************************/
/*   Define forward pointers to various data structures */
/********************************************************/ 
typedef struct Object_tag * ObjectP;    /* Objects      */
typedef struct Args_tag   * ArgsP ;     /* Arguments    */
typedef struct Expn_tag   * ExpnP ;     /* Expressions  */
typedef struct Decl_tag   * DeclP ;     /* Declarations */
typedef struct FParm_tag  * FparmP ;    /* Formal Parm  */
typedef struct Stmt_tag   * StmtP ;     /* Statements   */
typedef struct Scope_tag  * ScopeP ;    /* Scopes       */
typedef struct LitObj_tag * LitObjP;    /* Literal      */

/********************************************************/
/*  ObjectP  is the REAL  ASTtype                       */
/********************************************************/

/********************************************************/
/* Data structure used to store literal values          */
/********************************************************/ 

typedef struct LitObj_tag {
  DataType  ltype;  /* type of literal stored           */

  union litValue_tag {
    char* str;     /* value for string constant         */
    int   num;     /* value for numeric constant,       */
    /* or 0 (false) or 1 (true) for boolean constants   */
  } lvalue;     /* value of the literal                 */
  /* literals don't need line numbers */
} LitObj;
 
/********************************************************/
/* Data structure used to describe expressions          */
/********************************************************/ 

typedef struct Expn_tag {
  ExpnType  etype;      /* type of expression           */
  union expnVal_tag {
  
    struct expnOp_tag {      /* operator                */
        OperType eoper ;     /* type of operator        */
        ExpnP    eopleft ;   /* left operand (if any )  */
        ExpnP    eopright ;  /* right operand (if any)  */
    } eop ;
    
    struct expnFn_tag {      /* function call           */
         char *  efname ;    /* name of function        */
         ArgsP   efargs ;    /* list of arguments (if any) */
    } efn ;
    
    LitObj      elitVal ;    /* literal constant        */
    
    char *      eident ;     /*  identifier             */
    
    struct expnCd_tag {      /* conditional expression  */
        ExpnP  control ;     /* control expression      */
        ExpnP  contrue ;     /* true expression         */
        ExpnP  confalse ;    /* false expression        */
    } econx ;
    
  } evalue;     /* value of the expression              */

  int line; /* line number */
} ExpnObj;
 
/********************************************************/ 
/* Structure for describing declared entities           */
/********************************************************/

typedef struct Decl_tag {
  DclType   dctype ;     /* type of declaration         */
  DataType  dtype;       /* data type                   */
  char *    dname;       /* variable/func name          */
  FparmP    dfparm;      /* formal parameters (if any)  */
  int       dbound;      /* array bound (if any)        */
  ObjectP   dobject;     /* associated object (if any)  */
  ScopeP    dscope ;     /* containing scope (if any)   */
  ExpnP     dinitexpn;   /* expression for initialization*/
  int line; /* line number */
} DeclObj;
 
/********************************************************/ 
/* Formal parameters (to functions and procedures)      */
/********************************************************/
typedef struct FParm_tag {
  char*     fname;       /* parameter name              */
  DataType  ftype;       /* parameter datatype          */
  FparmP    fnext;       /* next parameter              */
  int line; /* line number */
} FParms ;
 
/********************************************************/ 
/* Actual parameters (to functions and procedures)      */
/********************************************************/
typedef struct Args_tag {
  ExpnP   avalue;       /* argument expression          */
  ArgsP    anext;        /* next argument               */
  int line; /* line number */
} Args ;
 
/********************************************************/ 
/* Statement Object.  Used to specify one statement     */
/********************************************************/
typedef struct Stmt_tag {
  StmtType      stype ;         /* statement type       */
  ExpnP         sexpn1, sexpn2, sexpn3;/* related expressions  */
  ObjectP       sstmt1, sstmt2 ; /* related statements  */
  ArgsP         sargs ;         /* related arguments    */
  ScopeP        sscope ;  /* related scope (if any)     */
  char*     	pname;  /* procedure name */
  int 		line; /* line number */
  int 		incramt;  /* increment amount (if any) */
} StmtObj ;

/********************************************************/ 
/* Scope Object.  Used to specify one scope             */
/********************************************************/
typedef struct Scope_tag {
  ScopeP        scparent ;      /* containing scope     */
  ObjectP       scbody ;        /* body of this scope   */
                                /* a list of objects    */
  int line; /* line number */
} ScopeObj ;
 
/********************************************************/ 
/* Object structure.  Used to store anything that       */
/* will result in code generation or semantic analysis: */
/* expressions, statements, declarations, etc.          */
/********************************************************/
typedef struct Object_tag {
  ObjType  otype;       /*  type of this object         */
  
  union  objectValue_tag {
        DeclP  decl ;           /* declaration          */
        StmtP  stmt ;   /* statement            */
        ExpnP  expn ;           /* expression           */
        ScopeP scope ;  /* scope                */
  } ovalue ;            /* value of the object          */
  
  ObjectP  onext ;      /* pointer to next related object */
} Object ;

#endif



