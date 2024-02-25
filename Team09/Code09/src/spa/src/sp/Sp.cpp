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

    std::vector<Token> tokens = tokenizer.tokenize(simpleProgramAsString);
    // std::unique_ptr<ASTNode> astRoot = ast.buildAST(tokens);
    // designExtractor.extract(*(astRoot.get()));
    // designExtractor.writePKB(pkbWriter);
}
