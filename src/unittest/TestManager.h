#ifndef TESTMANAGER_H
#define TESTMANAGER_H

#include <assert.h>
#include <iostream>
#include <string>

enum indicatorTypes {
  indicator_start = 0,
  indicator_pass = 1,
  indicator_fail = 2
};

class TestManager 
{
 public:
  
  TestManager(const char* testName);
  void doStartAction();
  void doPassAction();
  void doFailAction();

 private:

  TestManager(); // Don't allow default construction
  void indicate(indicatorTypes type);
  
  std::string testName;
};

#endif
