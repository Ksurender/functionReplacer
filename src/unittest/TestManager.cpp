#include "TestManager.h"

TestManager::TestManager(const char* testName)
{
  this->testName.assign(testName);
}

void TestManager::doStartAction()
{
  std::cout << "Starting Test: " << testName << std::endl;
}

void TestManager::doPassAction()
{
  std::cout << "Passed Test: " << testName << std::endl << std::endl;  
}

void TestManager::doFailAction()
{
  std::cout << "Failed Test: " << testName << std::endl << std::endl;
  assert(0);
}

void TestManager::doFailAction(const char* failMessage)
{
  std::cout << "Failed Test: " << testName << std::endl;
  std::cout << failMessage << std::endl << std::endl;
  assert(0);
}
