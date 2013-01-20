#include <string>
#include "myHeader.h"

int myFunction(int arg1, int arg2)
{
  int intermediate = arg1 + arg2;
  int anotherVal = arg1*arg2;
  int functionVal = NS::Replacement(funCall())->methodCall(arg1 * arg2, funCall(arg2));
  return functionVal;
}
