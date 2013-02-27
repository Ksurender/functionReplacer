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
  bool needsMore() const;
  unsigned int getClosingParenthesesPos() const;

  void getLocatedArgs(std::vector<argInfo> &locatedArgs) const;
  
 private:
  void processCharacter(char theChar);
  void doCommaType(char theChar);
  void doOpeningParenthesesType(char theChar);
  void doClosingParenthesesType(char theChar);
  void doOtherType(char theChar);

  void storeCurrentArgInfo();
  void startNewArgAtNextChar();

  void updateCurrentArgInfo(char theChar);

  void completeProcessing();
  bool hasZeroArgs();

  AsciiValueProcessor charProcessor;
  int currentPositionInFullString;
  int currentParenthesesLevel;
  bool fullFunctionProcessed;

  int closingParenthesesPosition;
  std::vector<argInfo> arguments;
  argInfo currentArgInfo;
};

#endif
