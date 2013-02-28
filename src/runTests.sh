#!/bin/bash
# runtests.sh
# Builds and runs unittest suite for function replacer

# TODO : Add input argument parser for DEBUG AND PROFILE
# Create makefile command to match


echo Building src and unittest

make -f Make.mk DEBUG=1

echo Build Complete
echo 
echo Running unittest suite
echo

echo TESTING : FunctionArgLocator
./unittest/testsFor_FunctionArgLocator


echo && echo TESTING : FunctionReplacer
./unittest/testsFor_FunctionReplacer

echo unittest suit complete
