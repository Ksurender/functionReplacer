#include "FunctionArgLocator.h"
#include "testsFor_FunctionArgLocator.h"

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

int main(int argn, char** argv)
{
  test_instantiate();
  return 0;
}



