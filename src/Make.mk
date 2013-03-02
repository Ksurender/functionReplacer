ABSPATH := $(shell pwd)
UNITTESTDIR_DEFINE := -DUNITTESTDIR_ABSPATH=$(ABSPATH)/unittest/

INCLUDE_DIRS := unittest/ ./ /cygdrive/c/KS/unittest-cpp/unittest-cpp/UnitTest++/src
INCLUDE_FLAGS := $(foreach dir,$(INCLUDE_DIRS),-I$(dir))

LIB_DIRS := /cygdrive/c/KS/unittest-cpp/unittest-cpp/UnitTest++
LINK_FLAGS := $(foreach dir,$(LIB_DIRS),-L$(dir))

TEST_LIBS_NAMES := UnitTest++
TEST_LIBS := $(foreach lib,$(TEST_LIBS_NAMES),-l$(lib))

COMPFLAGS := -Wall $(INCLUDE_FLAGS)

DEBUG ?= 0
ifeq ($(DEBUG),1)
	COMPFLAGS := $(COMPFLAGS) -g
endif

PROFILE ?= 0
ifeq ($(PROFILE),1)
	COMPFLAGS := $(COMPFLAGS) -O0 -fprofile-arcs -ftest-coverage
endif

COMPILER := g++

# Targets
all : FunctionReplacer.o unittest/testsFor_FunctionReplacer unittest/testsFor_FunctionArgLocator FunctionReplacer

FunctionReplacer.o : FunctionArgLocator.o FunctionReplacer.cpp FunctionReplacer.h
	$(COMPILER) $(COMPFLAGS) -c -o FunctionReplacer.o FunctionReplacer.cpp

FunctionArgLocator.o : CharProcessor.o FunctionArgLocator.cpp FunctionArgLocator.h
	$(COMPILER) $(COMPFLAGS) -c -o FunctionArgLocator.o FunctionArgLocator.cpp 

CharProcessor.o : CharProcessor.cpp CharProcessor.h
	$(COMPILER) $(COMPFLAGS) -c -o CharProcessor.o CharProcessor.cpp

unittest/testsFor_FunctionReplacer : FunctionReplacer.o unittest/testsFor_FunctionReplacer.o
	$(COMPILER) $(COMPFLAGS) $(LINK_FLAGS) -o unittest/testsFor_FunctionReplacer FunctionReplacer.o FunctionArgLocator.o CharProcessor.o unittest/testsFor_FunctionReplacer.o $(TEST_LIBS)

unittest/testsFor_FunctionArgLocator : FunctionArgLocator.o unittest/testsFor_FunctionArgLocator.o
	$(COMPILER) $(COMPFLAGS) $(LINK_FLAGS) -o unittest/testsFor_FunctionArgLocator FunctionArgLocator.o CharProcessor.o unittest/testsFor_FunctionArgLocator.o $(TEST_LIBS)

FunctionReplacer : FunctionReplacerFrontEnd.o FunctionReplacer.o
	$(COMPILER) $(COMPFLAGS) $(LINK_FLAGS) -o FunctionReplacer FunctionReplacerFrontEnd.o FunctionReplacer.o FunctionArgLocator.o CharProcessor.o

FunctionReplacerFrontEnd.o : FunctionReplacer.o FunctionArgLocator.o FunctionReplacerFrontEnd.cpp
	$(COMPILER) $(COMPFLAGS) -c -o FunctionReplacerFrontEnd.o FunctionReplacerFrontEnd.cpp

unittest/testsFor_FunctionArgLocator.o : unittest/testsFor_FunctionArgLocator.cpp FunctionArgLocator.cpp FunctionArgLocator.h
	$(COMPILER) $(COMPFLAGS) -c -o unittest/testsFor_FunctionArgLocator.o unittest/testsFor_FunctionArgLocator.cpp

unittest/testsFor_FunctionReplacer.o : unittest/testsFor_FunctionReplacer.cpp unittest/testsFor_FunctionReplacer.h FunctionReplacer.cpp FunctionReplacer.h
	$(COMPILER) $(COMPFLAGS) $(UNITTESTDIR_DEFINE) -c -o unittest/testsFor_FunctionReplacer.o unittest/testsFor_FunctionReplacer.cpp

clean :
	rm FunctionReplacer.o
	rm FunctionArgLocator.o
	rm CharProcessor.o
	rm unittest/testsFor_FunctionReplacer.o
	rm unittest/testsFor_FunctionReplacer
	rm unittest/testsFor_FunctionArgLocator.o
	rm unittest/testsFor_FunctionArgLocator
	rm FunctionReplacerFrontEnd.o
	rm FunctionReplacer

.PHONY : all clean
