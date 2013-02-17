// FunctionArgLocator.h
#ifndef FUNCTIONARGLOCATOR_H
#define FUNCTIONARGLOCATOR_H

#include <exception>
#include <string>
#include <vector>
#include "CharProcessor.h"

class EmptyArgumentException : public std::exception
{
 public:
  virtual const char* what() const throw()
  {
    return "Empty argument located";
  }
};

struct argInfo
{
  unsigned int startPos;
  unsigned int endPos;
  std::string arg;

  void setAllFields(unsigned int theStartPos, unsigned int theEndPos, 
		    std::string theArg)
  {
    startPos = theStartPos;
    endPos = theEndPos;
    arg = theArg;
  }
};

class FunctionArgLocator
{
 public:
  FunctionArgLocator();
  ~FunctionArgLocator();
  
  void feed(char singleChar);
  void feed(const char* startChar, const char* endChar);
  bool needsMore();
  unsigned int getClosingBracketPos();

  void getLocatedArgs(std::vector<argInfo> &locatedArgs);
  
 private:
  void processCharacter(char theChar);
  void doCommaType(char theChar);
  void doOpeningBracketType(char theChar);
  void doClosingBracketType(char theChar);
  void doOtherType(char theChar);

  void storeCurrentArgInfo();
  void startNewArgAtNextChar();

  void updateCurrentArgInfo(char theChar);

  void completeProcessing();
  bool hasZeroArgs();

  CharProcessor* pCharProcessor;
  int currentPositionInFullString;
  int currentBracketLevel;
  bool fullFunctionProcessed;

  int closingBracketPosition;
  std::vector<argInfo> arguments;
  argInfo currentArgInfo;
};

#endif
