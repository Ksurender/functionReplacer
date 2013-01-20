#include <string>
#include "myHeader.h"

int myFunction(int arg1, int arg2)
{
  int intermediate = arg1 + arg2;
  int anotherVal = arg1*arg2;
  int functionVal = replacement(arg1 * arg2, funCall(arg2), funCall());
  return functionVal;
}
