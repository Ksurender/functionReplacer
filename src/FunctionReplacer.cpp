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

FunctionReplacerSetup FunctionReplacer::getSetup()
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
  std::string findThis = setup.originalFunctionName + "(";  
  size_t searchPosition = 0;

  while(searchPosition != NOMATCHES && 
	searchPosition < codeBeingProcessed.length()) {
    size_t usagePosition = codeBeingProcessed.find(findThis, searchPosition);
    
    if(usagePosition != NOMATCHES) {
      processSingleUsage(usagePosition);
      searchPosition = usagePosition + 1;
    } 
    else {
      searchPosition = NOMATCHES;
    }
  }

  if(searchPosition != NOMATCHES) {
    std::stringstream errorInfoBuilder;
    errorInfoBuilder << "Error when finding and processing usages of "
		     << setup.originalFunctionName << "." << std::endl;
      
    UnexpectedStringEndReachedException except(errorInfoBuilder.str());  
    throw except;
  }
}

void FunctionReplacer::processSingleUsage(size_t usagePosition)
{
  FunctionArgLocator argLocator = getArgLocatorForUsage(usagePosition);
  
  std::vector<argInfo> locatedArgs;
  argLocator.getLocatedArgs(locatedArgs);
  if(locatedArgs.size() == setup.originalFunctionNumArgs) {
    replaceSingleUsage(argLocator, usagePosition);
  }
}

FunctionArgLocator FunctionReplacer::getArgLocatorForUsage(size_t usagePosition)
{
  FunctionArgLocator argLocator;
  size_t position = usagePosition + setup.originalFunctionName.length() + 1;

  while(argLocator.needsMore() && position < codeBeingProcessed.length()) {
    argLocator.feed(codeBeingProcessed[position]);
    position++;
  }

  if(argLocator.needsMore()) {
    std::stringstream errorInfoBuilder;
    errorInfoBuilder << "Error when locating arguments in usage of "
		     << setup.originalFunctionName
                     << ".  This may have happened because a usage of the "
                        "function may exist with an opening bracket and no "
                        "corresponding closing bracket." << std::endl;

    UnexpectedStringEndReachedException except(errorInfoBuilder.str());  
    throw except;
  }


  return(argLocator);
}

void FunctionReplacer::replaceSingleUsage(const FunctionArgLocator &argLocator, 
				    size_t usagePosition)
{
  std::vector<argInfo> locatedArgs;
  argLocator.getLocatedArgs(locatedArgs);
  std::string replacementString = buildReplacementString(locatedArgs);
  size_t originalFunctionUsageLength = setup.originalFunctionName.length() + 1 + 
                                       (1 + argLocator.getClosingBracketPos());
  codeBeingProcessed.replace(usagePosition, originalFunctionUsageLength, replacementString);
}

std::string FunctionReplacer::buildReplacementString(const std::vector<argInfo> &args)
{
  std::string replacementString(setup.replacementSpec);

  for(unsigned int k = 1; k <= setup.originalFunctionNumArgs; k++) {
    std::stringstream toBeReplacedBuilder;
    toBeReplacedBuilder << '@' << k;
    std::string toBeReplaced(toBeReplacedBuilder.str());

    findAndReplaceAll(replacementString, toBeReplaced, args[k-1].arg);
  }
  return(replacementString);
}

void FunctionReplacer::findAndReplaceAll(std::string &source, const std::string &toBeReplaced, const std::string &replaceWith)
{
  size_t searchPosition = 0;

  while(searchPosition != NOMATCHES &&
	searchPosition < source.length()) {
    size_t usagePosition = source.find(toBeReplaced, searchPosition);

    if(usagePosition != NOMATCHES) {
      source.replace(usagePosition, toBeReplaced.length(), replaceWith);
      searchPosition = usagePosition + 1;
    }
    else {
      searchPosition = NOMATCHES;
    }
  }

  if(searchPosition != NOMATCHES) {
    std::stringstream errorInfoBuilder;
    errorInfoBuilder << "Error when finding and replacing all usages of "
		     << toBeReplaced << "with " << replaceWith << "in "
                     << source << std::endl;
      
    UnexpectedStringEndReachedException except(errorInfoBuilder.str());  
    throw except;
  }
}
