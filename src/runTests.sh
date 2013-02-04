#!/bin/bash
# runtests.sh
# Builds and runs unittest suite for function replacer

echo Building src and unittest

make -f Make.mk

echo Build Complete
echo 
echo Running unittest suite
echo

echo TESTING : FunctionArgLocator
./unittest/testsFor_FunctionArgLocator.exe
#./unittest/testsFor_FunctionReplacer.exe

echo Unittest suit complete