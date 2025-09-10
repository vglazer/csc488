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
of `UNDEFINED` (one for scalars, `array_size` for arrays) onto the stack to
allocate space. Doing this means that any attempt to access the variable
before it has been initialized with a value will cause a machine exception.

### Routine Declarations
When processing a variable declaration, we first push the address of the
routine on the stack. Then we push a branch around the body of the routine,
which we backpatch once we've emitted code for the body.

## EXPRESSIONS

### Arithmetic operators
We used the built-in `ADD`, `SUB`, `MUL`, `DIV` and `NEG` instructions to implement `+`,
`-` (binary), `*`, `/` and `-` (unary).

### Boolean Operators
`NOT` is implemented using simple arithmetic: `NOT X := 1 - X`.  Although the
language doesn't require it, we decided to make `AND` and `OR` short-circuiting
(left to right). This means that `OR` had to be re-implemented using conditional
branches, even though the machine comes with it built-in.

### Comparison Operators
For `=` and `<`, we just used the built-in `EQ` and `LT` instructions. The rest of
the comparison operators were implemented using simple arithmetic, i.e. `a > b
<=> b - a < 0`

### Ternary Operator (?)
The ternary operator (`?`) was implemented in the obvious way, by branching and
pushing the value of the appropriate expression onto the stack.

## STATEMENTS 

### Loops and Conditional Statements
If statements and loops were implemented using conditional branches as
outlined in the course notes.

### Input / Output
Text output is done in the obvious way: the ASCII values of the string
characters are pushed onto the stack and then printed using the `PRINTC`
instruction, one after the other. The `PRINTI` instruction is used for integer
output, and a combination of `LOAD` and `READI` is used for integer input.

### Routine calls
If it is a function we `PUSH UNDEFINED` to allocate space for the return value.
Next, we back up the display at the routine body's lexic level. Then we push
the return address (i.e. the address of the statement immediately after the
routine call) onto the stack, followed by the n arguments. Finally, we push
the address of the routine body inorder to branch to it. This means that just
before control is transferred to a routine (with n parameters), the stack
looks something like this (not counting the address of the routine code,
which gets popped by `BR`).

    |      argn      |
    |      ...       |
    |      arg2      |
    |      arg1      |
    | return address |
    |  old display   |
    |  return value  |   * not present if this is a procedure call
    ------------------

