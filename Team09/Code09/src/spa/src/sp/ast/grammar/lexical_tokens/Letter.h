#pragma once
#include <LexicalToken.h>

// TODO(ben): perform validation whether it consists of lowercase and
// uppercase letters
class Letter : public LexicalTokenInterface {
 private:
  char letter;

 public:
  bool validate() override { return true; }
};
