#pragma once
#include <LexicalToken.h>

// TODO(ben): perform validation whether it consists of numbers from 1 - 9
class NonZeroDigit : public LexicalTokenInterface {
 private:
  char NonZeroDigit;
};
