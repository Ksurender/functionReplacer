#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "FunctionReplacer.h"

static void displayHelpInformation()
{
  std::cout << "FunctionReplacer SOURCE_FILENAME FUNCTION_NAME NUM_ARGS REPLACEMENT_SPEC" << std::endl; 
  // Need to add more info here :(
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
