

#pragma once
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "SourceLoader.h"

/*
This is the entry point for all SIMPLE programs to be parsed
*/
class SP {
 private:
  SourceLoader sourceLoader;

 public:
  void processFile(std::string filepath);
};
