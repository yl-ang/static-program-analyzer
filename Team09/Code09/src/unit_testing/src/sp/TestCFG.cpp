
#include <fstream>
#include <iostream>
#include <memory>
#include <queue>
#include <sstream>

#include "catch.hpp"
#include "sp/SourceLoader.h"
#include "sp/ast/Ast.h"
#include "sp/ast/AstNode.h"
#include "sp/tokenizer/SpTokenizer.h"
#include "sp/tokenizer/Token.h"

using namespace std;  // NOLINT

std::queue<Token> makeTokenQueue(std::vector<Token> tokens) {
    std::queue<Token> queue;
    for (auto token : tokens) {
        queue.push(token);
    }
    return queue;
}

TEST_CASE("AST Build Tests") {
    AST ast;
    SpTokenizer tokenizer;
    SourceLoader sourceLoader;

    SECTION("Basic CFG built correctly") {
        std::ifstream sourceProgram("Team09/Code09/src/unit_testing/src/sp/sample test files/CFG/Website.txt");
        std::stringstream buffer;
        buffer << sourceProgram.rdbuf();

        // pass the string stream buffer to the sourceloader
        std::vector<std::string> simpleProgramAsString = sourceLoader.loadSimple(buffer);
        std::vector<Token> tokens = tokenizer.tokenize(simpleProgramAsString);
        std::shared_ptr<ProgramNode> astRoot = ast.buildAST(tokens);
    }
}
