
#include <iostream>
#include <memory>

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

    SECTION("Build while then if CFG correctly") {
        std::vector<Token> tokens = {
            Token(PROCEDURE, "procedure", -1),
            Token(NAME, "Second", -1),
            Token(OPEN_CURLY_BRACE, "{", -1),
            Token(NAME, "x", 1),
            Token(EQUAL, "=", 1),
            Token(INTEGER, "0", 1),
            Token(SEMICOLON, ";", 1),
            Token(NAME, "i", 2),
            Token(EQUAL, "=", 2),
            Token(INTEGER, "5", 2),
            Token(SEMICOLON, ";", 2),
            Token(WHILE, "while", 3),
            Token(OPEN_BRACKET, "(", 3),
            Token(NAME, "i", 3),
            Token(NOT_EQUAL_CHECK, "!=", 3),
            Token(INTEGER, "0", 3),
            Token(CLOSE_BRACKET, ")", 3),
            Token(OPEN_CURLY_BRACE, "{", 3),
            Token(NAME, "x", 4),
            Token(EQUAL, "=", 4),
            Token(NAME, "x", 4),
            Token(ADD, "+", 4),
            Token(INTEGER, "2", 4),
            Token(MUL, "*", 4),
            Token(NAME, "y", 4),
            Token(SEMICOLON, ";", 4),
            Token(NAME, "i", 5),
            Token(EQUAL, "=", 5),
            Token(INTEGER, "1", 5),
            Token(SEMICOLON, ";", 5),
            Token(NAME, "i", 6),
            Token(EQUAL, "=", 6),
            Token(NAME, "i", 6),
            Token(SUB, "-", 6),
            Token(INTEGER, "1", 6),
            Token(SEMICOLON, ";", 6),
            Token(CLOSE_CURLY_BRACE, "}", -1),
            Token(IF, "if", 7),
            Token(OPEN_BRACKET, "(", 7),
            Token(NAME, "x", 7),
            Token(EQUAL_CHECK, "==", 7),
            Token(INTEGER, "1", 7),
            Token(CLOSE_BRACKET, ")", 7),
            Token(THEN, "then", 7),
            Token(OPEN_CURLY_BRACE, "{", 7),
            Token(NAME, "x", 8),
            Token(EQUAL, "=", 8),
            Token(NAME, "x", 8),
            Token(ADD, "+", 8),
            Token(INTEGER, "1", 8),
            Token(SEMICOLON, ";", 8),
            Token(CLOSE_CURLY_BRACE, "}", -1),
            Token(ELSE, "else", -1),
            Token(OPEN_CURLY_BRACE, "{", -1),
            Token(NAME, "z", 9),
            Token(EQUAL, "=", 9),
            Token(INTEGER, "1", 9),
            Token(SEMICOLON, ";", 9),
            Token(CLOSE_CURLY_BRACE, "}", -1),
            Token(NAME, "z", 10),
            Token(EQUAL, "=", 10),
            Token(NAME, "z", 10),
            Token(ADD, "+", 10),
            Token(NAME, "x", 10),
            Token(ADD, "+", 10),
            Token(NAME, "i", 10),
            Token(SEMICOLON, ";", 10),
            Token(NAME, "y", 11),
            Token(EQUAL, "=", 11),
            Token(NAME, "z", 11),
            Token(ADD, "+", 11),
            Token(INTEGER, "2", 11),
            Token(SEMICOLON, ";", 11),
            Token(NAME, "x", 12),
            Token(EQUAL, "=", 12),
            Token(NAME, "x", 12),
            Token(MUL, "*", 12),
            Token(NAME, "y", 12),
            Token(ADD, "+", 12),
            Token(NAME, "z", 12),
            Token(SEMICOLON, ";", 12),
            Token(CLOSE_CURLY_BRACE, "}", -1),
        };

        std::shared_ptr<ProgramNode> astRoot = ast.buildAST(tokens);

        for (auto child : astRoot->getChildren()) {
            cfg.buildCFG(child);
        }
        std::unordered_map<int, std::vector<int>> expected = {{1, {2}},  {2, {3}},   {3, {4, 7}}, {4, {5}},
                                                              {5, {6}},  {6, {3}},   {7, {8, 9}}, {8, {10}},
                                                              {9, {10}}, {10, {11}}, {11, {12}}};

        REQUIRE(cfg.parentToChildMap == expected);
    }
}
