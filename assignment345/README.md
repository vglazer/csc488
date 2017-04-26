# File Manifest
    README.md                - this file 
    README.CVS               - instructions on using CVS
    TODO                     - what remains to be done
    src/                     - source for the compiler
    doc/                     - compiler documentation  
    doc/DESIGN.semantics     - semantic checker design documentation  
    doc/DESIGN.codegen       - code generation design documentation  
    doc/templates            - code generation templates
    test/                    - test programs and testing documentation
    test/TESTING.semantics   - testing strategy for semantic checking
    test/TESTING.codegen     - testing strategy for code generation
    test/INDEX.semantics     - description of test cases for semantic checking
    test/INDEX.codegen       - description of test cases for code generation
    test/semantic_test       - semantic checking test case
    test/array_errors        - semantic checking test case
    test/ast_test            - semantic checking test case
    test/errors_test         - semantic checking test case
    test/func_test           - semantic checking test case
    test/func_errors         - semantic checking test case
    test/1a.asm              - code generation example (hand-crufted)
    test/1c.asm              - code generation example (hand-crufted)
    test/1f.asm              - code generation example (hand-crufted)
    test/run_codegentests.sh - a script that runs the code generation test cases
    test/q_a1                - code generation test case (from A1)
    test/q_a2                - code generation test case (from A1)
    test/q_a3                - code generation test case (from A1)
    test/q_b                 - code generation test case (from A1)
    test/q_c                 - code generation test case (from A1)
    test/q_d                 - code generation test case (from A1)
    test/q_e                 - code generation test case (from A1)
    test/q_f                 - code generation test case (from A1)
    test/q_g                 - code generation test case (from A1)
    test/q_h                 - code generation test case (from A1)
    test/display_test        - code generation test case (new)
    dist/                    - files for managing distribution and submission of the assignment

# CODE GENERATION DESIGN DECISIONS

### Rebuilding the symbol table
Under our current implementation, the symbol table is gone by the time the 
semantic checking pass completes, which means that it cannot be reused during 
code generation. Rather than changing our implementation to allow symbol table
hiding (which would presumably require us to modify our lookup method and
much besides), we chose to rebuild the symbol table from scratch during the code
generation pass.

### Program Initialization
We begin each program with code to set the the display for each lexic level to
point to the top of the stack.  Since the top of the stack is determined by
the length of the program, we must backpatch the pushed display addresses in 
the finalization portion of code generation. 

## DECLARATIONS 

### Scalar/Array Declarations
When processing a variable declaration, we simply push the appropriate number
of UNDEFINED (one for scalars, array_size for arrays) onto the stack to
allocate space. Doing this means that any attempt to access the variable
before it has been initialized with a value will cause a machine exception.

### Routine Declarations
When processing a variable declaration, we first push the address of the
routine on the stack. Then we push a branch around the body of the routine,
which we backpatch once we've emitted code for the body.


## EXPRESSIONS

### Arithmetic operators
We used the built-in ADD, SUB, MUL, DIV and NEG instructions to implement '+',
'-' (binary), '*', '/' and '-' (unary).

### Boolean Operators
NOT is implemented using simple arithmetic: NOT X := 1 - X.  Although the
language doesn't require it, we decided to make AND and OR short-circuiting
(left to right). This means that OR had to be re-implemented using conditional
branches, even though the machine comes with it built-in.

### Comparison Operators
For '=' and '<', we just used the built-in EQ and LT instructions. The rest of
the comparison operators were implemented using simple arithmetic, i.e.  a > b
<=> b - a < 0

### Ternary Operator (?)
The ternary operator (?) was implemented in the obvious way, by branching and
pushing the value of the appropriate expression onto the stack.

## STATMENTS 

### Loops and Conditional Statements
If statements and loops were implemented using conditional branches as
outlined in the course notes.

### Input / Output
Text output is done in the obvious way: the ASCII values of the string
characters are pushed onto the stack and then printed using the PRINTC
instruction, one after the other. The PRINTI instruction is used for integer
output, and a combination of LOAD and READI is used for integer input.

### Routine calls
If it is a function we PUSH UNDEFINED to allocate space for the return value.
Next, we back up the display at the routine body's lexic level. Then we push
the return address (i.e. the address of the statement immediately after the
routine call) onto the stack, followed by the n arguments. Finally, we push
the address of the routine body inorder to branch to it. This means that just
before control is transferred to a routine (with n parameters), the stack
looks something like this (not counting the address of the routine code,
which gets popped by BR).

    |      argn      |
    |      ...       |
    |      arg2      |
    |      arg1      |
    | return address |
    |  old display   |
    |  return value  |   * not present if this is a procedure call
    ------------------

