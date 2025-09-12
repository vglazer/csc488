# CSC488H1 - Spring 2003

```
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

                ADDR 1 0                 ; result 1
                PUSH 3
                SUB
                PUSH 1                   ; push value of expn
                STORE
                PUSH [ label funclean ]
                BR

     :condf     ADDR 1 0                 ; address of return value
                PUSH 3
                SUB

                                         ; call factorial
                PUSH 0                   ; space for result
                ADDR 1 0                 ; back up display
                PUSH [ label retaddr ]   ; push return address

                                         ; load arguments
                ADDR 1 0                 ; load the first arg passed in, (n)
                LOAD                     ; load n
                PUSH 1
                SUB                      ; subtract 1 from n

                PUSH [ label factorial ] ; go to beginning of function
                BR

     :retaddr   SETD 1
                                         ; multiply result by n
                ADDR 1 0
                LOAD                     ; load n again
                MUL                      ; multiply n with factorial(n-1)

                STORE                    ; store result
                PUSH [ label funclean ]
                BR                       ; end result factorial(n-1) * n
```

## Authors (Group c488h05)

* Ryan "The Cheat" MacLean
* Victor "Strong Bad" Glazer
* Jacob "The Yellow Dart" Tseng
* jon "Homestar" pipitone

## Supported Platforms

Everything should work out of the box on [macOS Sierra](https://www.apple.com/macos/sierra/) 
and [Ubuntu 16.04](https://www.ubuntu.com/download/desktop), provided the necessary dependencies are 
installed as described below.

Other flavors of Linux should also work, but haven't been tested. The same goes for 
[Cygwin](https://www.cygwin.com/) and [MinGW](http://www.mingw.org/).

## Dependencies

You will need a C compiler (`clang` or `gcc`) and GNU `make`. `flex` and `bison` are also required. 
`ctags`, `groff` and `ps2pdf` are optional.

### On macOS

1. Install XCode from the App Store
1. Run `xcode-select --install` from the terminal to install Command Line Tools
1. Install [homebrew](https://brew.sh/)
1. Run `brew install groff ghostscript ctags`

`flex` and `bison` should be available in `/usr/bin` already.

### On Ubuntu

* Run `sudo apt-get install -y gcc make flex bison groff ctags ghostscript`

`ghostscript` provides `ps2pdf`, in case you were wondering.

## Overview

* [assignment1](https://github.com/vglazer/csc488/tree/master/assignment1) is a collection of programs 
written in the "CSC488 language", the programming language we are building a compiler for. you can't run 
any of these without the compiler from assignment345, however. a formal grammar for the language can be 
found in assignment2.
* [assignment2](https://github.com/vglazer/csc488/tree/master/assignment2) contains a lexer (or scanner) 
and a parser for the "csc488 language". the code for these is generated using 
[flex](https://github.com/westes/flex) and [bison](https://www.gnu.org/software/bison/), respectively. 
there are also some tests which are based on the programs from assignment1.
* [assignment345](https://github.com/vglazer/csc488/tree/master/assignment345) builds on assignment2, 
but is independent from it. it is a full-fledged compiler for the "csc488 language", which emits 
instructions for a fictitious machine. embedded within the compiler binary is a virtual machine for
executing the assembly code it emits, which you wouldn't be able to run otherwise.

## Getting Started

1. `cd assignment345/src`
1. Run `make` (for debuggable binaries) or `make opt` (for optimized ones). This will build the 
compiler and AST test driver, as well as generating a manpage for the compiler in several formats. It will
also index the headers and sources to produce a tags file.
    1. Building the compiler requires `flex` and `bison`
    1. Generating the manpages requires `groff` and `ps2pdf`
    1. Generating tags requires `ctags`
    1. If you just want the compiler, run `make compiler` or `make compiler_opt` instead
1. This should create a compiler for the "csc488 language" in `./compiler488`. see 
`./compiler488.pdf` for detailed usage options.
1. There are tests in `../test`. Most of these can be run with `make test`. 
The rest must be run manually (see the 
[README](https://github.com/vglazer/csc488/blob/master/assignment345/README.md) for details).
1. You can now use the compiler to run the various "csc488 language" programs found in 
[assignment1](https://github.com/vglazer/csc488/tree/master/assignment1), 
[assignment2](https://github.com/vglazer/csc488/tree/master/assignment2) and 
[assignment345](https://github.com/vglazer/csc488/tree/master/assignment345/test).
1. To get a sense of the various compilation steps, try turning on all of the "dump" and "trace" options, 
like so: `./compiler488 -Dabxy -Tacilpsxy -U./dump_file -R./trace_file ../test/q_c`. Then have a look 
at `./dump_file` and `./trace_file`. See the manpage (`assignment345/src/compiler488.{txt,pdf}`) for 
a detailed description of the command-line options.
