#include <sstream>
#include "FunctionReplacer.h"

#define NOMATCHES std::string::npos

FunctionReplacer::FunctionReplacer(FunctionReplacerSetup initialSetup) :
  setup(initialSetup)
{
}

void FunctionReplacer::setSetup(FunctionReplacerSetup newSetup)
{
  setup = newSetup;
}

FunctionReplacerSetup FunctionReplacer::getSetup() const
{
  return setup;
}

std::string FunctionReplacer::doReplace(std::string originalCode)
{
  
  codeBeingProcessed = originalCode;
  performReplacements();

  return codeBeingProcessed;
}

void FunctionReplacer::performReplacements()
{
  std::string functionUsagePrefix = setup.originalFunctionName + "(";  
  size_t searchStartPosition = 0;

  while(searchStartPosition != NOMATCHES && 
	searchStartPosition < codeBeingProcessed.length()) {
    size_t usageStartPosition = codeBeingProcessed.find(functionUsagePrefix, searchStartPosition);
    
    if(usageStartPosition != NOMATCHES) {
      processSingleUsage(usageStartPosition);
      searchStartPosition = usageStartPosition + 1;
    } 
    else {
      searchStartPosition = NOMATCHES;
    }
  }

  if(searchStartPosition != NOMATCHES) {
    std::stringstream errorInfoBuilder;
    errorInfoBuilder << "Error when finding and processing usages of "
		     << setup.originalFunctionName << "." << std::endl;
      
    UnexpectedStringEndReachedException except(errorInfoBuilder.str());  
    throw except;
  }
}

void FunctionReplacer::processSingleUsage(size_t usageStartPosition)
{
  FunctionArgLocator argLocator = getArgLocatorForUsage(usageStartPosition);
  
  std::vector<argInfo> locatedArgs;
  argLocator.getLocatedArgs(locatedArgs);
  if(locatedArgs.size() == setup.originalFunctionNumArgs) {
    replaceSingleUsage(argLocator, usageStartPosition);
  }
}

FunctionArgLocator FunctionReplacer::getArgLocatorForUsage(size_t usageStartPosition)
{
  FunctionArgLocator argLocator;
  size_t argsPosition = getPositionAfterOpeningParantheses(usageStartPosition);

  while(argLocator.needsMore() && argsPosition < codeBeingProcessed.length()) {
    argLocator.feed(codeBeingProcessed[argsPosition]);
    argsPosition++;
  }

  if(argLocator.needsMore()) {
    std::stringstream errorInfoBuilder;
    errorInfoBuilder << "Error when locating arguments in usage of "
		     << setup.originalFunctionName
                     << ".  This may have happened because a usage of the "
                        "function may exist with an opening parantheses and no "
                        "corresponding closing parantheses." << std::endl;

    UnexpectedStringEndReachedException except(errorInfoBuilder.str());  
    throw except;
  }

  return(argLocator);
}

size_t FunctionReplacer::getPositionAfterOpeningParantheses(size_t usageStartPosition)
{
  return usageStartPosition + setup.originalFunctionName.length() + 1;
}


void FunctionReplacer::replaceSingleUsage(const FunctionArgLocator &argLocator, 
				    size_t usageStartPosition)
{
  std::vector<argInfo> locatedArgs;
  argLocator.getLocatedArgs(locatedArgs);
  std::string replacementString = buildReplacementString(locatedArgs);
  size_t originalFunctionUsageLength = setup.originalFunctionName.length() + 1 + 
                                       (1 + argLocator.getClosingParanthesesPos());
  codeBeingProcessed.replace(usageStartPosition, originalFunctionUsageLength, replacementString);
}

std::string FunctionReplacer::buildReplacementString(const std::vector<argInfo> &args)
{
  std::string replacementString(setup.replacementSpec);

  for(unsigned int argNum = 1; argNum <= setup.originalFunctionNumArgs; argNum++) {
    std::stringstream toBeReplacedBuilder;
    toBeReplacedBuilder << '@' << argNum;
    std::string toBeReplaced(toBeReplacedBuilder.str());

    findAndReplaceAll(replacementString, toBeReplaced, args[argNum-1].arg);
  }
  return(replacementString);
}

void FunctionReplacer::findAndReplaceAll(std::string &source, const std::string &toBeReplaced, const std::string &replaceWith)
{
  size_t searchStartPosition = 0;

  while(searchStartPosition != NOMATCHES &&
	searchStartPosition < source.length()) {
    size_t usageStartPosition = source.find(toBeReplaced, searchStartPosition);

    if(usageStartPosition != NOMATCHES) {
      source.replace(usageStartPosition, toBeReplaced.length(), replaceWith);
      searchStartPosition = usageStartPosition + 1;
    }
    else {
      searchStartPosition = NOMATCHES;
    }
  }

  if(searchStartPosition != NOMATCHES) {
    std::stringstream errorInfoBuilder;
    errorInfoBuilder << "Error when finding and replacing all usages of "
		     << toBeReplaced << "with " << replaceWith << "in "
                     << source << std::endl;
      
    UnexpectedStringEndReachedException except(errorInfoBuilder.str());  
    throw except;
  }
}
