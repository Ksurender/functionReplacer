// FunctionArgLocator.h
#ifndef FUNCTIONARGLOCATOR_H
#define FUNCTIONARGLOCATOR_H

#include <string>
#include <vector>
#include "CharProcessor.h"


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
  
  void feed(const char* startChar, const char* endChar);
  int needsMore();
  unsigned int getClosingBracketPos();

  void getLocatedArgs(std::vector<argInfo> &locatedArgs);
  
 private:
  void processCharacter(char theChar);
  void doCommaType(char theChar);
  void doOpeningBracketType(char theChar);
  void doClosingBracketType(char theChar);
  void doOtherType(char theChar);

  void storeCurrentArgInfo();
  void startNewArgInfoAtNextChar();

  void updateCurrentArgInfo(char theChar);

  void completeProcessing();
  bool hasZeroArgs();

  CharProcessor* pCharProcessor;
  int positionInFullString;
  int bracketLevel;
  int fullFunctionProcessed;

  int closingBracketPosition;
  std::vector<argInfo> arguments;
  argInfo currentArgInfo;
};

#endif
