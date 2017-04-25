## File Manifest
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

## TODO
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

