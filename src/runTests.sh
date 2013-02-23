#!/bin/bash
# runtests.sh
# Builds and runs unittest suite for function replacer

echo Building src and unittest

make -f Make.mk DEBUG=1 PROFILE=1

echo Build Complete
echo 
echo Running unittest suite
echo

echo TESTING : FunctionArgLocator
./unittest/testsFor_FunctionArgLocator.exe


echo && echo TESTING : FunctionReplacer
./unittest/testsFor_FunctionReplacer.exe

echo unittest suit complete
