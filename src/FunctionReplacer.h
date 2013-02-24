/* functionReplacer.h */
/* functionReplacer is a class designed to replace function
 * call usage in a C++ source file from one usage to 
 * another
 */

#ifndef FUNCTIONREPLACER_H
#define FUNCTIONREPLACER_H

#include <exception>
#include <string>
#include <vector>
#include "FunctionArgLocator.h"

class UnexpectedStringEndReachedException : public std::exception
{
 public:

  UnexpectedStringEndReachedException() : message("Unexpectadly reached end of string. ")
  {
  }

  UnexpectedStringEndReachedException(std::string additionalInfo) : message("Unexpectadly reached end of string. ") 
  {
    message.append(additionalInfo);
  }
  ~UnexpectedStringEndReachedException() throw(){};

  virtual const char* what() const throw()
  {
    return message.c_str();
  }

private:
  std::string message;
};

struct FunctionReplacerSetup {
  std::string originalFunctionName;
  unsigned int originalFunctionNumArgs;
  std::string replacementSpec;
};


class FunctionReplacer
{
 public:
  FunctionReplacer(FunctionReplacerSetup initialSetup);
  
  void setSetup(FunctionReplacerSetup newSetup);
  FunctionReplacerSetup getSetup() const;

  std::string doReplace(std::string originalCode);

 private:
  // To prevent the outside world from using default constructor
  FunctionReplacer() {};

  void performReplacements();
  void processSingleUsage(size_t usageStartPosition);
  size_t getPositionAfterOpeningParantheses(size_t usageStartPosition);
  FunctionArgLocator getArgLocatorForUsage(size_t usagePosition);
  void replaceSingleUsage(const FunctionArgLocator &argLocator, size_t usagePosition);
  std::string buildReplacementString(const std::vector<argInfo> &args);
  void findAndReplaceAll(std::string &source, const std::string &toBeReplaced, const std::string &replaceWith);
		   
  std::string codeBeingProcessed;
  FunctionReplacerSetup setup;

};

#endif
