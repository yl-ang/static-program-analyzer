#pragma once
#include <iostream>
#include <string>
#include <vector>

class SourceLoader {
 public:
  /* Scrubs a SIMPLE program to create an array of characters. New line
   * characters are preserved.*/
  std::vector<std::string> LoadSIMPLE(std::string simple_input);
};
