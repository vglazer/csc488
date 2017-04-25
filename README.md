# Compiler488
# CSC488H1 - Spring 2003

File Manifest for assignment345:

    README                   - this file 
    README.CVS               - instructions on using CVS
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
    dist/                    - files for managing distribution and submission
                               of the assignment

## Authors:
* Ryan "The Cheat" MacLean
* Victor "Strong Bad" Glazer
* Jacob "The Yellow Dart" Tseng
* jon "Homestar" pipitone

## Group c488h05

## Supported Platforms
Everything should work out of the box on macOS Sierra and Ubuntu 16.04, provided the necessary
dependencies are installed as described below.

## Dependencies
You will need a C compiler (`clang` or `gcc`) and GNU `make`. `flex` and `bison` are also required. 
`groff` amd `ctags` are optional. `makedepend` is used on Linux but not macOS.

### On macOS:
1. Install XCode from the App Store
2. Run `xcode-select --install` from the terminal to install Command Line Tools
3. Install [homebrew](https://brew.sh/)
4. Run `brew install groff`
5. Install universal ctags as described [here](https://github.com/universal-ctags/homebrew-universal-ctags)
`flex` and `bison` should be available in `/usr/bin` already. 

If you `brew install ctags` you will get [exuberant](http://ctags.sourceforge.net/) ctags rather than 
[universal](https://github.com/universal-ctags) ctags, not that it matters in this context.

### On Ubuntu:
1. Run `sudo apt-get install -y gcc make flex bison groff ctags xutils-dev`

That last one is for `makedepend`, in case you were wondering. 

## Getting Started
`assignment1` doesn't have any runnable code. `assignment2` and `assignment345` are independent of each 
other, with the latter effectively superceding the former. 

cd into either `assignment2` or `assignment345/src`, then run `make && make test`
