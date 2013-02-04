#include "FunctionArgLocator.h"

FunctionArgLocator::FunctionArgLocator():
  pCharProcessor(NULL),
  positionInFullString(-1),
  bracketLevel(0),
  fullFunctionProcessed(0)
{
  pCharProcessor = new AsciiValueProcessor;
  currentArgInfo.setAllFields(0, 0, std::string());
}

FunctionArgLocator::~FunctionArgLocator()
{
  delete pCharProcessor;
}

void FunctionArgLocator::feed(const char* startChar, const char* endChar)
{

  for(const char* toBeProcessed = startChar; toBeProcessed <= endChar; 
      toBeProcessed++) {
    
    positionInFullString++;

    if(needsMore() == 1) {
      processCharacter(*toBeProcessed);
    }
  }
}

void FunctionArgLocator::processCharacter(char theChar)
{
  charType theCharType = pCharProcessor->process(theChar);
  
  switch(theCharType) {

  case charType_comma:
    doCommaType(theChar);
    break;
  
  case charType_openingBracket:
    doOpeningBracketType(theChar);
    break;
  
  case charType_closingBracket:
    doClosingBracketType(theChar);
    break;
    
  default:
    doOtherType(theChar);
    break;
  }
}

void FunctionArgLocator::doCommaType(char theChar)
{
  if(bracketLevel == 0) {
    storeCurrentArgInfo();
    startNewArgInfoAtNextChar();
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
  arguments.push_back(currentArgInfo);
}

void FunctionArgLocator::startNewArgInfoAtNextChar()
{ 
  currentArgInfo.setAllFields(positionInFullString + 1, 
			      positionInFullString + 1, std::string());
}

void FunctionArgLocator::doOpeningBracketType(char theChar)
{
  bracketLevel++;
  updateCurrentArgInfo(theChar);
}

void FunctionArgLocator::doClosingBracketType(char theChar)
{
  if(bracketLevel == 0) {
    completeProcessing();
    return;
  }

  bracketLevel--;
  updateCurrentArgInfo(theChar);
}

void FunctionArgLocator::completeProcessing()
{
  if(!hasZeroArgs()) {
    storeCurrentArgInfo();
  }

  fullFunctionProcessed = 1;
  closingBracketPosition = positionInFullString;
}

bool FunctionArgLocator::hasZeroArgs()
{
  if(!arguments.empty()) {
    return false;
  }
  
  for(std::string::iterator it = currentArgInfo.arg.begin(); 
      it != currentArgInfo.arg.end(); 
      ++it) {   
    charType cType = pCharProcessor->process(*it);
    
    if(cType != charType_whitespace) {
      return false;
    }
  }
  return true;
}

void FunctionArgLocator::updateCurrentArgInfo(char theChar)
{
  currentArgInfo.arg.push_back(theChar);
  currentArgInfo.endPos = positionInFullString;
}

int FunctionArgLocator::needsMore()
{
  if(fullFunctionProcessed) {
    return 0;
  }
  return 1;
}

unsigned int FunctionArgLocator::getClosingBracketPos()
{
  return closingBracketPosition;
}

void FunctionArgLocator::getLocatedArgs(std::vector<argInfo> &locatedArgs)
{
  locatedArgs = arguments;
}


