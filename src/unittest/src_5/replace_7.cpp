#include <string>
#include "myHeader.h"

int myFunction(int arg1, int arg2)
{
  int intermediate = arg1 + arg2;
  int anotherVal = arg1*arg2;
  int functionVal = NS::Replacement(arg1 * arg2)->methodCall(funCall(arg2), funCall());
  return functionVal;
}
