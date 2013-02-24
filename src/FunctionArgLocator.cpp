#include "FunctionArgLocator.h"

FunctionArgLocator::FunctionArgLocator():
  currentPositionInFullString(-1),
  currentParanthesesLevel(0),
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
  
  case charType_openingParantheses:
    doOpeningParanthesesType(theChar);
    break;
  
  case charType_closingParantheses:
    doClosingParanthesesType(theChar);
    break;
    
  default:
    doOtherType(theChar);
    break;
  }
}

void FunctionArgLocator::doCommaType(char theChar)
{
  if(currentParanthesesLevel == 0) {
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

void FunctionArgLocator::doOpeningParanthesesType(char theChar)
{
  currentParanthesesLevel++;
  updateCurrentArgInfo(theChar);
}

void FunctionArgLocator::doClosingParanthesesType(char theChar)
{
  if(currentParanthesesLevel == 0) {
    completeProcessing();
    return;
  }

  currentParanthesesLevel--;
  updateCurrentArgInfo(theChar);
}

void FunctionArgLocator::completeProcessing()
{
  if(!hasZeroArgs()) {
    storeCurrentArgInfo();
  }

  fullFunctionProcessed = true;
  closingParanthesesPosition = currentPositionInFullString;
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

unsigned int FunctionArgLocator::getClosingParanthesesPos() const
{
  return closingParanthesesPosition;
}

void FunctionArgLocator::getLocatedArgs(std::vector<argInfo> &locatedArgs) const
{
  locatedArgs = arguments;
}


