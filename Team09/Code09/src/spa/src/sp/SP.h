

#pragma once
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "SourceLoader.h"
#include "sp/ast/Ast.h"
#include "sp/tokenizer/Tokenizer.h"

/*
This is the entry point for all SIMPLE programs to be parsed
*/
class SP {
 private:
  SourceLoader sourceLoader;
  Tokenizer tokenizer;
  AST ast;

 public:
  void processFile(std::string filepath);
};
