/* functionReplacer.h */
/* functionReplacer is a class designed to replace function
 * call usage in a C++ source file from one usage to 
 * another
 */
#include <string>


struct FunctionReplacerSetup {
  std::string originalFunctionName;
  unsigned int originalFunctionNumArgs;
  std::string replaceWithSpec;
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

  FunctionReplacerSetup setup;

};
