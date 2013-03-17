#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "FunctionReplacer.h"

static void displayHelpInformation()
{
  std::cout << "FunctionReplacer SOURCE_FILENAME FUNCTION_NAME NUM_ARGS REPLACEMENT_SPEC" << std::endl;
  
std::cout <<

"\nSOURCE_FILENAME is the cpp source file within which you want to replace usages of a function.  The source file after replacement is sent to the standard output. FUNCTION_NAME is the case-sensitive name of the function you want to replace.  "

"To find replacement candidates FunctionReplacer looks for usages of this name followed by an openining parenthesis.  NUM_ARGS is the number of arguments passed "

"to the function you want to replace.  Meaning FunctionReplacer only tries to replace usages of FUNCTION_NAME with NUM_ARGS arguments.  After locating replacement candidates FunctionReplacer identifies the arguments passed to the candidate.  "

"If the number of arguments matches NUM_ARGS it will replace candidate based on REPLACEMENT_SPEC.  This specification should use the format '@N' to represent the location of an argument in the replacement.  Where N is the argument number.  "

"For example:\n\n$ FunctionReplacer mySourceFile.cpp original 2 'myFunction(@1)->myMethod(@2)'\n\nwill replace the following usage within mySourceFile.cpp:\n\noriginal(arg1,arg2)\n\nwith:\n\nmyFunction(arg1)->myMethod(arg2)\n\n"

"NOTE : Because of the argument syntax '@N' FunctionReplacer can only replace functions with less than 10 arguments :-/." 
   
<< std::endl;
}

static void displayIncorrectArgumentsInformation()
{
  std::cout << "Error: Arguments don't correspond to correct usage of FunctionReplacer. "  
    "See 'FunctionReplacer help' for more information.";
}

static int convertCharStringToUINT(const char* charString)
{
  unsigned int convertedFromCharString;
  std::stringstream converter;
  converter << charString;
  converter >> convertedFromCharString;
  return convertedFromCharString;
}

static void displayCannotOpenFileError(const std::string &filename)
{
  std::cout << "Error: Cannot open " << filename << std::endl;
}

int main(int argn, char** argc)
{
  std::string filename;
  std::string originalFunctionName;
  unsigned int originalFunctionNumArgs;
  std::string replacementSpec;

  switch(argn) {
  case 5:
    filename.assign(argc[1]);
    originalFunctionName.assign(argc[2]);
    originalFunctionNumArgs = convertCharStringToUINT(argc[3]);
    replacementSpec.assign(argc[4]);
    break;
  case 2:
    if(std::string("help").compare(argc[1]) == 0) {
      displayHelpInformation();
      return 0;
      break;
    }
  default:
    displayIncorrectArgumentsInformation();
    return 1;
  }

  FunctionReplacerSetup setup = {originalFunctionName, originalFunctionNumArgs, replacementSpec};
  FunctionReplacer functionReplacer(setup);
  
  std::ifstream sourceFile(filename.c_str());
  if(sourceFile.fail()) {
    displayCannotOpenFileError(filename);
    return 1;
  }
  std::stringstream originalCodeStream;
  sourceFile >> originalCodeStream.rdbuf();
 
  std::string codeAfterReplace;
  try {
    codeAfterReplace = functionReplacer.doReplace(originalCodeStream.str());
  }
  catch(MoreThanNineArgumentsException except) {
    std::cout << except.what() << std::endl;
    return 1;
  }
  catch(UnexpectedStringEndReachedException except) {
    std::cout << except.what() << std::endl;
    return 1;
  }

  std::cout << codeAfterReplace;
  return 0;
}
