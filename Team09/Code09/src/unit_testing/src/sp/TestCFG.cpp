
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>

#include "catch.hpp"
#include "sp/SourceLoader.h"
#include "sp/ast/Ast.h"
#include "sp/cfg/Cfg.h"
#include "sp/tokenizer/SpTokenizer.h"
#include "sp/tokenizer/Token.h"
#include "utils.h"

using namespace std;  // NOLINT

TEST_CASE("CFG Build Tests") {
    AST ast;
    SpTokenizer tokenizer;
    SourceLoader sourceLoader;
    CFG cfg;

    SECTION("Basic CFG built correctly") {
        ifstream sourceProgram("../../../src/unit_testing/src/sp/test_files/CFG/Website.txt");
        std::stringstream buffer;
        buffer << sourceProgram.rdbuf();

        // pass the string stream buffer to the sourceloader
        std::vector<std::string> simpleProgramAsString = sourceLoader.loadSimple(buffer);
        std::vector<Token> tokens = tokenizer.tokenize(simpleProgramAsString);
        std::shared_ptr<ProgramNode> astRoot = ast.buildAST(tokens);

        for (auto child : astRoot->getChildren()) {
            cfg.buildCFG(child);
        }
        std::unordered_map<int, std::vector<int>> expected = {{1, {2}}, {2, {3}}, {3, {4, 7}}, {4, {5}},  {5, {6}},
                                                              {6, {3}}, {7, {8}}, {8, {10}},   {9, {10}}, {10, {11}}};

        REQUIRE(cfg.parentToChildMap == expected);
    }
}
