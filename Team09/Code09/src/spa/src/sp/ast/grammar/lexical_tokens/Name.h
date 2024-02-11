#pragma once
#include <LexicalToken.h>

#include <string>

// TODO(ben): procedure names and variables are strings of letters, and
// digits, starting with a letter. Validate them.
class Name : public LexicalTokenInterface {
 private:
  std::string name;
};
