functionReplacer
================

Refactoring tool for C++.  Used for replacing usage of a function in C++ source code.  This was inspired by the following type of problem:

Replace all usages of 'myFunction(arg1, arg2)' with 'anotherFunction(arg1)->classMethod(arg2)'

If only the name of the function needed to change, some sort of regular expression replacement could likely get the job done.  But this type of replacement requires moving funtion arguments around. At the same time I saw this type of problem, I also wanted play around with TDD, writing "Clean Code", and git.  Alas FunctionReplacer was born.

HOW TO BUILD FunctionReplacer

To build the FunctionReplacer binary use the "Make.mk" file with the 'FunctionReplacer' target:

$ make -f Make.mk FunctionReplacer

For a debug build define a 'DEBUG' environment variable equal to one:

$ make -f Make.mk FunctionReplacer DEBUG=1

There are also unittests in the unittest directory.  They do not get built with the commands above.  The unittest suite uses UnitTest++.  Before building the unittest suit define the following environment variables appropriately: UNITTESTCPP_INCLUDEDIR and UNITTESTCPP_LIBDIR.  To build the unittest suite call make without a specific target after defining this environment variables:

$ make -f Make.mk DEBUG=1

You can also build and run the unittest's using the "runTests.sh" bash script.  

HOW TO USE FunctionReplacer:

The FunctionReplacer binary has the following usage:

$ FunctionReplacer SOURCE_FILENAME FUNCTION_NAME NUM_ARGS REPLACEMENT_SPEC

SOURCE_FILENAME is the cpp source file within which you want to replace usages of a function.  The source file after replacement is sent to the standard output. FUNCTION_NAME is the case-sensitive name of the function you want to replace.  To find replacement candidates FunctionReplacer looks for usages of this name followed by an openining parenthesis.  NUM_ARGS is the number of arguments passed to the function you want to replace.  Meaning FunctionReplacer only tries to replace usages of FUNCTION_NAME with NUM_ARGS arguments.  After locating replacement candidates FunctionReplacer identifies the arguments passed to the candidate.  If the number of arguments matches NUM_ARGS it will replace candidate based on REPLACEMENT_SPEC.  This specification should use the format "@N" to represent the location of an argument in the replacement.  Where N is the argument number.  For example:

$ FunctionReplacer mySourceFile.cpp original 2 'myFunction(@1)->myMethod(@2)'

will replace the following usage within mySourceFile.cpp:

original(arg1,arg2)

with:

myFunction(arg1)->myMethod(arg2)

NOTE : Because of the argument syntax '@N' FunctionReplacer can only replace functions with less than 10 arguments :-/.  