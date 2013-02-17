#include "FunctionArgLocator.h"

FunctionArgLocator::FunctionArgLocator():
  pCharProcessor(NULL),
  currentPositionInFullString(-1),
  currentBracketLevel(0),
  fullFunctionProcessed(false)
{
  pCharProcessor = new AsciiValueProcessor;
  currentArgInfo.setAllFields(0, 0, std::string());
}

FunctionArgLocator::~FunctionArgLocator()
{
  delete pCharProcessor;
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
  if(currentBracketLevel == 0) {
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

void FunctionArgLocator::doOpeningBracketType(char theChar)
{
  currentBracketLevel++;
  updateCurrentArgInfo(theChar);
}

void FunctionArgLocator::doClosingBracketType(char theChar)
{
  if(currentBracketLevel == 0) {
    completeProcessing();
    return;
  }

  currentBracketLevel--;
  updateCurrentArgInfo(theChar);
}

void FunctionArgLocator::completeProcessing()
{
  if(!hasZeroArgs()) {
    storeCurrentArgInfo();
  }

  fullFunctionProcessed = true;
  closingBracketPosition = currentPositionInFullString;
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
  currentArgInfo.endPos = currentPositionInFullString;
}

bool FunctionArgLocator::needsMore()
{
  if(fullFunctionProcessed) {
    return false;
  }
  return true;
}

unsigned int FunctionArgLocator::getClosingBracketPos()
{
  return closingBracketPosition;
}

void FunctionArgLocator::getLocatedArgs(std::vector<argInfo> &locatedArgs)
{
  locatedArgs = arguments;
}


