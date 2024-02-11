#pragma once
#include <LexicalToken.h>

#include <string>

// TODO(ben): Constants are sequence of digits with no leading zero. Validate
// them.
class Integer : public LexicalTokenInterface {
 private:
  std::string name;
};
