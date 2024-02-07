#include "SourceLoader.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> SourceLoader::LoadSIMPLE(std::string simple_input) {
  std::stringstream stringStream(simple_input);
  std::vector<std::string> processed_input;
  std::string line;
  const std::string emptyString = "";
  // loop through each line delimited by the new line character
  while (std::getline(stringStream, line, '\n')) {
    std::stringstream lineStream(line);
    std::string word;
    std::vector<std::string> processed_line;
    while (lineStream >> word) {
      if (word != emptyString) {
        processed_input.push_back(word);
      }
    }
    processed_input.push_back("\n");
  }
  // there will be an extra new line at the end so we remove that
  processed_input.pop_back();
  return processed_input;
}
