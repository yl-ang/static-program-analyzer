#include "SP.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

void SP::processFile(std::string filepath) {
  std::ifstream sourceProgram(filepath);
  std::stringstream buffer;
  buffer << sourceProgram.rdbuf();

  // pass the string stream buffer to the sourceloader
  std::vector<std::string> simpleProgramAsString =
      sourceLoader.loadSimple(buffer);

  std::vector<Token> tokens = tokenizer.tokenize(simpleProgramAsString);
  ASTNode astRoot = ast.buildAST(tokens);

  // add other components
}
