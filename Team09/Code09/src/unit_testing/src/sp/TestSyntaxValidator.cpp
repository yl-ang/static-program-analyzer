#include "catch.hpp"
#include "sp/tokenizer/SyntaxValidator.h"

TEST_CASE("Syntax Validator Tests") {
    SyntaxValidator syntaxValidator;

    SECTION("syntax") {
        std::vector<Token> input = {Token(PROCEDURE, "procedure", -1),
                                    Token(NAME, "proc1", -1),
                                    Token(OPEN_CURLY_BRACE, "{", -1),

                                    Token(IF, "if", 1),
                                    Token(OPEN_BRACKET, "(", 1),
                                    Token(OPEN_BRACKET, "(", 1),
                                    Token(OPEN_BRACKET, "(", 1),
                                    Token(NAME, "x", 1),
                                    Token(GREATER_THAN, ">", 1),
                                    Token(INTEGER, "1", 1),
                                    Token(CLOSE_BRACKET, ")", 1),
                                    Token(ANDAND, "&&", 1),
                                    Token(OPEN_BRACKET, "(", 1),
                                    Token(NAME, "x", 1),
                                    Token(LESS_THAN, "<", 1),
                                    Token(INTEGER, "10", 1),
                                    Token(CLOSE_BRACKET, ")", 1),
                                    Token(CLOSE_BRACKET, ")", 1),
                                    Token(OR, "||", 1),
                                    Token(OPEN_BRACKET, "(", 1),
                                    Token(NOT, "!", 1),
                                    Token(OPEN_BRACKET, "(", 1),
                                    Token(NAME, "y", 1),
                                    Token(EQUAL_CHECK, "==", 1),
                                    Token(NAME, "x", 1),
                                    Token(CLOSE_BRACKET, ")", 1),
                                    Token(CLOSE_BRACKET, ")", 1),
                                    Token(CLOSE_BRACKET, ")", 1),
                                    Token(THEN, "then", 1),
                                    Token(OPEN_CURLY_BRACE, "{", 1),

                                    Token(PRINT, "print", 2),
                                    Token(NAME, "y", 2),
                                    Token(SEMICOLON, ";", 2),

                                    Token(READ, "read", 3),
                                    Token(NAME, "x", 3),
                                    Token(SEMICOLON, ";", 3),

                                    Token(CLOSE_CURLY_BRACE, "}", -1),
                                    Token(ELSE, "else", -1),
                                    Token(OPEN_CURLY_BRACE, "{", -1),

                                    Token(WHILE, "while", 4),
                                    Token(OPEN_BRACKET, "(", 4),
                                    Token(NAME, "y", 4),
                                    Token(EQUAL_CHECK, "==", 4),
                                    Token(OPEN_BRACKET, "(", 4),
                                    Token(INTEGER, "1", 4),
                                    Token(ADD, "+", 4),
                                    Token(OPEN_BRACKET, "(", 4),
                                    Token(INTEGER, "3", 4),
                                    Token(SUB, "-", 4),
                                    Token(INTEGER, "7", 4),
                                    Token(CLOSE_BRACKET, ")", 4),
                                    Token(CLOSE_BRACKET, ")", 4),
                                    Token(CLOSE_BRACKET, ")", 4),
                                    Token(OPEN_CURLY_BRACE, "{", 4),

                                    Token(NAME, "z", 5),
                                    Token(EQUAL, "=", 5),
                                    Token(INTEGER, "5", 5),
                                    Token(MUL, "*", 5),
                                    Token(OPEN_BRACKET, "(", 5),
                                    Token(INTEGER, "7", 5),
                                    Token(ADD, "+", 5),
                                    Token(INTEGER, "9", 5),
                                    Token(CLOSE_BRACKET, ")", 5),
                                    Token(SEMICOLON, ";", 5),

                                    Token(CLOSE_CURLY_BRACE, "}", -1),
                                    Token(CLOSE_CURLY_BRACE, "}", -1),
                                    Token(CLOSE_CURLY_BRACE, "}", -1)};

        bool result = syntaxValidator.validateSyntax(input);
        REQUIRE(result);
    }
}
