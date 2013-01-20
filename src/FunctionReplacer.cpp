#include "FunctionReplacer.h"

FunctionReplacer::FunctionReplacer(FunctionReplacerSetup initialSetup) :
  setup(initialSetup)
{
}

void FunctionReplacer::setSetup(FunctionReplacerSetup newSetup)
{
  setup = newSetup;
}

FunctionReplacerSetup FunctionReplacer::getSetup()
{
  return setup;
}

// Function Stub.  To be implemented
std::string FunctionReplacer::doReplace(std::string originalCode)
{
  return originalCode;
}


