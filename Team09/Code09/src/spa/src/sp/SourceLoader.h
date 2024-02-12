#pragma once
#include <iostream>
#include <string>
#include <vector>

class SourceLoader {
 public:
  /* Scrubs a SIMPLE program to create an array of characters. New line
   * characters are preserved.*/
  std::vector<std::string> loadSimple(std::stringstream& stringStream);
  bool isValidSymbolPair(char key, char value);
  std::string createSymbolString(std::stringstream& stream);
  bool isCharSymbol(char character);

 private:
  char peekNextChar(std::stringstream& stream);
  char getNextChar(std::stringstream& stream);
};
