#include "FunctionArgLocator.h"

FunctionArgLocator::FunctionArgLocator():
  bracketLevel(0),
  currentArg(0),
  fullFunctionProcessed(0)
{
}

void FunctionArgLocator::feed(const char* toBeProcessed)
{
}

int FunctionArgLocator::needsMore()
{
  if(fullFunctionProcessed) {
    return 0;
  }
  return 1;
}

unsigned int FunctionArgLocator::getOpeningBracketPos()
{
  return 0;
}

unsigned int FunctionArgLocator::getClosingBracketPos()
{
  return 0;
}

void FunctionArgLocator::getLocatedArgs(std::vector<argInfo> &locatedArgs)
{
  locatedArgs = arguments;
}


