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
  
  unsigned int feed(std::string toBeProcessed);
  

 private:
  unsigned int bracketLevel;
  unsigned int currentArg;
  std::vector<argInfo> arguments;
};

#endif
