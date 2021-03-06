#include "FunctionArgLocator.h"

FunctionArgLocator::FunctionArgLocator():
  currentPositionInFullString(-1),
  currentParenthesesLevel(0),
  fullFunctionProcessed(false)
{
  currentArgInfo.setAllFields(0, 0, std::string());
}

FunctionArgLocator::~FunctionArgLocator()
{
}

void FunctionArgLocator::feed(char singleChar)
{
  processCharacter(singleChar);
}

void FunctionArgLocator::feed(const char* startChar, const char* endChar)
{

  for(const char* toBeProcessed = startChar; toBeProcessed <= endChar; 
      toBeProcessed++) {   
    processCharacter(*toBeProcessed);
  }
}

void FunctionArgLocator::processCharacter(char theChar)
{

  currentPositionInFullString++;
  
  if(!needsMore()) {
    return;
  }

  charType theCharType = charProcessor.process(theChar);
  
  switch(theCharType) {

  case charType_comma:
    doCommaType(theChar);
    break;
  
  case charType_openingParentheses:
    doOpeningParenthesesType(theChar);
    break;
  
  case charType_closingParentheses:
    doClosingParenthesesType(theChar);
    break;
    
  default:
    doOtherType(theChar);
    break;
  }
}

void FunctionArgLocator::doCommaType(char theChar)
{
  if(currentParenthesesLevel == 0) {
    storeCurrentArgInfo();
    startNewArgAtNextChar();
    return;
  }

  updateCurrentArgInfo(theChar);
}

void FunctionArgLocator::doOtherType(char theChar)
{
  updateCurrentArgInfo(theChar);
}

void FunctionArgLocator::storeCurrentArgInfo()
{
  if(currentArgInfo.arg.empty()) {
    EmptyArgumentException emptyArgumentException;
    throw emptyArgumentException;
  }

  arguments.push_back(currentArgInfo);
}

void FunctionArgLocator::startNewArgAtNextChar()
{ 
  currentArgInfo.setAllFields(currentPositionInFullString + 1, 
			      currentPositionInFullString + 1, std::string());
}

void FunctionArgLocator::doOpeningParenthesesType(char theChar)
{
  currentParenthesesLevel++;
  updateCurrentArgInfo(theChar);
}

void FunctionArgLocator::doClosingParenthesesType(char theChar)
{
  if(currentParenthesesLevel == 0) {
    completeProcessing();
    return;
  }

  currentParenthesesLevel--;
  updateCurrentArgInfo(theChar);
}

void FunctionArgLocator::completeProcessing()
{
  if(!hasZeroArgs()) {
    storeCurrentArgInfo();
  }

  fullFunctionProcessed = true;
  closingParenthesesPosition = currentPositionInFullString;
}

bool FunctionArgLocator::hasZeroArgs()
{
  if(!arguments.empty()) {
    return false;
  }
  
  for(std::string::iterator it = currentArgInfo.arg.begin(); 
      it != currentArgInfo.arg.end(); 
      ++it) {   
    charType cType = charProcessor.process(*it);
    
    if(cType != charType_whitespace) {
      return false;
    }
  }
  return true;
}

void FunctionArgLocator::updateCurrentArgInfo(char theChar)
{
  currentArgInfo.arg.push_back(theChar);
  currentArgInfo.endPos = currentPositionInFullString;
}

bool FunctionArgLocator::needsMore() const
{
  if(fullFunctionProcessed) {
    return false;
  }
  return true;
}

unsigned int FunctionArgLocator::getClosingParenthesesPos() const
{
  return closingParenthesesPosition;
}

void FunctionArgLocator::getLocatedArgs(std::vector<argInfo> &locatedArgs) const
{
  locatedArgs = arguments;
}