Once the routine has control, it sets display at its lexic level to point to
`arg1` so that arguments are treated exactly like local variables. The
unfortunate side effect of this is that for functions we must subtract the
magic number 3 from `ADDR` [ function body's lexic level ] 0 to `STORE` the return
value in the space we've allocated.

When a `result` or `return` statement is encountered, we pop the current
stack frame as described in the section on "result / return". For result
statements we of course `STORE` the result of the specified expression in the
location allocated for the return value.  On return from a routine the stack
looks like this:

    | old display  |
    | return value |    * not present if this is procedure call
    ----------------

The first thing we do when we come back to the code that called the routine is
restore the old display using `SETD`.

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

# TESTING STRATEGY

## AST BUILDING AND SEMANTIC CHECKING

We decided to go with three rather large test cases involving a fairly
representative sampling of language features, including I/O, both while and
do..until loops with exits, if statements with else clauses, arrays,
functions, procedures and expressions involving both boolean and arithmetic
operators. One test case demonstrates that ast building is working properly,
another shows that semantic analysis and symbol table manipulation are done
correctly and the last one shows that all semantic errors are caught (except
for ones that involve arrays, functions or procedures) by deliberately
invoking them (it also shows that line numbers are handled properly).
Separating ast testing from semantic testing allows us to demonstrate that
arrays, functions and procedures are properly parsed even though we have yet
to implement the corresponding semantic checking.

## CODE GENERATION

The bulk of our testing for code generation was done using our 
programs from assignment1.  These programs were designed to use
almost all of the rules of the language.  We also wrote a new
test case called `display_test` designed to ensure that the code
generation properly handles entering and exiting scopes of all
types.  Each of these tests can be run individually.  We also
wrote a script, `run_codegentests.sh`, which runs a series of these
tests and outputs the results.

# SEMANTIC CHECKING TESTS

## ast_test

### description:

    ast_test is an app written in the csc488 source language designed to test
    the abstract syntax tree built by the compiler It uses a significant portion
    of the language, including arrays, functions and procedures.  

    The semantic checker currently implemented does not support arrays,
    functions or procedures.  As a result it does not work with ast_test. 

    This test is run with the ast dumping flags turned on.

### instructions:

    To run this test, attempt to compile the test from the root of the project
    directory, like so:

        $ src/compiler488 -Da test/errors_test

    Compiling from source file test/ast_test
    Compilation begins

    *** Abstract Syntax Tree ***
            The tree is empty.

    *** End Abstract Syntax Tree ***

    *** Abstract Syntax Tree ***
    { (0x805fb48) 
         Scope (0x805fb38)
            Parent pointer = (nil) 
            { (0x805ed98) 
                 Declaration - array (0x805ed60) 
                 Line: 5
                 int array [ 100 ]              Containing Scope pointer (0x805fb38)
                 End Declaration
            }
            { (0x805f308) 
                 Declaration - function (0x805eda8) 
                 Line: 6
                 int func (i : int, j : int)
                 Associated Object
                    { (0x805f330) 
                         Scope (0x805f2f8)
                            Parent pointer = 0x805fb38 
                            { (0x805f2e8) 
                                 if+else Statement (0x805f2c0)
                                 Line: 22
                                     Expression (0x805ef68)  ! (((i < j) = (j < 100)))
                                    { (0x805f068) 
                                         scope Statement (0x805f040)
                                         Line: 12
                                          Related scope:                                          Scope (0x805f030)
                                                Parent pointer = (nil) 
                                                { (0x805efb8) 
                                                     Declaration - scalar (0x805ef80) 
                                                     Line: 10
                                                     int i                                                  Containing Scope
     pointer (0x805f030)
                                                     End Declaration
                                                }
                                                { (0x805efe0) 
                                                     assignment Statement (0x805f008)
                                                     Line: 12
                                                     Expression (0x805efc8) i
                                                     Expression (0x805eff0) 1
                                                }
                                             End Scope
                                    }
                                    { (0x805f210) 
                                         do-until Statement (0x805f238)
                                         Line: 20
                                             Expression (0x805f220) 0
                                            { (0x805f160) 
                                                 while Statement (0x805f138)
                                                 Line: 18
                                                     Expression (0x805f088) 1
                                                    { (0x805f0a0) 
                                                     put Statement (0x805f0d8)
                                                     Line: 17
                                                      Related arguments: "hello"
                                                    }
                                                    { (0x805f128) 
                                                     exit Statement (0x805f100)
                                                     Line: 17
                                                    }
                                            }
                                            { (0x805f200) 
                                                 put Statement (0x805f1d8)
                                                 Line: 20
                                                  Related arguments: "still doing..", skip
                                            }
                                    }
                                    { (0x805f2b0) 
                                         result Statement (0x805f288)
                                         Line: 22
                                             Expression (0x805f270) i
                                    }
                            }
                         End Scope
                    }
                 Containing Scope pointer (0x805fb38)
                 End Declaration
            }
            { (0x805f618) 
                 Declaration - procedure (0x805f5e0) 
                 Line: 29
                 void printNotted (value : boolean)
                 Associated Object
                    { (0x805f608) 
                         Scope (0x805f5d0)
                            Parent pointer = 0x805fb38 
                            { (0x805f3d8) 
                                 Declaration - scalar (0x805f390) 
                                 Line: 26
                                 int number                              Containing Scope pointer (0x805f5d0)
                                 End Declaration
                            }
                            { (0x805f440) 
                                 assignment Statement (0x805f4c8)
                                 Line: 28
                                     Expression (0x805f3e8) number
                                     Expression (0x805f4b0) (( ! (value) & 1) ? 1 : 0) 
                            }
                            { (0x805f5c0) 
                                 put Statement (0x805f598)
                                 Line: 29
                                  Related arguments: "input boolean notted:", number, skip
                            }
                         End Scope
                    }
                 Containing Scope pointer (0x805fb38)
                 End Declaration
            }
            { (0x805f828) 
                 Declaration - procedure (0x805f7f0) 
                 Line: 33
                 void printSqaredPlusFive (value : int)
                 Associated Object
                    { (0x805f818) 
                         Scope (0x805f7e0)
                            Parent pointer = 0x805fb38 
                            { (0x805f7d0) 
                                 put Statement (0x805f7a8)
                                 Line: 33
                                  Related arguments: "input number squared:", ((value * value) + 5), skip
                            }
                         End Scope
                    }
                 Containing Scope pointer (0x805fb38)
                 End Declaration
            }
            { (0x805f848) 
                 assignment Statement (0x805f8b8)
                 Line: 36
                     Expression (0x805f870) array [ 12 ] 
                     Expression (0x805f8a0) 1000
            }
            { (0x805fa48) 
                 put Statement (0x805fa20)
                 Line: 37
                  Related arguments: "hello world", func(array [ 12 ] , 4), skip
            }
            { (0x805faa8) 
                 proc call Statement (0x805fa80) : printNotted
                 Line: 37
                  Related arguments: 1
            }
            { (0x805fb28) 
                 proc call Statement (0x805fb00) : printSquaredPlusFive
                 Line: 38
                  Related arguments: 5
            }
         End Scope
    }

    *** End Abstract Syntax Tree ***
    Segmentation fault


## errors_test

### description:

    errors_test is an app written in the csc488 source language designed to test
    that the semantic checker correctly detects and reports errors.  No arrays,
    functions or procedures are used in the program.  

    No special flags are used in the execution of this test.

### instructions:

    To run this test, attempt to compile the test from the root of the project
    directory, like so:

        $ src/compiler488 test/errors_test
        
### expected output:

    Compiling from source file test/errors_test
    Compilation begins

    *** Abstract Syntax Tree ***
            The tree is empty.

    *** End Abstract Syntax Tree ***
    ERROR: line 9: j already declared at this lexical level 
    ERROR: line 12: expected boolean expression type
    ERROR: line 20: type mismatch in assigning boolean to integer
    ERROR: line 20: expected boolean control in conditional expresion
    ERROR: line 23: exit occured outside a loop
    ERROR: line 29: put arguments can only be text, integer, or 'skip'
    ERROR: line 32: get arguments can only be integers
    ERROR: line 32: undeclared variable: z
    ERROR: line 32: unknown 

    Compilation Ends
    Execution suppressed due to compilation errors

## semantic_test

### description:

  semantic_test is an app written in the csc488 source language designed to
  test the compiler's symbol table and semantic checking.  When symbol table
  tracing or dumping is turned on , the semantic checker will print the symbol
  table immediately before exiting any scope.  The program does not contain
  any semantic errors, so the semantic checker will accept it.  no arrays,
  functions or procedures are tested in the program.

  This test is run with symbol table dumping turned on.

### instructions: 

    To run this test, attempt to compile the test from the root of the project
    directory, like so:

        $ src/compiler488 -Dy test/semantic_test 

### expected output:

    Compiling from source file test/semantic_test
    Compilation begins

    *** Abstract Syntax Tree ***
            The tree is empty.

    *** End Abstract Syntax Tree ***
    *** Dumping Symbol Table ***
    lexicLevel:1
    orderNumber:1
    SymbTableSize:200
    top:2

             Entry#          sname          skind          stype      snumelems         slevel       sonumber
                  0              a            VAR     SCALAR_INT              0              0              0
                  1              a            VAR     SCALAR_INT              0              1              0
    *** Done Symbol Table ***
    *** Dumping Symbol Table ***
    lexicLevel:1
    orderNumber:0
    SymbTableSize:200
    top:1

             Entry#          sname          skind          stype      snumelems         slevel       sonumber
                  0              a            VAR     SCALAR_INT              0              0              0
    *** Done Symbol Table ***
    *** Dumping Symbol Table ***
    lexicLevel:3
    orderNumber:0
    SymbTableSize:200
    top:5

             Entry#          sname          skind          stype      snumelems         slevel       sonumber
                  0              a            VAR     SCALAR_INT              0              0              0
                  1              b            VAR    SCALAR_BOOL              0              1              0
                  2              c            VAR     SCALAR_INT              0              1              1
                  3              b            VAR     SCALAR_INT              0              2              0
                  4              d            VAR     SCALAR_INT              0              2              1
    *** Done Symbol Table ***
    *** Dumping Symbol Table ***
    lexicLevel:2
    orderNumber:2
    SymbTableSize:200
    top:5

             Entry#          sname          skind          stype      snumelems         slevel       sonumber
                  0              a            VAR     SCALAR_INT              0              0              0
                  1              b            VAR    SCALAR_BOOL              0              1              0
                  2              c            VAR     SCALAR_INT              0              1              1
                  3              b            VAR     SCALAR_INT              0              2              0
                  4              d            VAR     SCALAR_INT              0              2              1
    *** Done Symbol Table ***
    *** Dumping Symbol Table ***
    lexicLevel:1
    orderNumber:2
    SymbTableSize:200
    top:3

             Entry#          sname          skind          stype      snumelems         slevel       sonumber
                  0              a            VAR     SCALAR_INT              0              0              0
                  1              b            VAR    SCALAR_BOOL              0              1              0
                  2              c            VAR     SCALAR_INT              0              1              1
    *** Done Symbol Table ***
    *** Dumping Symbol Table ***
    lexicLevel:0
    orderNumber:1
    SymbTableSize:200
    top:1

             Entry#          sname          skind          stype      snumelems         slevel       sonumber
                  0              a            VAR     SCALAR_INT              0              0              0
    *** Done Symbol Table ***

    Compilation Ends
    Start Exectution         pc =     0, msp =     1, mlp =  8191
            memory[ 0 .. 1 ] =        0  -32768
    End Execution    pc =     0, msp =     1, mlp =  8191
            memory[ 0 .. 1 ] =        0  -32768


## func_test

### description:

    func_test is a program to test semantic checking for procedures and
    functions.  this program contains:
        * declaration of a function with paramters
        * declaration of a function with no parmaters
        * declaration of a procedure with paramters
        * declaration of a procedure with no parmaters
        * function calling another function
        * calls to each of the above routines  

    This test is run with the symbol and AST table dumping flags turned on.

### instructions:

    To run this test, attempt to compile the test from the root of the project
    directory, like so:

        $ src/compiler488 -Day test/func_test

### expected output:
    Compiling from source file test/func_test
    Compilation begins

    *** Abstract Syntax Tree ***
            The tree is empty.

    *** End Abstract Syntax Tree ***

    *** Abstract Syntax Tree ***
    { (0x805ef28) 
         Scope (0x805ef18)
            Parent pointer = (nil) 
            { (0x805e8e8) 
                 Declaration - function (0x805e7e0) 
                 Line: 12
                 int foo (             Associated Object
                    { (0x805e910) 
                         Scope (0x805e8d8)
                            Parent pointer = 0x805ef18 
                            { (0x805e828) 
                                 put Statement (0x805e860)
                                 Line: 14
                                  Related arguments: "hello"
                            }
                            { (0x805e888) 
                                 result Statement (0x805e8b0)
                                 Line: 15
                                     Expression (0x805e898) 12
                            }
                         End Scope
                    }
                 Containing Scope pointer (0x805ef18)
                 End Declaration
            }
            { (0x805ea48) 
                 Declaration - function (0x805e920) 
                 Line: 17
                 boolean foobar (i : int, j : int)
                 Associated Object
                    { (0x805ea70) 
                         Scope (0x805ea38)
                            Parent pointer = 0x805ef18 
                            { (0x805ea28) 
                                 result Statement (0x805ea00)
                                 Line: 19
                                     Expression (0x805e9e8) (i < foo)
                            }
                         End Scope
                    }
                 Containing Scope pointer (0x805ef18)
                 End Declaration
            }
            { (0x805eb70) 
                 Declaration - procedure (0x805eb38) 
                 Line: 24
                 void bar (             Associated Object
                    { (0x805eb60) 
                         Scope (0x805eb28)
                            Parent pointer = 0x805ef18 
                            { (0x805ea90) 
                                 put Statement (0x805eac8)
                                 Line: 23
                                  Related arguments: "happy"
                            }
                            { (0x805eb18) 
                                 return Statement (0x805eaf0)
                                 Line: 23
                            }
                         End Scope
                    }
                 Containing Scope pointer (0x805ef18)
                 End Declaration
            }
            { (0x805ecb0) 
                 Declaration - procedure (0x805ec78) 
                 Line: 29
                 void barfoo (i : boolean)
                 Associated Object
                    { (0x805eca0) 
                         Scope (0x805ec68)
                            Parent pointer = 0x805ef18 
                            { (0x805eba0) 
                                 put Statement (0x805ec08)
                                 Line: 28
                                  Related arguments: "happy"
                            }
                            { (0x805ec58) 
                                 return Statement (0x805ec30)
                                 Line: 28
                            }
                         End Scope
                    }
                 Containing Scope pointer (0x805ef18)
                 End Declaration
            }
            { (0x805ef08) 
                 if Statement (0x805eee0)
                 Line: 35
                     Expression (0x805ed40) foobar(foo, foo)
                    { (0x805edc8) 
                         put Statement (0x805eda0)
                         Line: 33
                          Related arguments: foo
                    }
                    { (0x805ee10) 
                         proc call Statement (0x805ede8) : bar
                         Line: 34
                    }
                    { (0x805eed0) 
                         proc call Statement (0x805eea8) : barfoo
                         Line: 34
                          Related arguments: foobar(1, 2)
                    }
            }
         End Scope
    }

    *** End Abstract Syntax Tree ***
    *** Dumping Symbol Table ***
    lexicLevel:1
    orderNumber:0
    SymbTableSize:200
    top:1

             Entry#          sname          skind          stype      snumelems         slevel       sonumber
                  0            foo       function            int              0              0              0
    *** Done Symbol Table ***
    *** Dumping Symbol Table ***
    lexicLevel:1
    orderNumber:2
    SymbTableSize:200
    top:4

             Entry#          sname          skind          stype      snumelems         slevel       sonumber
                  0            foo       function            int              0              0              0
                  1         foobar       function        boolean              2              0              1
                  2              i         scalar            int              0              1              0
                  3              j         scalar            int              0              1              1
    *** Done Symbol Table ***
    *** Dumping Symbol Table ***
    lexicLevel:1
    orderNumber:0
    SymbTableSize:200
    top:3

             Entry#          sname          skind          stype      snumelems         slevel       sonumber
                  0            foo       function            int              0              0              0
                  1         foobar       function        boolean              2              0              1
                  2            bar           proc           void              0              0              2
    *** Done Symbol Table ***
    *** Dumping Symbol Table ***
    lexicLevel:1
    orderNumber:1
    SymbTableSize:200
    top:5

             Entry#          sname          skind          stype      snumelems         slevel       sonumber
                  0            foo       function            int              0              0              0
                  1         foobar       function        boolean              2              0              1
                  2            bar           proc           void              0              0              2
                  3         barfoo           proc           void              1              0              3
                  4              i         scalar        boolean              0              1              0
    *** Done Symbol Table ***
    *** Dumping Symbol Table ***
    lexicLevel:0
    orderNumber:4
    SymbTableSize:200
    top:4

             Entry#          sname          skind          stype      snumelems         slevel       sonumber
                  0            foo       function            int              0              0              0
                  1         foobar       function        boolean              2              0              1
                  2            bar           proc           void              0              0              2
                  3         barfoo           proc           void              1              0              3
    *** Done Symbol Table ***

    Compilation Ends
    Start Exectution         pc =     0, msp =     1, mlp =  8191
            memory[ 0 .. 1 ] =        0  -32768
    End Execution    pc =     0, msp =     1, mlp =  8191
            memory[ 0 .. 1 ] =        0  -32768

## array_errors

### description:

    array_errors is an app written in the csc488 source language designed to test
    that the semantic checker correctly detects and reports errors for arrays.

    No special flags are used in the execution of this test.

### instructions:

    To run this test, attempt to compile the test from the root of the project
    directory, like so:

        $ src/compiler488 test/array_errors

### expected output:

    Compiling from source file test/array_test
    Compilation begins

    *** Abstract Syntax Tree ***
            The tree is empty.

    *** End Abstract Syntax Tree ***
    ERROR: line 15: undeclared variable: p
    ERROR: line 15: illegal subscripting of non-array variable
    ERROR: line 18: type mismatch in assigning integer to boolean
    ERROR: line 21: type mismatch in array subscript expression.  Must be an integer
    ERROR: line 21: type mismatch in assigning integer to boolean
    ERROR: line 24: type mismatch in assigning boolean to integer
    ERROR: line 27: put arguments can only be text, integer, or 'skip'
    ERROR: line 30: illegal subscripting of non-array variable
    ERROR: line 33: use of array variable as a scalar
    Compiling from source file test/array_test
    Compilation begins

    *** Abstract Syntax Tree ***
            The tree is empty.

    *** End Abstract Syntax Tree ***

    Compilation Ends
    Execution suppressed due to compilation errors

## func_errors

### description:

  func_errors is an app written in the csc488 source language designed to test
  that the semantic checker correctly detects and reports errors for routines.

  No special flags are used in the execution of this test.

### instructions:

    To run this test, attempt to compile the test from the root of the project
    directory, like so:

        $ src/compiler488 test/func_errors

### expected output:

    Compiling from source file test/func_errors
    Compilation begins

    *** Abstract Syntax Tree ***
            The tree is empty.

    *** End Abstract Syntax Tree ***
    ERROR: line 5: return occured outside a procedure
    ERROR: line 2: function missing result statment 
    ERROR: line 14: type mismatch in result expression
    ERROR: line 17: illegal assignment to function
    ERROR: line 20: too few/many arguments in function call
    ERROR: line 23: too few/many arguments in function call
    ERROR: line 26: type mismatch in function argument

    Compilation Ends
    Execution suppressed due to compilation errors

# CODE GENERATION TESTS

## display_test

### description:

  display_test is an program written in the csc488 source language designed to
  test that the code generator correctly emits code to restore the display on
  closing a minor scope, returning from a function, returning from a
  procedure, and exiting a loop.

  No special flags are used in the execution of this test.

### instructions:

    To run this test, attempt to compile the test from the root of the project
    directory, like so:

        $ src/compiler488 test/display_test

### expected output:
    Compiling from source file test/display_test
    Compilation begins

    *** Abstract Syntax Tree ***
            The tree is empty.

    *** End Abstract Syntax Tree ***

    Compilation Ends
    Start Exectution         pc =     0, msp =   737, mlp =  8191
            memory[ 729 .. 737 ] =        1       0       0      15       8       6       0       0  -32768
    i = 10 (expected 10)
    i = 10 (expected 10)
    bar returns 14
    i = 10 (expected 10)
    i = 10 (expected 10)
    End Execution    pc =   736, msp =   737, mlp =  8191    display[ 0 .. 19 ] =      737     737     737     737     737     737     737     737     737     737     737     737     737     737     737     737     737     737     737     737
            memory[ 729 .. 737 ] =        1       0       0      15       8       6       0       0     737

## q_a1 - q_h

### description:

   q_a1, q_a2, q_a3, q_b, q_c, q_d, q_e, q_f q_g and q_h are essentially the programs 
   originally written for a1.  Some of these have been modified slightly to make them
   more useful in the testing of code generation.  Together, they test almost every 
   rule in the language.  See the comments at the beginning of each program for a 
   detailed description.

### instructions:

   To run one of these tests, attempt to compile it from the root of the project
   directory.  To run q_a1, for example, type:
   
       $ src/compiler488 test/q_a1

   We have also prepared a test script which runs display_test as well as q_a1, 
   q_a2, q_a3, q_b, q_c, q_d, q_e, q_f, and q_h.  q_g is omitted because it requires
   user input (q_g does work - it can be tested individually).  To execute this script,
   type
       
       $ run_codegentests.sh 

   from the test directory of the project.

### expected output:
   
   See the top of each program's file for its expected output.

# Appendix: Sample Compiler Inputs and Outputs
Below are a few programs in the "csc488 project language" (the input to the compiler) along with
the corresponding "csc488 machine" assembly code (the output of the compiler).

## Program 1a
A short program that doesn't use arrays, functions or procedures.

### Project language

    {
        int i
        int j

        i = 1
        j = 1

        if 3 + i * 5 / 2 = j then
            put "hello world!", skip
        else 
            put "42", skip
        fi
    }

### Project machine assembly

           ; back up display for scope's lexic level
           ADDR 0 0

           ; set up for scope (somewhat useless in this case)  
           PUSHMT       ; update display
           SETD 0       
       
           PUSH 0   ; allocate space for i
           PUSH 0   ; allocate space for j

           ADDR 0 0 ; LL(i) = ON(i) = 0
           PUSH 1   
           STORE    ; i = 1
           ADDR 0 1 ; LL(j) = 0, ON(j) = 1
           PUSH 1
           STORE    ; j = 1

           ADDR 0 0  
           LOAD     ; load i 
           PUSH 5
           MUL      ; i *= 5
           PUSH 2
           DIV      ; i /= 2
           PUSH 3
           ADD      ; i += 3
           ADDR 0 1
           LOAD     ; push j
           EQ       ; 3 + i * 5 / 2 = j ?
           PUSH [ label else ]  
           BF       ; jump to else branch if i != j

           PUSH 104 ; push 'h'
           PRINTC   ; print 'h'
           PUSH 101 ; push 'e'
           PRINTC   ; print 'e'
           PUSH 108 ; push 'l'
           PRINTC   ; print 'l'
           PUSH 108 ; push 'l' 
           PRINTC   ; print 'l'
           PUSH 111 ; push 'o'
           PRINTC   ; print 'o'
           PUSH 032 ; push ' '
           PRINTC   ; print ' ' 
           PUSH 119 ; push 'w'
           PRINTC   ; print 'w'
           PUSH 111 ; push 'o'
           PRINTC   ; print 'o'
           PUSH 114 ; push 'r'
           PRINTC   ; print 'r'
           PUSH 108 ; push 'l'
           PRINTC   ; print 'l'
           PUSH 100 ; push 'd'
           PRINTC   ; print 'd'
           PUSH 033 ; push '!'
           PRINTC   ; print '!'
           PUSH 012 ; push '\n'
           PRINTC   ; print '\n'

    :else  PUSH 50  ; push '4'
           PRINTC   ; print '4'
           PUSH 52  ; push '2'
           PRINTC   ; print '2'
           PUSH 012 ; push '\n'
           PRINTC   ; print '\n'

           ; clean up scope  
           PUSHMT 
           ADDR 0 0 
           SUB
           POPN

## Program 1c
A program using loops, including nested loops.

### Project language

    {
        int i 
        int j
        int total
 
        total = 0
        i = 0
        while i < 10 do
            j = 0
            do
                total = total + 1
                j = j + 1
            until j = 10
            i = i + 1
        od
        put total, skip
    }

### Project machine assembly

                ; back up display for scope's lexic level
                ADDR 0 0

                ; initialise display at LL = 0
                PUSHMT
                SETD 0

                PUSH 0                    ; allocate space for i
                PUSH 0                    ; allocate space for j
                PUSH 0                    ; allocate space for total

                ; total = 0
                ADDR 0 2    
                PUSH 0
                STORE      

                ; i = 0
                ADDR 0 0
                PUSH 0
                STORE

    :whilecond  ADDR 0 0 
                LOAD
                PUSH 10 
                LT        ; i < 10 ?
                PUSH [ label exit      ]    
                BF                        ; exit if i >= 10

                ADDR 0 1                  ; j = 0
                PUSH 0
                STORE                     

     :dostart   ADDR 0 2
                LOAD                      ; load total
                PUSH 1    
                ADD       
                ADDR 0 2
                STORE                     ; total = total + 1

                ADDR 0 1                  ; load j
                LOAD                      
                PUSH 1
                ADD
                ADDR 0 1
                STORE                     ; j = j + 1

                ADDR 0 1                  ; load j
                LOAD                      
                PUSH 10
                EQ                        ; j = 10 ?
                PUSH [ label dostart   ]  
                BF

                ADDR 0 0                  ; i = 0
                LOAD                     
                PUSH 1
                ADD
                ADDR 0 0
                STORE                     ; i = i + 1

                PUSH [ label whilecond ] 
                BR

    :exit       ADDR 0 2
                LOAD                      ; load total
                PRINTI                    ; put total
                PUSH 012                  ; ASCII for '\n'
                PRINTC                    ; put skip

                ; clean up scope  
                PUSHMT 
                ADDR 0 0 
                SUB
                POPN

## Program 1f 
A program using recursive functions with parameters.

### Project language

    {
        int factorial(int n) :
        {
            if (n = 0) then
                result 1
            else
                result factorial(n - 1) * n
            fi
        }
        put factorial(5)
    }

### Project machine assembly

    ; start of body of main scope
    :startpt    ADDR 0 0                 ; back up display 
                PUSH 0                   ; add space for return of factorial(5)
                ADDR 1 0 
                PUSH [ label retaddr ] 
                PUSH 5                   ; push the arg
                PUSH [ label factorial ] 
                BR
    :retaddr2   SETD 1 
                PRINTI
                PUSHMT                   ; pop locals
                ADDR 0 0 
                SUB
                POPN
                SETD 0                   ; restore display

    ; declaration for factorial
    :factorial  PUSHMT                   ; beginning of factorial
                PUSH 1                   ; push number of args 
                SUB
                SETD 1                   ; set the lexic level

                                         ; if condition expression
                PUSH 0               
                ADDR 1 0
                LOAD
                EQ
                PUSH [ label condf ] 
                BF 

                ADDR 1 0                ; result 1
                PUSH 3
                SUB
                PUSH 1                  ; push value of expn
                STORE
                PUSH [ label funclean ]  
                BR 

     :condf      ADDR 1 0                ; address of return value
                 PUSH 3
                 SUB 

                                         ; call factorial
                 PUSH 0                  ; space for result
                 ADDR 1 0                ; back up display 
                 PUSH [ label retaddr ]  ; push return address 

                                         ; load arguments
                 ADDR 1 0                ; load the first arg passed in, (n)
                 LOAD                    ; load n
                 PUSH 1   
                 SUB                     ; subtract 1 from n

                 PUSH [ label factorial ]; go to beginning of function
                 BR

     :retaddr    SETD 1 
                                         ; multiply result by n
                 ADDR 1 0 
                 LOAD                    ; load n again
                 MUL                     ; multiply n with factorial(n-1) 

                 STORE                   ; store result  
                 PUSH [ label funclean ] 
                 BR                      ; end result factorial(n-1) * n

     :funclean   PUSHMT 
                 ADDR 1 0
                 SUB
                 POPN
                 BR

