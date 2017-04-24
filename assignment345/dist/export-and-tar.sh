#!/bin/bash
# export a fresh copy of the project and tar it up
# $Id: export-and-tar.sh,v 1.6 2003/03/27 06:34:26 g1u Exp $
echo "exporting project.."
cvs -d `cat CVS/Root` export -f -D now compiler488-345

echo "creating tar file.." 
tar cvvzf c488h05.5.tar.gz -C compiler488-345 '.'

echo "(remember to run 'clean-up.sh' after submitting)"
