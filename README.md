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
`groff` amd `ctags` are optional. `makedepend` is used on Linux but not macOS.

### On macOS:
1. Install XCode from the App Store
1. Run `xcode-select --install` from the terminal to install Command Line Tools
1. Install [homebrew](https://brew.sh/)
1. Run `brew install groff`
1. Install universal ctags as described [here](https://github.com/universal-ctags/homebrew-universal-ctags)
`flex` and `bison` should be available in `/usr/bin` already. 

If you `brew install ctags` you will get [exuberant](http://ctags.sourceforge.net/) ctags rather than 
[universal](https://github.com/universal-ctags) ctags, not that it matters in this context.

### On Ubuntu:
* Run `sudo apt-get install -y gcc make flex bison groff ctags xutils-dev`

That last one is for `makedepend`, in case you were wondering. 

## Getting Started
`assignment1` doesn't have any runnable code. `assignment2` and `assignment345` are independent of each 
other, with the latter effectively superceding the former. 

cd into either `assignment2` or `assignment345/src`, then run `make && make test`
