/* functionReplacer.h */
/* functionReplacer is a class designed to replace function
 * call usage in a C++ source file from one usage to 
 * another
 */
#include <string>
#include <vector>
#include "FunctionArgLocator.h"


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
  FunctionReplacerSetup getSetup();

  std::string doReplace(std::string originalCode);

 private:
  // To prevent the outside world from using default constructor
  FunctionReplacer() {};

  void performReplacements();
  void processSingleUsage(size_t usagePosition);
  FunctionArgLocator getArgLocatorForUsage(size_t usagePosition);
  void replaceSingleUsage(const FunctionArgLocator &argLocator, size_t usagePosition);
  std::string buildReplacementString(const std::vector<argInfo> &args);
  void findAndReplaceAll(std::string &source, const std::string &toBeReplaced, const std::string &replaceWith);

		   
  std::string codeBeingProcessed;
  FunctionReplacerSetup setup;

};
