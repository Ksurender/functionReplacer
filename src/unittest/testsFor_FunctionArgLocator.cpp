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

struct singleString_oneArg_fixture
{
  singleString_oneArg_fixture() {};

  std::string functionString;
  FunctionArgLocator argLocator;
  std::vector<argInfo> locatedArgs;
};

SUITE(FunctionArgLocator_TestSuite)
{
  TEST_FIXTURE(singleString_noArgs_fixture, singleString_noArgs_shouldNotNeedMore)
  {
    CHECK_EQUAL(argLocator.needsMore(), 0);
  }

  TEST_FIXTURE(singleString_noArgs_fixture, singleString_noArgs_closingBracketPosition)
  {
    CHECK_EQUAL(argLocator.getClosingBracketPos(), 0);
  }

  TEST_FIXTURE(singleString_noArgs_fixture, singleString_noArgs_locatedArguments)
  {
    std::vector<argInfo> locatedArgs;
    argLocator.getLocatedArgs(locatedArgs);
    CHECK_EQUAL(locatedArgs.size(), 0);
  }

  TEST_FIXTURE(singleString_oneArg_fixture, singleString_oneArg_basic)
  {
    functionString.assign("argN);");
    const char* pStartChar = functionString.c_str();
    const char* pEndChar = pStartChar + (functionString.size() - 1);
    argLocator.feed(pStartChar, pEndChar);
    argLocator.getLocatedArgs(locatedArgs);

    CHECK_EQUAL(0, argLocator.needsMore());
    CHECK_EQUAL(4, argLocator.getClosingBracketPos());
    CHECK_EQUAL(1, locatedArgs.size());
    
    CHECK_EQUAL(0, locatedArgs[0].startPos);
    CHECK_EQUAL(3, locatedArgs[0].endPos);
    CHECK_EQUAL("argN", locatedArgs[0].arg.c_str());
  }

  TEST_FIXTURE(singleString_oneArg_fixture, singleString_oneArg_leadTrailSpace)
  {
    functionString.assign(" argN );");
    const char* pStartChar = functionString.c_str();
    const char* pEndChar = pStartChar + (functionString.size() - 1);
    argLocator.feed(pStartChar, pEndChar);
    argLocator.getLocatedArgs(locatedArgs);

    CHECK_EQUAL(0, argLocator.needsMore());
    CHECK_EQUAL(6, argLocator.getClosingBracketPos());
    CHECK_EQUAL(1, locatedArgs.size());
    
    CHECK_EQUAL(0, locatedArgs[0].startPos);
    CHECK_EQUAL(5, locatedArgs[0].endPos);
    CHECK_EQUAL(" argN ", locatedArgs[0].arg.c_str());    
  }
}

int main(int argn, char** argv)
{
  return UnitTest::RunAllTests();
}



