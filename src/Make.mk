ABSPATH := $(shell pwd)
UNITTESTDIR_DEFINE := -DUNITTESTDIR_ABSPATH=$(ABSPATH)/unittest/

UNITTESTCPP_INCLUDEDIR_ERROR = 
ifeq ($(origin UNITTESTCPP_INCLUDEDIR),undefined)
	UNITTESTCPP_INCLUDEDIR_ERROR = $(error Please define location of UnitTest++ include directory using UNITTESTCPP_INCLUDEDIR environement variable)	
endif
INCLUDE_DIRS := unittest/ ./ $(UNITTESTCPP_INCLUDEDIR)
INCLUDE_FLAGS := $(foreach dir,$(INCLUDE_DIRS),-I$(dir))

UNITTESTCPP_LIBDIR_ERROR =
ifeq ($(origin UNITTESTCPP_LIBDIR),undefined)
	UNITTESTCPP_LIBDIR_ERROR = $(error Please define location of UnitTest++ static library directory using UNITTESTCPP_LIBDIR environment variable)	
endif
LIB_DIRS := $(UNITTESTCPP_LIBDIR)
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
	$(UNITTESTCPP_INCLUDEDIR_ERROR)
	$(UNITTESTCPP_LIBDIR_ERROR)
	$(COMPILER) $(COMPFLAGS) $(LINK_FLAGS) -o unittest/testsFor_FunctionReplacer FunctionReplacer.o FunctionArgLocator.o CharProcessor.o unittest/testsFor_FunctionReplacer.o $(TEST_LIBS)

unittest/testsFor_FunctionArgLocator : FunctionArgLocator.o unittest/testsFor_FunctionArgLocator.o
	$(UNITTESTCPP_INCLUDEDIR_ERROR)
	$(UNITTESTCPP_LIBDIR_ERROR)
	$(COMPILER) $(COMPFLAGS) $(LINK_FLAGS) -o unittest/testsFor_FunctionArgLocator FunctionArgLocator.o CharProcessor.o unittest/testsFor_FunctionArgLocator.o $(TEST_LIBS)

FunctionReplacer : FunctionReplacerFrontEnd.o FunctionReplacer.o
	$(COMPILER) $(COMPFLAGS) $(LINK_FLAGS) -o FunctionReplacer FunctionReplacerFrontEnd.o FunctionReplacer.o FunctionArgLocator.o CharProcessor.o

FunctionReplacerFrontEnd.o : FunctionReplacer.o FunctionArgLocator.o FunctionReplacerFrontEnd.cpp
	$(COMPILER) $(COMPFLAGS) -c -o FunctionReplacerFrontEnd.o FunctionReplacerFrontEnd.cpp

unittest/testsFor_FunctionArgLocator.o : unittest/testsFor_FunctionArgLocator.cpp FunctionArgLocator.cpp FunctionArgLocator.h
	$(UNITTESTCPP_INCLUDEDIR_ERROR)
	$(UNITTESTCPP_LIBDIR_ERROR)
	$(COMPILER) $(COMPFLAGS) -c -o unittest/testsFor_FunctionArgLocator.o unittest/testsFor_FunctionArgLocator.cpp

unittest/testsFor_FunctionReplacer.o : unittest/testsFor_FunctionReplacer.cpp unittest/testsFor_FunctionReplacer.h FunctionReplacer.cpp FunctionReplacer.h
	$(UNITTESTCPP_INCLUDEDIR_ERROR)
	$(UNITTESTCPP_LIBDIR_ERROR)
	$(COMPILER) $(COMPFLAGS) $(UNITTESTDIR_DEFINE) -c -o unittest/testsFor_FunctionReplacer.o unittest/testsFor_FunctionReplacer.cpp

clean :
	rm -f FunctionReplacer.o
	rm -f FunctionArgLocator.o
	rm -f CharProcessor.o
	rm -f unittest/testsFor_FunctionReplacer.o
	rm -f unittest/testsFor_FunctionReplacer
	rm -f unittest/testsFor_FunctionArgLocator.o
	rm -f unittest/testsFor_FunctionArgLocator
	rm -f FunctionReplacerFrontEnd.o
	rm -f FunctionReplacer

.PHONY : all clean
