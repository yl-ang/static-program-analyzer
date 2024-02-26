#include "Sp.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

void SP::processFile(std::string filepath) {
    std::ifstream sourceProgram(filepath);
    std::stringstream buffer;
    buffer << sourceProgram.rdbuf();

    // pass the string stream buffer to the sourceloader
    std::vector<std::string> simpleProgramAsString = sourceLoader.loadSimple(buffer);

    try {
        std::vector<Token> tokens = tokenizer.tokenize(simpleProgramAsString);
        std::unique_ptr<ProgramNode> astRoot = ast.buildAST(tokens);
        designExtractor.extract(std::move(astRoot));
        designExtractor.writePKB(&pkbWriter);
    } catch (SyntaxError& e) {
        std::cerr << "Syntax Error" << std::endl;
        std::cerr << e.what() << std::endl;
    }
}
