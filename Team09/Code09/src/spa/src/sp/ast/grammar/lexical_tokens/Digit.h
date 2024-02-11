#pragma once
#include <LexicalToken.h>

// TODO(ben): perform validation whether it consists of numbers from 0 - 9
class Digit : public LexicalTokenInterface {
 private:
  char digit;

 public:
  bool validate() override { return true; }
};
