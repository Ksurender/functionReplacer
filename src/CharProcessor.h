// Using an interface class to the character processor because
// I ultimately want to use an open source lexer to get tokens.

#ifndef CHARPROCESSOR_H
#define CHARPROCESSOR_H


enum charType {
  charType_comma,
  charType_openingParantheses,
  charType_closingParantheses,
  charType_whitespace,
  charType_other
};

class CharProcessor
{
 public:
  CharProcessor() {};
  virtual ~CharProcessor() {};
  virtual charType process(char theChar) = 0;
  
};

class AsciiValueProcessor : public CharProcessor
{
 public:
  AsciiValueProcessor() {};
  virtual charType process(char theChar);

};

#endif
