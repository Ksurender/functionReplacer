/* testsFor_functionReplacer.cpp */
#include "FunctionReplacer.h"
#include "testsFor_FunctionReplacer.h"
#include "UnitTest++.h"

#include <iostream>
#include <sstream>


static int checkSetupsEqual(FunctionReplacerSetup funcReplacerSetup_1,
		     FunctionReplacerSetup funcReplacerSetup_2)
{
  if(funcReplacerSetup_1.originalFunctionName !=
     funcReplacerSetup_2.originalFunctionName)
    {
      return 1;
    }
  
  if(funcReplacerSetup_1.originalFunctionNumArgs !=
     funcReplacerSetup_2.originalFunctionNumArgs)
    {
      return 1;
    }

  if(funcReplacerSetup_1.replaceWithSpec !=
     funcReplacerSetup_2.replaceWithSpec)
    {
      return 1;
    }
  
  return 0;
}

struct twoFunctionsFixture
{

  twoFunctionsFixture() {
    funcReplacerSetup_1.originalFunctionName = std::string("origFun1");
    funcReplacerSetup_1.originalFunctionNumArgs = 1;
    funcReplacerSetup_1.replaceWithSpec = std::string("newFun(@1)");

    funcReplacerSetup_2.originalFunctionName = std::string("anotherFun");
    funcReplacerSetup_2.originalFunctionNumArgs = 3;
    funcReplacerSetup_2.replaceWithSpec = std::string("bobFun(@3, @1)");
  }
  FunctionReplacerSetup funcReplacerSetup_1;
  FunctionReplacerSetup funcReplacerSetup_2;
};

SUITE(FunctionReplacer_Suite)
{
  TEST(Instantiate)
  {
    FunctionReplacerSetup funcReplacerSetup;
    
    funcReplacerSetup.originalFunctionName = std::string("origFun");
    funcReplacerSetup.originalFunctionNumArgs = 1;
    funcReplacerSetup.replaceWithSpec = std::string("newFun(@1)");

    try {
      FunctionReplacer funcReplacer(funcReplacerSetup);
      CHECK(true);
    } 
    catch(...) {
      CHECK(false);
    }
  }

  TEST_FIXTURE(twoFunctionsFixture, InitQueryChangeQuery)
  {
    /* Init */
    FunctionReplacer funReplacer(funcReplacerSetup_1);

    /* Query */
    int testResult = checkSetupsEqual(funReplacer.getSetup(),
				    funcReplacerSetup_1);
    CHECK_EQUAL(testResult, 0);
  
    /* Change */
    funReplacer.setSetup(funcReplacerSetup_2);
  
    /* Query */
    testResult = checkSetupsEqual(funReplacer.getSetup(),
				  funcReplacerSetup_2);

    CHECK_EQUAL(testResult, 0);
  }
}


int main(int argn, char** argc)
{
  return UnitTest::RunAllTests();
}



/*
int test_doReplace(TestDefinition testDefinition)
{
  FunctionReplacer funcReplacer(testDefinition.functionReplacerSetup);
  
  std::string replaceResult = 
    funcReplacer.doReplace(testDefinition.originalCode);

  if(replaceResult != testDefinition.expectedResultCode)
    {
      return 1;
    }

  return 0;
}

void test_doReplaceHarness()
{
  TestManager testManager("doReplace");
  testManager.doStartAction();

  for(unsigned int srcNum = 1; srcNum <= 5; srcNum++) {

    for(unsigned int setupNum = 1; setupNum <= 7; setupNum++) {

      TestDefinition testDef = buildTestDefinition(srcNum, setupNum);
    
      int testResult = test_doReplace(testDef);

      if(testResult == 1) {
	std::cout << "Source Number " << srcNum;
	std::cout << " Setup Number " << setupNum << std::endl;
	testManager.doFailAction();
      } 
    }
  }
  testManager.doPassAction();
}

*/



