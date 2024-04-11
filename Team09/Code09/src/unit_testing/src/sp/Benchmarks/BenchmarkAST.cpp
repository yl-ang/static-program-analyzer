

#include "catch.hpp"
#include "sp/ast/Ast.h"
#include "sp/tokenizer/Token.h"

TEST_CASE("Benchmark AST building (if-then-else)") {
    AST ast;

    int counter = 0;
    std::vector<Token> inputTokenArray = {
        Token(LEXICAL_TOKEN_TYPE::PROCEDURE, "procedure", 0),
        Token(LEXICAL_TOKEN_TYPE::NAME, "a", 0),
        Token(LEXICAL_TOKEN_TYPE::OPEN_CURLY_BRACE, "{", 0),
        Token(LEXICAL_TOKEN_TYPE::IF, "if", 0),
        Token(LEXICAL_TOKEN_TYPE::OPEN_BRACKET, "(", 0),
        Token(LEXICAL_TOKEN_TYPE::OPEN_BRACKET, "(", 0),
        Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0),
        Token(LEXICAL_TOKEN_TYPE::LESS_THAN, "<", 0),
        Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0),

        Token(LEXICAL_TOKEN_TYPE::CLOSE_BRACKET, ")", 0),
        Token(LEXICAL_TOKEN_TYPE::ANDAND, "&&", 0),
        Token(LEXICAL_TOKEN_TYPE::OPEN_BRACKET, "(", 0),
        Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0),
        Token(LEXICAL_TOKEN_TYPE::GREATER_THAN, ">", 0),
        Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0),

        Token(LEXICAL_TOKEN_TYPE::CLOSE_BRACKET, ")", 0),
        Token(LEXICAL_TOKEN_TYPE::CLOSE_BRACKET, ")", 0),
        Token(LEXICAL_TOKEN_TYPE::THEN, "then", 0),
        Token(LEXICAL_TOKEN_TYPE::OPEN_CURLY_BRACE, "{", 0),
        Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0),
        Token(LEXICAL_TOKEN_TYPE::EQUAL, "=", 0),
        Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0),
        Token(LEXICAL_TOKEN_TYPE::SEMICOLON, ";", 1),
        Token(LEXICAL_TOKEN_TYPE::CLOSE_CURLY_BRACE, "}", 1),
        Token(LEXICAL_TOKEN_TYPE::ELSE, "else", 0),
        Token(LEXICAL_TOKEN_TYPE::OPEN_CURLY_BRACE, "{", 0),
        Token(LEXICAL_TOKEN_TYPE::NAME, "a", 0),
        Token(LEXICAL_TOKEN_TYPE::EQUAL, "=", 0),
        Token(LEXICAL_TOKEN_TYPE::NAME, "b", 0),
        Token(LEXICAL_TOKEN_TYPE::SEMICOLON, ";", 1),
        Token(LEXICAL_TOKEN_TYPE::CLOSE_CURLY_BRACE, "}", 1),
        Token(LEXICAL_TOKEN_TYPE::CLOSE_CURLY_BRACE, "}", 0),
    };

    BENCHMARK("Building queue") {
        std::queue<Token> queue;
        for (auto token : inputTokenArray) {
            queue.push(token);
        }
        return queue;
    };

    BENCHMARK("If-then-else") {
        return ast.buildAST(inputTokenArray);
    };
}
