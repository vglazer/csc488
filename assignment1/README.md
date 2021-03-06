# A1 documentation 

## Assignment 1, Program a1
    Requirements:      A program that doesn't use arrays, functions or procedures
    Description:       Booleans and their operations.  Performs a series of
                       operations on boolean variables, checks that operator
                       precedence.  Also tests use of uppercase and lowercase
                       identifiers for variables.
    Expected Output:   "C = 0; D = 1; E = 1; F = 0"
    Key Rules Used:
           program:    scope
         statement:    variable '=' expression
                       'put' output
                       statement statement
       declaration:    type identifier
                       declaration declaration
             scope:    '{' declaration statement '}'
            output:    text
                       'skip'
                       output ',' output
              type:    'boolean'
          variable:    variablename
        expression:    'true'
                       'false'
                       '!' expression
                       expression '&' expression
                       expression '|' expression
                       expression '=' expression
                       '(' expression ')'
                       '(' expression '?' expression ':' expression ')'
                       variable
       variablename:   identifier

## Assignment 1, Program a2
    Requirements:      A program that doesn't use arrays, functions or procedures
    Description:       Integers and their operations.  Tests a number of
                       integer operations, verifies that precedence rules
                       work correctly.
    Expected Output:   "b = 5; c = 8; d = -5; e = 5; f = -20; g = 2"
                       "h = -17; k = 10"
    Key Rules Used:
                output:expression
                  type:'int'
            expression:integer
                       '-' expression
                       expression '+' expression
                       expression '-' expression
                       expression '*' expression
                       expression '/' expression

# Assignment 1, Program a3
    Requirements:      A program that doesn't use arrays, functions or procedures
    Description:       Program that tests control flow and comparison operators
    Expected Output:   "i < j"
                       "k < l"
    Key Rules Used:
         statement:    'if' expression 'then' statement 'fi'
                       'if' expression 'then' statement 'else' statement 'fi'
        expression:    expression '#' expression
                       expression '<''=' expression
                       expression '>' expression
                       `
# Assignment 1, Program b
    Requirements:      A program using arrays but not functions or procedures
    Description:       This program allocates an array of the maximum allowed
                       size, assigns values to it and uses it in a comparison.
                       If all goes as planned, the program outputs "success".
    Expected Output:   "success"
    Key Rules Used:
       declaration:    type identifier '[' integer ']'
          variable:    arrayname '[' expression ']'
        expression:    expression '<' expression
         arrayname:    identifier

# Assignment 1, Program c
    Requirements:      A program using loops including nested loops
    Description:       This program uses an outer and inner loop to add
                       3 to a number (which is initially 0) until that
                       number is larger than 10
    Expected Output:   "i = 12"
    Key Rules Used:
         statement:    'while' expression 'do' statement 'od'
                       'do' statement 'until' expression
                       'exit'
         expression:   expression '>''=' expression

# Assignment 1, Program d
    Requirements:      A program using non-recursive procedures without
                       parameters
    Description:       This program outputs the parity of integers between
                       1 and 7.
    Expected Output:   "odd"
                       "even"
                       "odd"
                       "even"
                       "odd"
                       "even"
                       "odd"
    Key Rules Used:
         statement:    'return'
                       procedurename
       declaration:    'proc' identifier ':' scope
     procedurename:    identifier

# Assignment 1, Program e
    Requirements:      A program using non-recursive functions with parameters
    Description:       This program defines a function for calculating modulo,
                       then it uses that function and outputs the result.
    Expected Output:   "trying: a mod b, a > b ... correct"
                       "trying: a mod b, a < b ... correct"
    Key Rules Used:
         statement: 'result' expression
                    procedurename '(' arguments ')'
       declaration: type identifier '(' parameters ')' ':' scope
                    'proc' identifier '(' parameters ')' ':' scope
             scope: '{' statement '}'
         arguments: expression
                    arguments ',' arguments
        parameters: type identifier
                    parameters, parameters
        expression: functionname '(' arguments ')'
                    parametername
      functionname: identifier
     parametername: identifier

# Assignment 1, Program f
    Requirements:      A program using recursive functions with parameters
    Description:       This program defines a function for calculating factorial,
                       and then uses it and outputs the result.
    Expected Output:   "trying 0! ... correct"
                       "trying 7! ... correct"
    Key Rules Used:    No new rules used

# Assignment 1, Program g
    Requirements:    A program using input and ouput
    Description:     This program calculates the number of digits in a number
    Expected Output: "Enter an integer: [n]"
                     "The number has [d] digit(s)"
                     (where [n] is the number entered by the user
                     and [d] is the number of decimal digits in [n])
    Key Rules Used:
         statement: 'get' input
             input: variable

# Assignment 1, Program h
    Requirements:      A program that uses a procedure declared within
                       another procedure
    Description:       This program uses a nested procedure declaration
    Expected Output:   none
    Key Rules Used:    No new rules used

# Additional notes:
    The following rules are NOT tested by these programs:
    statement : scope
    declaration : type identifier ':' scope
    input : input ',' input
    expression : functionname
    All other rules are tested.

## Notes
1. arrays                                     :  ryan_A1_ProgramB
1. loops, including nested loops              :  loops
1. non-recursive procedures without parameters:  procnoparam
1. non-recursive functions with parameters    :  jp_e, but remove `test_result()`
1. recursive functions with parameters        :  jp_f, remove `test_result()`
1. input and output                           :  jp_g, scoping redundant?
1. procedure in procedure                     :  procinproc