Once the routine has control, it sets display at its lexic level to point to
arg1 so that arguments are treated exactly like local variables. The
unfortunate side effect of this is that for functions we must subtract the
magic number 3 from ADDR [ function body's lexic level ] 0 to STORE the return
value in the space we've allocated.

When a 'result' or return statement is encountered, we pop the current
stack frame as described in the section on "result / return". For result
statements we of course STORE the result of the specified expression in the
location allocated for the return value.  On return from a routine the stack
looks like this:

    | old display  |
    | return value |    * not present if this is procedure call
    ----------------

The first thing we do when we come back to the code that called the routine is
restore the old display using SETD.


### Result/Return
Code for these statements follows a similar pattern of first emiting code for
each scope it is embedded in, then jumping to any clean up code for the
routine. Of course, before doing any of this the Result statement stores the
return value. 

Emiting code to clean up each scope the statement is embedded in is somewhat
wasteful of memory, but we felt the simplicity this approach afforded us was
worth it. 

### Scopes
We are not subsuming minor scopes. This means that we back up the display when
we first enter a minor scope (and similarly for program scope). On exit, we
pop locals and restore the display (just like in a function or a procedure).

# DESIGN OVERVIEW

## Abstract Syntax Tree
We made a several modifications to the Abstract Syntax Tree definitions from
definitions given in the starter code. 

In the `Object` structure we changed each field in the `ovalue` field to be
pointers to their respective structures rather than the structures themselves.
This approach allows us to create an `Object` to 'wrap' another node without
copying it into the `Object` structure. Copying the node into the `Object` would
cause any pointers to this node from nodes farther down in the tree to be
invalidated. 

In the `StmtObj` type we changed the type of the fields `sstmt1` and `sstmt2`
from `StmtP` to `ObjectP`.  This allowed us to handle compound statements by
having these fields each point to a list of contained statements.  For
example, when encoding an if-then-else statement in a `StmtObj` we make the
`sstmt1` field point to the list of statements in the if-clause, and the
`sstmt2` field point to the list of statements in the else-clause. This
allows us to use the `Object` structure to build up all lists of statements in a
generic way (regardless of whether this list was eventually going to be part
of a scope, or compound statement). 

As well, in the `StmtObject` we added a string field `pname` to record the name
of a procedure when encoding a procedure call statement.  We decided this
approach was simpler than encoding the procedure name as a literal expression
in one of the `StmtObj`'s `ExpnP` fields.

To handle source coordinate tracking we added an integer field called `line`
to each AST node type except `LitObj` and `Object`.  When the parser makes calls
to the AST module it passes the global `line` variable set by the scanner. 

## Semantic Analysis
Our basic strategy was to mimic the mutually recursive functions approach used
in `astDump.c`. 

A single instance of a specialised struct of type `commFlags` is used to
propagate semantic information up the tree, one level at a time. At this
point, its primary purpose is to help ensure `exit` statements occur only
inside `do` or `while` loops (later on, it will also be used to ensure
`result` and `return` statements occur only inside functions and procedures,
respectively). During statement processing, the `insideLoop` field of the
struct is set to `TRUE` whenever a loop is about to be checked. An error is
then generated if an `exit` statement is found when `insideLoop` is `FALSE`. 

We have decided to report errors as they are detected (rather than bailing out 
as soon as the first error is encountered), even though some of them might turn
out to be spurious. Once semantic analysis is complete, we check the value of 
the global `errorOccured` flag; if it's `TRUE`, no attempt to generate code is 
made. Some of the error conditions we check for are essentially sanity checks 
(i.e. they shouldn't happen, but we are including them for now anyway just
to make sure our tree building works properly); these are marked as `DEBUG` 
errors.

## Symbol table
We decided to implement the symbol table as a fixed-size (array-based) stack.
This approach simplifies matters dramatically by automatically implementing
the lookup rules for the language (assuming one starts at the top of the stack
and works one's way down), and is generally very straightforward. We've also
decided to treat major and minor scopes in exactly the same way, again for
simplicity's sake. The internal representation of the symbol table is not 
visible to the semantic analyser (a special public entry format is used 
instead), so that switching to a list-based implementation or one that 
distinguishes between major and minor scopes (if the need arises in the future)
should be fairly painless.

Internally, each entry consists of the symbol identifier, its kind (i.e.
variable, function or procedure), type (integer scalar, boolean array, etc.)
and a pair of values ("lexic level" and "order number") that identify the
entry uniquely within the table (those two will be used in code generation).
There's also a "number of elements" field, used for entries representing
arrays.   

Global variables (declared static to minimise namespace pollution) are used to
keep track of the current lexic level and order number, as well as the index
of the top of the stack.

The public symbol table interface includes functions that allow the semantic
analyser to signal the beginning and end of a scope (`symbolEnterScope()` and
`symbolExitScope()`, respectively), determine the current lexic level
(`symbolLexicLevel()`; this is useful for checking whether a given identifier is
declared multiple times in a given scope), add a new symbol to the table
(`symbolAdd()`), look up a symbol (`symbolLookup()`) and dump either a particular
entry or the entire table to some specified stream (`symbolDumpTable()` and
`symbolDumpTableEntry()`, respectively).

# TODO
### 3d) add a 'forward' declaration
    - parser    [jp:DONE]
        - <type> <indent> ( <paramlist> ) : forward
        - proc   <indent> ( <paramlist> ) : forward
    - ast       [jp:DONE] 
        - add a new type(s) of declarations
        jp: rather than add a new type, i just make the scope of the
        declaration NULL.  If this seems too messy then it'll be easy to add a
        new types: Dforward_proc and Dforward_func or something
    - symbol table         
        - somehow indicate that the declaration is a forward
        - possibly add a spot for the body address? or
          somehow store this in flags.
    - semantic checking
        - on forward declaration make sure signature matches any
          previous forward declaration exactly.  might we be
          able to remove multiple foward declarations for
          the same routine?
        - on body definition make sure signature matches and
          body hasn't already been defined.
    - codegen
        - backpatch function address when we get to the body
          declaration

### 2c) add run time subscript checking
    - codegen
        - on initialization emit a function that takes two
          parameters (size of array and subscript) and causes a
          runtime error (PUSH UNDECLARED or something) if the
          subscript is out of bounds.
        - when subscripting to call the above function

### 1b) add an assert statement
    - parser.y
        - assert <expression> 
    - ast
        - add a new statement type 
    - semantic checking
        - check that the expression is a boolean
    - codegen
        - on an assert, emit some code that causes a runtime
          error if the expression returns false.

and one other that we didn't mention, but should be dead
easy:
### 1c) do constant array subscripting at compile time 
    - codegen
        - blah.


