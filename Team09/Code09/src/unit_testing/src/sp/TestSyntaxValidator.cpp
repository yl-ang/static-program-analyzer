#include "catch.hpp"
#include "sp/tokenizer/SyntaxValidator.h"

TEST_CASE("Syntax Validator Tests") {
    SyntaxValidator syntaxValidator;

    SECTION("Big test input") {
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

    SECTION("Handle correct PROCEDURE") {
        std::vector<Token> tokens = {Token(PROCEDURE, "procedure", -1),
                                     Token(NAME, "example", -1),
                                     Token(OPEN_CURLY_BRACE, "{", -1),
                                     Token(NAME, "x", 1),
                                     Token(EQUAL, "=", 1),
                                     Token(INTEGER, "1", 1),
                                     Token(SEMICOLON, ";", 1),
                                     Token(CLOSE_CURLY_BRACE, "}", -1)};
        bool result = syntaxValidator.validateSyntax(tokens);
        REQUIRE(result);
    }

    SECTION("Handle incorrect PROCEDURE") {
        // Missing close curly brace token
        std::vector<Token> tokens = {Token(PROCEDURE, "procedure", -1),
                                     Token(NAME, "example", -1),
                                     Token(OPEN_CURLY_BRACE, "{", -1),
                                     Token(NAME, "x", 1),
                                     Token(EQUAL, "=", 1),
                                     Token(INTEGER, "1", 1),
                                     Token(SEMICOLON, ";", 1)};
        REQUIRE_THROWS_AS(syntaxValidator.validateSyntax(tokens), SyntaxError);
    }

    SECTION("Handle correct ASSIGN STATEMENT") {
        std::vector<Token> tokens = {Token(PROCEDURE, "procedure", -1),
                                     Token(NAME, "example", -1),
                                     Token(OPEN_CURLY_BRACE, "{", -1),
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
                                     Token(CLOSE_CURLY_BRACE, "}", -1)};
        bool result = syntaxValidator.validateSyntax(tokens);
        REQUIRE(result);
    }

    SECTION("Handle incorrect ASSIGN STATEMENT") {
        // Missing equal token
        std::vector<Token> tokens = {Token(PROCEDURE, "procedure", -1),
                                     Token(NAME, "example", -1),
                                     Token(OPEN_CURLY_BRACE, "{", -1),
                                     Token(NAME, "z", 5),
                                     Token(INTEGER, "5", 5),
                                     Token(MUL, "*", 5),
                                     Token(OPEN_BRACKET, "(", 5),
                                     Token(INTEGER, "7", 5),
                                     Token(ADD, "+", 5),
                                     Token(INTEGER, "9", 5),
                                     Token(CLOSE_BRACKET, ")", 5),
                                     Token(SEMICOLON, ";", 5),
                                     Token(CLOSE_CURLY_BRACE, "}", -1)};
        REQUIRE_THROWS_AS(syntaxValidator.validateSyntax(tokens), SyntaxError);
    }

    SECTION("Handle correct IF STATEMENT") {
        std::vector<Token> tokens = {Token(PROCEDURE, "procedure", -1),
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

                                     Token(READ, "read", 3),
                                     Token(NAME, "x", 3),
                                     Token(SEMICOLON, ";", 3),

                                     Token(CLOSE_CURLY_BRACE, "}", -1),
                                     Token(ELSE, "else", -1),
                                     Token(OPEN_CURLY_BRACE, "{", -1),

                                     Token(PRINT, "print", 2),
                                     Token(NAME, "y", 2),
                                     Token(SEMICOLON, ";", 2),

                                     Token(CLOSE_CURLY_BRACE, "}", -1),
                                     Token(CLOSE_CURLY_BRACE, "}", -1)};
        bool result = syntaxValidator.validateSyntax(tokens);
        REQUIRE(result);
    }

    SECTION("Handle incorrect IF STATEMENT") {
        // Missing open curly brace after then
        std::vector<Token> tokens = {Token(PROCEDURE, "procedure", -1),
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

                                     Token(READ, "read", 3),
                                     Token(NAME, "x", 3),
                                     Token(SEMICOLON, ";", 3),

                                     Token(CLOSE_CURLY_BRACE, "}", -1),
                                     Token(ELSE, "else", -1),
                                     Token(OPEN_CURLY_BRACE, "{", -1),

                                     Token(PRINT, "print", 2),
                                     Token(NAME, "y", 2),
                                     Token(SEMICOLON, ";", 2),

                                     Token(CLOSE_CURLY_BRACE, "}", -1),
                                     Token(CLOSE_CURLY_BRACE, "}", -1)};
        REQUIRE_THROWS_AS(syntaxValidator.validateSyntax(tokens), SyntaxError);
    }

    SECTION("Handle correct cond expr") {
        /*
         if ( (x + 2) > 1) then {
            read x;
        } else {
            print y;
        }
        */
        std::vector<Token> tokens = {Token(PROCEDURE, "procedure", -1),
                                     Token(NAME, "procedure", -1),
                                     Token(OPEN_CURLY_BRACE, "{", -1),

                                     Token(IF, "if", 1),
                                     Token(OPEN_BRACKET, "(", 1),
                                     Token(OPEN_BRACKET, "(", 1),
                                     Token(NAME, "x", 1),
                                     Token(ADD, "+", 1),
                                     Token(INTEGER, "2", 1),
                                     Token(CLOSE_BRACKET, ")", 1),
                                     Token(GREATER_THAN, ">", 1),
                                     Token(INTEGER, "1", 1),
                                     Token(CLOSE_BRACKET, ")", 1),

                                     Token(THEN, "then", 1),
                                     Token(OPEN_CURLY_BRACE, "{", 1),

                                     Token(READ, "read", 3),
                                     Token(NAME, "x", 3),
                                     Token(SEMICOLON, ";", 3),

                                     Token(CLOSE_CURLY_BRACE, "}", -1),
                                     Token(ELSE, "else", -1),
                                     Token(OPEN_CURLY_BRACE, "{", -1),

                                     Token(PRINT, "print", 2),
                                     Token(NAME, "y", 2),
                                     Token(SEMICOLON, ";", 2),
                                     Token(CLOSE_CURLY_BRACE, "}", -1),
                                     Token(CLOSE_CURLY_BRACE, "}", -1)};

        bool result = syntaxValidator.validateSyntax(tokens);
        REQUIRE(result);
    }

    SECTION("Handle correct cond expr 2") {
        /*
         if ( 1 > (x+2)) then {
            read x;
        } else {
            print y;
        }
        */
        std::vector<Token> tokens = {Token(PROCEDURE, "procedure", -1),
                                     Token(NAME, "procedure", -1),
                                     Token(OPEN_CURLY_BRACE, "{", -1),

                                     Token(IF, "if", 1),
                                     Token(OPEN_BRACKET, "(", 1),
                                     Token(INTEGER, "1", 1),
                                     Token(GREATER_THAN, ">", 1),
                                     Token(OPEN_BRACKET, "(", 1),
                                     Token(NAME, "x", 1),
                                     Token(ADD, "+", 1),
                                     Token(INTEGER, "2", 1),
                                     Token(CLOSE_BRACKET, ")", 1),
                                     Token(CLOSE_BRACKET, ")", 1),

                                     Token(THEN, "then", 1),
                                     Token(OPEN_CURLY_BRACE, "{", 1),

                                     Token(READ, "read", 3),
                                     Token(NAME, "x", 3),
                                     Token(SEMICOLON, ";", 3),

                                     Token(CLOSE_CURLY_BRACE, "}", -1),
                                     Token(ELSE, "else", -1),
                                     Token(OPEN_CURLY_BRACE, "{", -1),

                                     Token(PRINT, "print", 2),
                                     Token(NAME, "y", 2),
                                     Token(SEMICOLON, ";", 2),
                                     Token(CLOSE_CURLY_BRACE, "}", -1),
                                     Token(CLOSE_CURLY_BRACE, "}", -1)};

        bool result = syntaxValidator.validateSyntax(tokens);
        REQUIRE(result);
    }

    SECTION("Handle correct WHILE STATEMENT 2") {
        std::vector<Token> tokens = {Token(PROCEDURE, "procedure", -1), Token(NAME, "proc1", -1),
                                     Token(OPEN_CURLY_BRACE, "{", -1),

                                     Token(WHILE, "while", 4),          Token(OPEN_BRACKET, "(", 4),

                                     Token(OPEN_BRACKET, "(", 1),       Token(NAME, "x", 1),
                                     Token(GREATER_THAN, ">", 1),       Token(INTEGER, "2", 1),
                                     Token(CLOSE_BRACKET, ")", 1),      Token(ANDAND, "&&", 1),
                                     Token(OPEN_BRACKET, "(", 1),       Token(NAME, "x", 1),
                                     Token(GREATER_THAN, ">", 1),       Token(INTEGER, "2", 1),
                                     Token(CLOSE_BRACKET, ")", 1),      Token(CLOSE_BRACKET, ")", 4),
                                     Token(OPEN_CURLY_BRACE, "{", 4),

                                     Token(PRINT, "print", 2),          Token(NAME, "y", 2),
                                     Token(SEMICOLON, ";", 2),

                                     Token(CLOSE_CURLY_BRACE, "}", -1), Token(CLOSE_CURLY_BRACE, "}", -1)};
        bool result = syntaxValidator.validateSyntax(tokens);
        REQUIRE(result);
    }

    SECTION("Handle correct WHILE STATEMENT") {
        std::vector<Token> tokens = {Token(PROCEDURE, "procedure", -1),
                                     Token(NAME, "proc1", -1),
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

                                     Token(PRINT, "print", 2),
                                     Token(NAME, "y", 2),
                                     Token(SEMICOLON, ";", 2),

                                     Token(CLOSE_CURLY_BRACE, "}", -1),
                                     Token(CLOSE_CURLY_BRACE, "}", -1)};
        bool result = syntaxValidator.validateSyntax(tokens);
        REQUIRE(result);
    }

    SECTION("Handle incorrect WHILE STATEMENT") {
        // Missing open bracket before cond_expr
        std::vector<Token> tokens = {Token(PROCEDURE, "procedure", -1),
                                     Token(NAME, "proc1", -1),
                                     Token(OPEN_CURLY_BRACE, "{", -1),

                                     Token(WHILE, "while", 4),
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

                                     Token(PRINT, "print", 2),
                                     Token(NAME, "y", 2),
                                     Token(SEMICOLON, ";", 2),

                                     Token(CLOSE_CURLY_BRACE, "}", -1),
                                     Token(CLOSE_CURLY_BRACE, "}", -1)};
        REQUIRE_THROWS_AS(syntaxValidator.validateSyntax(tokens), SyntaxError);
    }

    SECTION("Handle correct PRINT STATEMENT") {
        std::vector<Token> tokens = {
            Token(PROCEDURE, "procedure", -1), Token(NAME, "example", -1), Token(OPEN_CURLY_BRACE, "{", -1),

            Token(PRINT, "print", 2),          Token(NAME, "y", 2),        Token(SEMICOLON, ";", 2),

            Token(CLOSE_CURLY_BRACE, "}", -1)};
        bool result = syntaxValidator.validateSyntax(tokens);
        REQUIRE(result);
    }

    SECTION("Handle incorrect PRINT STATEMENT") {
        // Integer instead of name
        std::vector<Token> tokens = {
            Token(PROCEDURE, "procedure", -1), Token(NAME, "example", -1), Token(OPEN_CURLY_BRACE, "{", -1),

            Token(PRINT, "print", 2),          Token(INTEGER, "7", 4),     Token(SEMICOLON, ";", 2),

            Token(CLOSE_CURLY_BRACE, "}", -1)};
        REQUIRE_THROWS_AS(syntaxValidator.validateSyntax(tokens), SyntaxError);
    }

    SECTION("Handle correct READ STATEMENT") {
        std::vector<Token> tokens = {Token(PROCEDURE, "procedure", -1),
                                     Token(NAME, "example", -1),
                                     Token(OPEN_CURLY_BRACE, "{", -1),

                                     Token(READ, "read", 2),
                                     Token(NAME, "y", 2),
                                     Token(SEMICOLON, ";", 2),

                                     Token(CLOSE_CURLY_BRACE, "}", -1)};
        bool result = syntaxValidator.validateSyntax(tokens);
        REQUIRE(result);
    }

    SECTION("Handle incorrect READ STATEMENT") {
        // Close bracket instead of semicolon
        std::vector<Token> tokens = {Token(PROCEDURE, "procedure", -1),
                                     Token(NAME, "example", -1),
                                     Token(OPEN_CURLY_BRACE, "{", -1),

                                     Token(READ, "read", 2),
                                     Token(NAME, "y", 2),
                                     Token(CLOSE_BRACKET, ")", 2),

                                     Token(CLOSE_CURLY_BRACE, "}", -1)};
        REQUIRE_THROWS_AS(syntaxValidator.validateSyntax(tokens), SyntaxError);
    }
}
