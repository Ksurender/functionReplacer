#include "CharProcessor.h"

#define WHITESPACE_UPPERBOUND 33
#define OPENING_BRACKET 40
#define CLOSING_BRACKET 41
#define COMMA 44

charType AsciiValueProcessor::process(char theChar)
{
  if(theChar < WHITESPACE_UPPERBOUND) {
    return charType_whitespace;
  }

  if(theChar == OPENING_BRACKET) {
    return charType_openingBracket;
  }

  if(theChar == CLOSING_BRACKET) {
    return charType_closingBracket;
  }

  if(theChar == COMMA) {
    return charType_comma;
  }

  return charType_other;
}
