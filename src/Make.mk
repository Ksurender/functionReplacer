INCLUDE_DIRS := unittest/ ./
INCLUDE_FLAGS := $(foreach dir,$(INCLUDE_DIRS),-I$(dir))

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

unittest/testsFor_FunctionReplacer.exe : FunctionReplacer.o unittest/TestManager.o unittest/testsFor_FunctionReplacer.o
	$(COMPILER) $(COMPFLAGS) -o unittest/testsFor_FunctionReplacer.exe FunctionReplacer.o unittest/TestManager.o unittest/testsFor_FunctionReplacer.o

unittest/testsFor_FunctionArgLocator.exe : FunctionArgLocator.o unittest/TestManager.o unittest/testsFor_FunctionArgLocator.o
	$(COMPILER) $(COMPFLAGS) -o unittest/testsFor_FunctionArgLocator.exe FunctionArgLocator.o CharProcessor.o unittest/TestManager.o unittest/testsFor_FunctionArgLocator.o

unittest/TestManager.o : unittest/TestManager.cpp unittest/TestManager.h
	$(COMPILER) $(COMPFLAGS) -c -o unittest/TestManager.o unittest/TestManager.cpp

unittest/testsFor_FunctionArgLocator.o : unittest/testsFor_FunctionArgLocator.cpp unittest/testsFor_FunctionArgLocator.h FunctionArgLocator.cpp FunctionArgLocator.h
	$(COMPILER) $(COMPFLAGS) -c -o unittest/testsFor_FunctionArgLocator.o unittest/testsFor_FunctionArgLocator.cpp

unittest/testsFor_FunctionReplacer.o : unittest/testsFor_FunctionReplacer.cpp unittest/testsFor_FunctionReplacer.h FunctionReplacer.cpp FunctionReplacer.h
	$(COMPILER) $(COMPFLAGS) -c -o unittest/testsFor_FunctionReplacer.o unittest/testsFor_FunctionReplacer.cpp

clean :
	rm FunctionReplacer.o
	rm FunctionArgLocator.o
	rm CharProcessor.o
	rm unittest/TestManager.o
	rm unittest/testsFor_FunctionReplacer.o
	rm unittest/testsFor_FunctionReplacer.exe

.PHONY : all clean
