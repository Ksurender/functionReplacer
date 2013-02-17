#include "FunctionArgLocator.h"
#include "UnitTest++.h"

#include <string>
#include <vector>

struct singleString_noArgs_fixture
{
  singleString_noArgs_fixture() {
    functionString.assign(")");
    argLocator.feed(functionString.c_str(), 
		    functionString.c_str() + (functionString.size() - 1));
  }

  std::string functionString;
  FunctionArgLocator argLocator;
};

struct instantiateVarsOnly_fixture
{
  instantiateVarsOnly_fixture() {};

  std::string functionString;
  FunctionArgLocator argLocator;
  std::vector<argInfo> locatedArgs;
};

SUITE(FunctionArgLocator_TestSuite)
{
  TEST_FIXTURE(singleString_noArgs_fixture, singleString_noArgs_shouldNotNeedMore)
  {
    CHECK_EQUAL(false, argLocator.needsMore());
  }

  TEST_FIXTURE(singleString_noArgs_fixture, singleString_noArgs_closingBracketPosition)
  {
    CHECK_EQUAL((unsigned int)0, argLocator.getClosingBracketPos());
  }

  TEST_FIXTURE(singleString_noArgs_fixture, singleString_noArgs_locatedArguments)
  {
    std::vector<argInfo> locatedArgs;
    argLocator.getLocatedArgs(locatedArgs);
    CHECK_EQUAL((unsigned int)0, locatedArgs.size());
  }

  TEST_FIXTURE(instantiateVarsOnly_fixture, singleString_oneArg_basic)
  {
    functionString.assign("argN);");
    const char* pStartChar = functionString.c_str();
    const char* pEndChar = pStartChar + (functionString.size() - 1);
    argLocator.feed(pStartChar, pEndChar);
    argLocator.getLocatedArgs(locatedArgs);

    CHECK_EQUAL(false, argLocator.needsMore());
    CHECK_EQUAL((unsigned int)4, argLocator.getClosingBracketPos());
    CHECK_EQUAL((unsigned int)1, locatedArgs.size());
    
    CHECK_EQUAL((unsigned int)0, locatedArgs[0].startPos);
    CHECK_EQUAL((unsigned int)3, locatedArgs[0].endPos);
    CHECK_EQUAL("argN", locatedArgs[0].arg.c_str());
  }

  TEST_FIXTURE(instantiateVarsOnly_fixture, singleString_oneArg_leadTrailSpace)
  {
    functionString.assign(" argN );");
    const char* pStartChar = functionString.c_str();
    const char* pEndChar = pStartChar + (functionString.size() - 1);
    argLocator.feed(pStartChar, pEndChar);
    argLocator.getLocatedArgs(locatedArgs);

    CHECK_EQUAL(false, argLocator.needsMore());
    CHECK_EQUAL((unsigned int)6, argLocator.getClosingBracketPos());
    CHECK_EQUAL((unsigned int)1, locatedArgs.size());
    
    CHECK_EQUAL((unsigned int)0, locatedArgs[0].startPos);
    CHECK_EQUAL((unsigned int)5, locatedArgs[0].endPos);
    CHECK_EQUAL(" argN ", locatedArgs[0].arg.c_str());    
  }

  TEST_FIXTURE(instantiateVarsOnly_fixture, singleString_oneArg_multiBracketed)
  {
    functionString.assign("(int) generate(inputValue));");
    const char* pStartChar = functionString.c_str();
    const char* pEndChar = pStartChar + (functionString.size() - 1);
    argLocator.feed(pStartChar, pEndChar);
    argLocator.getLocatedArgs(locatedArgs);

    CHECK_EQUAL(false, argLocator.needsMore());
    CHECK_EQUAL((unsigned int)26, argLocator.getClosingBracketPos());
    CHECK_EQUAL((unsigned int)1, locatedArgs.size());
    
    CHECK_EQUAL((unsigned int)0, locatedArgs[0].startPos);
    CHECK_EQUAL((unsigned int)25, locatedArgs[0].endPos);
    CHECK_EQUAL("(int) generate(inputValue)", 
		locatedArgs[0].arg.c_str());        
  }

  TEST_FIXTURE(instantiateVarsOnly_fixture, singleString_twoArgs_base)
  {
    functionString.assign("inputArgument1, inputArgument2)");
    const char* pStartChar = functionString.c_str();
    const char* pEndChar = pStartChar + (functionString.size() - 1);
    argLocator.feed(pStartChar, pEndChar);
    argLocator.getLocatedArgs(locatedArgs);

    CHECK_EQUAL(false, argLocator.needsMore());
    CHECK_EQUAL((unsigned int)2, locatedArgs.size());
    CHECK_EQUAL((unsigned int)30, argLocator.getClosingBracketPos());

    CHECK_EQUAL((unsigned int)0, locatedArgs[0].startPos);
    CHECK_EQUAL((unsigned int)13, locatedArgs[0].endPos);
    CHECK_EQUAL("inputArgument1", locatedArgs[0].arg.c_str());     

    CHECK_EQUAL((unsigned int)15, locatedArgs[1].startPos);
    CHECK_EQUAL((unsigned int)29, locatedArgs[1].endPos);
    CHECK_EQUAL(" inputArgument2", locatedArgs[1].arg.c_str());
  }

  TEST_FIXTURE(instantiateVarsOnly_fixture, feedChars_twoArgs_basic)
  {
    functionString.assign("inputArgument1, inputArgument2)");
    const char* pStartChar = functionString.c_str();
    const char* pEndChar = pStartChar + (functionString.size() - 1);

    for(const char* pChar = pStartChar; pChar <= pEndChar; ++pChar) {
      CHECK_EQUAL(true, argLocator.needsMore());
      argLocator.feed(pChar);
    }

    argLocator.getLocatedArgs(locatedArgs);

    CHECK_EQUAL(false, argLocator.needsMore());
    CHECK_EQUAL((unsigned int)2, locatedArgs.size());
    CHECK_EQUAL((unsigned int)30, argLocator.getClosingBracketPos());

    CHECK_EQUAL((unsigned int)0, locatedArgs[0].startPos);
    CHECK_EQUAL((unsigned int)13, locatedArgs[0].endPos);
    CHECK_EQUAL("inputArgument1", locatedArgs[0].arg.c_str());     

    CHECK_EQUAL((unsigned int)15, locatedArgs[1].startPos);
    CHECK_EQUAL((unsigned int)29, locatedArgs[1].endPos);
    CHECK_EQUAL(" inputArgument2", locatedArgs[1].arg.c_str());
    
  }

  TEST_FIXTURE(instantiateVarsOnly_fixture, singleString_twoArgs_multiBracket_commas)
  {
    functionString.assign("(myType) object->fun(var1, var2), UPOP(call(), fill))");

    const char* pStartChar = functionString.c_str();
    const char* pEndChar = pStartChar + (functionString.size() - 1);
    argLocator.feed(pStartChar, pEndChar);
    argLocator.getLocatedArgs(locatedArgs);

    CHECK_EQUAL(false, argLocator.needsMore());
    CHECK_EQUAL((unsigned int)2, locatedArgs.size());
    CHECK_EQUAL((unsigned int)52, argLocator.getClosingBracketPos());

    CHECK_EQUAL((unsigned int)0, locatedArgs[0].startPos);
    CHECK_EQUAL((unsigned int)31, locatedArgs[0].endPos);
    CHECK_EQUAL("(myType) object->fun(var1, var2)", locatedArgs[0].arg.c_str());     

    CHECK_EQUAL((unsigned int)33, locatedArgs[1].startPos);
    CHECK_EQUAL((unsigned int)51, locatedArgs[1].endPos);
    CHECK_EQUAL(" UPOP(call(), fill)", locatedArgs[1].arg.c_str());
  }

  TEST_FIXTURE(instantiateVarsOnly_fixture, feedChars_twoArgs_multiBracket_commas)
  {
    functionString.assign("(myType) object->fun(var1, var2), UPOP(call(), fill))");

    const char* pStartChar = functionString.c_str();
    const char* pEndChar = pStartChar + (functionString.size() - 1);
    
    for(const char* pChar = pStartChar; pChar <= pEndChar; ++pChar) {
      CHECK_EQUAL(true, argLocator.needsMore());
      argLocator.feed(pChar);
    }

    argLocator.getLocatedArgs(locatedArgs);

    CHECK_EQUAL(false, argLocator.needsMore());
    CHECK_EQUAL((unsigned int)2, locatedArgs.size());
    CHECK_EQUAL((unsigned int)52, argLocator.getClosingBracketPos());

    CHECK_EQUAL((unsigned int)0, locatedArgs[0].startPos);
    CHECK_EQUAL((unsigned int)31, locatedArgs[0].endPos);
    CHECK_EQUAL("(myType) object->fun(var1, var2)", locatedArgs[0].arg.c_str());     

    CHECK_EQUAL((unsigned int)33, locatedArgs[1].startPos);
    CHECK_EQUAL((unsigned int)51, locatedArgs[1].endPos);
    CHECK_EQUAL(" UPOP(call(), fill)", locatedArgs[1].arg.c_str());
  }

  TEST_FIXTURE(instantiateVarsOnly_fixture, emptyArgumentResultsInException1)
  {
    functionString.assign(",secondArgument);");
    
    const char* pStartChar = functionString.c_str();
    const char* pEndChar = pStartChar + (functionString.size() - 1);
    
    CHECK_THROW(argLocator.feed(pStartChar, pEndChar), EmptyArgumentException);

    try {
      FunctionArgLocator argLocator2;
      argLocator2.feed(pStartChar, pEndChar);
    }
    catch(EmptyArgumentException& emptyArgException)
    {
      CHECK_EQUAL(emptyArgException.what(),"Empty argument located");
    }
  }

  TEST_FIXTURE(instantiateVarsOnly_fixture, emptyArgumentResultsInException2)
  {
    functionString.assign("firstArgument,,thirdArgument);");
    
    const char* pStartChar = functionString.c_str();
    const char* pEndChar = pStartChar + (functionString.size() - 1);
    
    CHECK_THROW(argLocator.feed(pStartChar, pEndChar), EmptyArgumentException);
  }
}

int main(int argn, char** argv)
{
  return UnitTest::RunAllTests();
}



