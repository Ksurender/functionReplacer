// FunctionArgLocator.h
#ifndef FUNCTIONARGLOCATOR_H
#define FUNCTIONARGLOCATOR_H

#include <string>
#include <vector>


struct argInfo
{
  unsigned int startPos;
  unsigned int endPos;
  std::string arg;
};

class FunctionArgLocator
{
 public:
  FunctionArgLocator();
  
  void feed(const char* toBeProcessed);
  int needsMore();
  unsigned int getOpeningBracketPos();
  unsigned int getClosingBracketPos();

  void getLocatedArgs(std::vector<argInfo> &locatedArgs);
  



 private:
  unsigned int bracketLevel;
  unsigned int currentArg;
  int fullFunctionProcessed;
  std::vector<argInfo> arguments;
};

#endif
