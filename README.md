# CSC488H1 - Spring 2003

## Authors (Group c488h05):
* Ryan "The Cheat" MacLean
* Victor "Strong Bad" Glazer
* Jacob "The Yellow Dart" Tseng
* jon "Homestar" pipitone

## Supported Platforms
Everything should work out of the box on [macOS Sierra](https://www.apple.com/macos/sierra/) 
and [Ubuntu 16.04](https://www.ubuntu.com/download/desktop), provided the necessary dependencies are 
installed as described below.

## Dependencies
You will need a C compiler (`clang` or `gcc`) and GNU `make`. `flex` and `bison` are also required. 
`groff` amd `ctags` are optional. `makedepend` is used on Linux, but not macOS.

### On macOS:
1. Install XCode from the App Store
1. Run `xcode-select --install` from the terminal to install Command Line Tools
1. Install [homebrew](https://brew.sh/)
1. Run `brew install groff`
1. Install universal ctags as described [here](https://github.com/universal-ctags/homebrew-universal-ctags).

`flex` and `bison` should be available in `/usr/bin` already. 

If you `brew install ctags` you will get [exuberant](http://ctags.sourceforge.net/) ctags rather than 
[universal](https://github.com/universal-ctags) ctags, not that it matters in this context.

### On Ubuntu:
* Run `sudo apt-get install -y gcc make flex bison groff ctags xutils-dev`

`xutils-dev` is how you pick up `makedepend`, in case you were wondering. 

## Overview
* [assignment1](https://github.com/vglazer/csc488/tree/master/assignment1) is a collection of programs written in the "CSC488 language", the programming language we 
are compiling. you can't run any of these without the compiler from assignment345, however. a formal 
grammar for the language can be found in assignment2.
* [assignment2](https://github.com/vglazer/csc488/tree/master/assignment2) contains a lexer (or scanner) and a parser for the "csc488 language". the code for these is generated using 
[flex](https://github.com/westes/flex) and [bison](https://www.gnu.org/software/bison/), respectively. 
there are also some tests which are based on the programs from assignment1.
* [assignment345](https://github.com/vglazer/csc488/tree/master/assignment345) builds on assignment2, but is independent from it. it is a full-fledged compiler for 
the "csc488 language", which emits instructions for a fictitious machine. there are also a variety of tests.
