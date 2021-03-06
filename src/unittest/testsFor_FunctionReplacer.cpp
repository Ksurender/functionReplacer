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

  if(funcReplacerSetup_1.replacementSpec !=
     funcReplacerSetup_2.replacementSpec)
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
    funcReplacerSetup_1.replacementSpec = std::string("newFun(@1)");

    funcReplacerSetup_2.originalFunctionName = std::string("anotherFun");
    funcReplacerSetup_2.originalFunctionNumArgs = 3;
    funcReplacerSetup_2.replacementSpec = std::string("bobFun(@3, @1)");
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
    funcReplacerSetup.replacementSpec = std::string("newFun(@1)");

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
  
  TEST(zeroArgumentFunction)
  {
    TestDefinition testDef = buildTestDefinition(2, 1);    

    FunctionReplacer funcReplacer(testDef.functionReplacerSetup);
    std::string replaceResult = 
      funcReplacer.doReplace(testDef.originalCode);

    CHECK_EQUAL(testDef.expectedResultCode.c_str(), replaceResult.c_str());
  }

  TEST(zeroArgumentFunction_noReplaces)
  {
    TestDefinition testDef = buildTestDefinition(2,2);
    
    FunctionReplacer funcReplacer(testDef.functionReplacerSetup);
    std::string replaceResult = 
      funcReplacer.doReplace(testDef.originalCode);

    CHECK_EQUAL(testDef.expectedResultCode.c_str(), replaceResult.c_str());
  }

  TEST(singleArgumentFunction_noReplaces)
  {
    TestDefinition testDef = buildTestDefinition(3,1);

    FunctionReplacer funcReplacer(testDef.functionReplacerSetup);
    std::string replaceResult =
      funcReplacer.doReplace(testDef.originalCode);

    CHECK_EQUAL(testDef.expectedResultCode.c_str(), replaceResult.c_str());
  }

  TEST(singleArgumentFunction_keepArgument)
  {
    TestDefinition testDef = buildTestDefinition(3,2);

    FunctionReplacer funcReplacer(testDef.functionReplacerSetup);
    std::string replaceResult =
      funcReplacer.doReplace(testDef.originalCode);

    CHECK_EQUAL(testDef.expectedResultCode.c_str(), replaceResult.c_str());
  }

  TEST(singleArgumentFunction_removeArgument)
  {
    TestDefinition testDef = buildTestDefinition(3,3);

    FunctionReplacer funcReplacer(testDef.functionReplacerSetup);
    std::string replaceResult =
      funcReplacer.doReplace(testDef.originalCode);

    CHECK_EQUAL(testDef.expectedResultCode.c_str(), replaceResult.c_str());
  }

  TEST(singleArgumentFunction_functionToClassMethod)
  {
    TestDefinition testDef = buildTestDefinition(3,4);

    FunctionReplacer funcReplacer(testDef.functionReplacerSetup);
    std::string replaceResult =
      funcReplacer.doReplace(testDef.originalCode);

    CHECK_EQUAL(testDef.expectedResultCode.c_str(), replaceResult.c_str());
  }

  TEST(threeArgumentFunction)
  {
    TestDefinition testDef = buildTestDefinition(4,5);
    
    FunctionReplacer funcReplacer(testDef.functionReplacerSetup);
    std::string replaceResult =
      funcReplacer.doReplace(testDef.originalCode);

    CHECK_EQUAL(testDef.expectedResultCode.c_str(), replaceResult.c_str());
  }
  
  TEST(threeArgumentFunction_removeOne)
  {
    TestDefinition testDef = buildTestDefinition(4,7);

    FunctionReplacer funcReplacer(testDef.functionReplacerSetup);
    std::string replaceResult =
      funcReplacer.doReplace(testDef.originalCode);

    CHECK_EQUAL(testDef.expectedResultCode.c_str(), replaceResult.c_str());
  }

  TEST(threeArgumentFunction_multiLine)
  {
    TestDefinition testDef = buildTestDefinition(5,5);

    FunctionReplacer funcReplacer(testDef.functionReplacerSetup);
    std::string replaceResult = 
      funcReplacer.doReplace(testDef.originalCode);

    CHECK_EQUAL(testDef.expectedResultCode.c_str(), replaceResult.c_str());
  }

  TEST(ExceptionThrownWhenNoClosingParentheses)
  {
    std::string badCode("unsigned int block = vsort(doStuff + Joseph()");
    FunctionReplacerSetup setup = {"vsort", 1, "changeIt(@1)"};
    FunctionReplacer funcReplacer(setup);

    CHECK_THROW(funcReplacer.doReplace(badCode), 
		UnexpectedStringEndReachedException);
  }

  TEST(ExceptionThrownWhenNoClosingParentheses_errorMessage)
  {
    std::string badCode("unsigned int block = vsort(doStuff + Joseph()");
    FunctionReplacerSetup setup = {"vsort", 1, "changeIt(@1)"};
    FunctionReplacer funcReplacer(setup);
    
    std::stringstream errorMessageBuilder;
    errorMessageBuilder << "Unexpectadly reached end of string. ";
    errorMessageBuilder << "Error when locating arguments in usage of ";
    errorMessageBuilder << setup.originalFunctionName;
    errorMessageBuilder << ".  This may have happened because a usage of the "
                           "function may exist with an opening parentheses and no "
                           "corresponding closing parentheses." << std::endl;
    std::string errorMessage(errorMessageBuilder.str());

    try {
      funcReplacer.doReplace(badCode);
      CHECK(0);
    }
    catch(UnexpectedStringEndReachedException except) {
      CHECK_EQUAL(errorMessage.c_str(), except.what()); 
    }
  }

  TEST(MoreThanNineArgumentsException)
  {
    std::string code("myFunction(inputArgument)");
    FunctionReplacerSetup setup = {"myFunction", 10, "newFunction(@5)"};
    FunctionReplacer funcReplacer(setup);

    CHECK_THROW(funcReplacer.doReplace(code), MoreThanNineArgumentsException);
  }
}

int main(int argn, char** argc)
{
  return UnitTest::RunAllTests();
}
