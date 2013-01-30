#include "FunctionArgLocator.h"
#include "testsFor_FunctionArgLocator.h"

#include <string>
#include <vector>

void test_instantiate()
{
  TestManager testManager("Instantiate");
  
  testManager.doStartAction();  
  try {
    FunctionArgLocator argLocator;
    testManager.doPassAction();
  } 
  catch(...) {
    testManager.doFailAction();
  }
}

void test_singleString_noArgs()
{
  TestManager testManager("singleString_noArgs");
  std::string testFunction("myFunction()");

  testManager.doStartAction();

  FunctionArgLocator argLocator;
  argLocator.feed(testFunction.c_str());

  // Check #1 : Should not need more 
  if(argLocator.needsMore()) {
    testManager.doFailAction();
    return;
  }

  // Check #2: Opening bracket position
  unsigned int openingBracketPos = argLocator.getOpeningBracketPos();
  if(openingBracketPos != 10) {
    testManager.doFailAction();
    return;
  }

  // Check #3: Closing bracket positions
  unsigned int closingBracketPos = argLocator.getClosingBracketPos();
  if(closingBracketPos != 11) {
    testManager.doFailAction();
    return;
  }

  // Check #4: Located arguments
  std::vector<argInfo> locatedArgs;
  argLocator.getLocatedArgs(locatedArgs);

  if(locatedArgs.size() != 0) {
    testManager.doFailAction();
    return;
  }

  testManager.doPassAction();
}


int main(int argn, char** argv)
{
  test_instantiate();
  test_singleString_noArgs();
  return 0;
}



