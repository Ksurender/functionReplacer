/* testsFor_functionReplacer.cpp */
#include "FunctionReplacer.h"
#include "testsFor_FunctionReplacer.h"

#include <iostream>
#include <sstream>

void test_instantiate()
{
  TestManager testManager("instantiate");
  testManager.doStartAction();
  
  FunctionReplacerSetup funcReplacerSetup;
  funcReplacerSetup.originalFunctionName = std::string("origFun"),
    funcReplacerSetup.originalFunctionNumArgs = 1;
  funcReplacerSetup.replaceWithSpec = std::string("newFun(@1)");
  
  FunctionReplacer funcReplacer(funcReplacerSetup);

  testManager.doPassAction();
}

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


void test_initializeQueryChangeQuery()
{
  TestManager testManager("initQueryChangeQuery");
  testManager.doStartAction();
  
  FunctionReplacerSetup funcReplacerSetup_1, funcReplacerSetup_2;
  
  funcReplacerSetup_1.originalFunctionName = std::string("origFun1");
  funcReplacerSetup_1.originalFunctionNumArgs = 1;
  funcReplacerSetup_1.replaceWithSpec = std::string("newFun(@1)");

  funcReplacerSetup_2.originalFunctionName = std::string("anotherFun");
  funcReplacerSetup_2.originalFunctionNumArgs = 3;
  funcReplacerSetup_2.replaceWithSpec = std::string("bobFun(@3, @1)");

  /* Init */
  FunctionReplacer funReplacer(funcReplacerSetup_1);

  /* Query */
  int testResult = checkSetupsEqual(funReplacer.getSetup(),
				    funcReplacerSetup_1);
  if(testResult == 1)
    {
      testManager.doFailAction();
      return;
    }
  
  /* Change */
  funReplacer.setSetup(funcReplacerSetup_2);
  
  /* Query */
  testResult = checkSetupsEqual(funReplacer.getSetup(),
				funcReplacerSetup_2);

  if(testResult == 1)
    {
      testManager.doFailAction();
      return;
    }

  testManager.doPassAction();
}

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

int main(int argn, char** argc)
{
  test_instantiate();
  test_initializeQueryChangeQuery();
  test_doReplaceHarness();
}


