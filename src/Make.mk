INCLUDE_DIRS := unittest/ ./ /cygdrive/c/KS/unittest-cpp/unittest-cpp/UnitTest++/src
INCLUDE_FLAGS := $(foreach dir,$(INCLUDE_DIRS),-I$(dir))

LIB_DIRS := /cygdrive/c/KS/unittest-cpp/unittest-cpp/UnitTest++
LINK_FLAGS := $(foreach dir,$(LIB_DIRS),-L$(dir))

TEST_LIBS_NAMES := UnitTest++
TEST_LIBS := $(foreach lib,$(TEST_LIBS_NAMES),-l$(lib))

DEBUG ?= 0

COMPFLAGS := -Wall $(INCLUDE_FLAGS)
ifeq ($(DEBUG),1)
	COMPFLAGS := $(COMPFLAGS) -g
endif

COMPILER := g++


# Targets
all : FunctionReplacer.o unittest/testsFor_FunctionReplacer.exe unittest/testsFor_FunctionArgLocator.exe

FunctionReplacer.o : FunctionArgLocator.o FunctionReplacer.cpp FunctionReplacer.h
	$(COMPILER) $(COMPFLAGS) -c -o FunctionReplacer.o FunctionReplacer.cpp

FunctionArgLocator.o : CharProcessor.o FunctionArgLocator.cpp FunctionArgLocator.h
	$(COMPILER) $(COMPFLAGS) -c -o FunctionArgLocator.o FunctionArgLocator.cpp 

CharProcessor.o : CharProcessor.cpp CharProcessor.h
	$(COMPILER) $(COMPFLAGS) -c -o CharProcessor.o CharProcessor.cpp

unittest/testsFor_FunctionReplacer.exe : FunctionReplacer.o unittest/testsFor_FunctionReplacer.o
	$(COMPILER) $(COMPFLAGS) $(LINK_FLAGS) -o unittest/testsFor_FunctionReplacer.exe FunctionReplacer.o unittest/testsFor_FunctionReplacer.o $(TEST_LIBS)

unittest/testsFor_FunctionArgLocator.exe : FunctionArgLocator.o unittest/testsFor_FunctionArgLocator.o
	$(COMPILER) $(COMPFLAGS) $(LINK_FLAGS) -o unittest/testsFor_FunctionArgLocator.exe FunctionArgLocator.o CharProcessor.o unittest/testsFor_FunctionArgLocator.o $(TEST_LIBS)

unittest/testsFor_FunctionArgLocator.o : unittest/testsFor_FunctionArgLocator.cpp FunctionArgLocator.cpp FunctionArgLocator.h
	$(COMPILER) $(COMPFLAGS) -c -o unittest/testsFor_FunctionArgLocator.o unittest/testsFor_FunctionArgLocator.cpp

unittest/testsFor_FunctionReplacer.o : unittest/testsFor_FunctionReplacer.cpp unittest/testsFor_FunctionReplacer.h FunctionReplacer.cpp FunctionReplacer.h
	$(COMPILER) $(COMPFLAGS) -c -o unittest/testsFor_FunctionReplacer.o unittest/testsFor_FunctionReplacer.cpp

clean :
	rm FunctionReplacer.o
	rm FunctionArgLocator.o
	rm CharProcessor.o
	rm unittest/testsFor_FunctionReplacer.o
	rm unittest/testsFor_FunctionReplacer.exe

.PHONY : all clean
