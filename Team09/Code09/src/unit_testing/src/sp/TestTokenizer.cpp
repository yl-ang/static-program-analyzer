#include "catch.hpp"
#include "sp/tokenizer/SpTokenizer.h"

TEST_CASE("Tokenizer Tests") {
    SpTokenizer tokenizer;

    SECTION("Token Equality Test") {
        Token tokenA = Token(NAME, "x", 1);
        Token tokenB = Token(NAME, "x", 1);

        REQUIRE(tokenA == tokenB);
    }

    SECTION("Tokenize PROCEDURE correctly") {
        std::vector<Token> expectedResultList = {Token(PROCEDURE, "procedure", -1)};
        std::vector<BasicToken> input = {BasicToken("procedure", KEYWORD)};
        std::vector<Token> resultList = tokenizer.assignTokens(input);
        REQUIRE(expectedResultList == resultList);
    }

    SECTION("Tokenize OPEN_CURLY_BRACE correctly") {
        std::vector<Token> expectedResultList = {Token(OPEN_CURLY_BRACE, "{", 0)};
        std::vector<BasicToken> input = {BasicToken("{", SYMBOL)};
        std::vector<Token> resultList = tokenizer.assignTokens(input);
        REQUIRE(expectedResultList == resultList);
    }

    SECTION("Tokenize CLOSE_CURLY_BRACE correctly") {
        std::vector<Token> expectedResultList = {Token(CLOSE_CURLY_BRACE, "}", -1)};
        std::vector<BasicToken> input = {BasicToken("}", SYMBOL)};
        std::vector<Token> resultList = tokenizer.assignTokens(input);
        REQUIRE(expectedResultList == resultList);
    }

    SECTION("Tokenize SEMICOLON correctly") {
        std::vector<Token> expectedResultList = {Token(SEMICOLON, ";", 0)};
        std::vector<BasicToken> input = {BasicToken(";", SYMBOL)};
        std::vector<Token> resultList = tokenizer.assignTokens(input);
        REQUIRE(expectedResultList == resultList);
    }

    SECTION("Tokenize NAME correctly") {
        std::vector<Token> expectedResultList = {Token(NAME, "happy", 0)};
        std::vector<BasicToken> input = {BasicToken("happy", _NAME)};
        std::vector<Token> resultList = tokenizer.assignTokens(input);
        REQUIRE(expectedResultList == resultList);
    }

    SECTION("Tokenize INTEGER correctly") {
        std::vector<Token> expectedResultList = {Token(INTEGER, "123843", 0)};
        std::vector<BasicToken> input = {BasicToken("123843", _INTEGER)};
        std::vector<Token> resultList = tokenizer.assignTokens(input);
        REQUIRE(expectedResultList == resultList);
    }

    SECTION("Tokenize EQUAL correctly") {
        std::vector<Token> expectedResultList = {Token(EQUAL, "=", 0)};
        std::vector<BasicToken> input = {BasicToken("=", SYMBOL)};
        std::vector<Token> resultList = tokenizer.assignTokens(input);
        REQUIRE(expectedResultList == resultList);
    }

    SECTION("Tokenize READ correctly") {
        std::vector<Token> expectedResultList = {Token(READ, "read", 1)};
        std::vector<BasicToken> input = {BasicToken("read", KEYWORD)};
        std::vector<Token> resultList = tokenizer.assignTokens(input);
        REQUIRE(expectedResultList == resultList);
    }

    SECTION("Tokenize PRINT correctly") {
        std::vector<Token> expectedResultList = {Token(PRINT, "print", 1)};
        std::vector<BasicToken> input = {BasicToken("print", KEYWORD)};
        std::vector<Token> resultList = tokenizer.assignTokens(input);
        REQUIRE(expectedResultList == resultList);
    }

    SECTION("Tokenize CALL correctly") {
        std::vector<Token> expectedResultList = {Token(CALL, "call", 1)};
        std::vector<BasicToken> input = {BasicToken("call", KEYWORD)};
        std::vector<Token> resultList = tokenizer.assignTokens(input);
        REQUIRE(expectedResultList == resultList);
    }

    SECTION("Tokenize IF correctly") {
        std::vector<Token> expectedResultList = {Token(IF, "if", 1)};
        std::vector<BasicToken> input = {BasicToken("if", KEYWORD)};
        std::vector<Token> resultList = tokenizer.assignTokens(input);
        REQUIRE(expectedResultList == resultList);
    }

    SECTION("Tokenize OPEN_BRACKET correctly") {
        std::vector<Token> expectedResultList = {Token(OPEN_BRACKET, "(", 0)};
        std::vector<BasicToken> input = {BasicToken("(", SYMBOL)};
        std::vector<Token> resultList = tokenizer.assignTokens(input);
        REQUIRE(expectedResultList == resultList);
    }
    SECTION("Tokenize CLOSE_BRACKET correctly") {
        std::vector<Token> expectedResultList = {Token(CLOSE_BRACKET, ")", 0)};
        std::vector<BasicToken> input = {BasicToken(")", SYMBOL)};
        std::vector<Token> resultList = tokenizer.assignTokens(input);
        REQUIRE(expectedResultList == resultList);
    }
    SECTION("Tokenize THEN correctly") {
        std::vector<Token> expectedResultList = {Token(THEN, "then", 0)};
        std::vector<BasicToken> input = {BasicToken("then", KEYWORD)};
        std::vector<Token> resultList = tokenizer.assignTokens(input);
        REQUIRE(expectedResultList == resultList);
    }
    SECTION("Tokenize ELSE correctly") {
        std::vector<Token> expectedResultList = {Token(ELSE, "else", -1)};
        std::vector<BasicToken> input = {BasicToken("else", KEYWORD)};
        std::vector<Token> resultList = tokenizer.assignTokens(input);
        REQUIRE(expectedResultList == resultList);
    }

    SECTION("Tokenize NOT correctly") {
        std::vector<Token> expectedResultList = {Token(NOT, "!", 0)};
        std::vector<BasicToken> input = {BasicToken("!", SYMBOL)};
        std::vector<Token> resultList = tokenizer.assignTokens(input);
        REQUIRE(expectedResultList == resultList);
    }

    SECTION("Tokenize ANDAND correctly") {
        std::vector<Token> expectedResultList = {Token(ANDAND, "&&", 0)};
        std::vector<BasicToken> input = {BasicToken("&&", SYMBOL)};
        std::vector<Token> resultList = tokenizer.assignTokens(input);
        REQUIRE(expectedResultList == resultList);
    }

    SECTION("Tokenize OR correctly") {
        std::vector<Token> expectedResultList = {Token(OR, "||", 0)};
        std::vector<BasicToken> input = {BasicToken("||", SYMBOL)};
        std::vector<Token> resultList = tokenizer.assignTokens(input);
        REQUIRE(expectedResultList == resultList);
    }

    SECTION("Tokenize LESS_THAN correctly") {
        std::vector<Token> expectedResultList = {Token(LESS_THAN, "<", 0)};
        std::vector<BasicToken> input = {BasicToken("<", SYMBOL)};
        std::vector<Token> resultList = tokenizer.assignTokens(input);
        REQUIRE(expectedResultList == resultList);
    }

    SECTION("Tokenize LESS_THAN_OR_EQUAL_TO correctly") {
        std::vector<Token> expectedResultList = {Token(LESS_THAN_OR_EQUAL_TO, "<=", 0)};
        std::vector<BasicToken> input = {BasicToken("<=", SYMBOL)};
        std::vector<Token> resultList = tokenizer.assignTokens(input);
        REQUIRE(expectedResultList == resultList);
    }

    SECTION("Tokenize GREATER_THAN correctly") {
        std::vector<Token> expectedResultList = {Token(GREATER_THAN, ">", 0)};
        std::vector<BasicToken> input = {BasicToken(">", SYMBOL)};
        std::vector<Token> resultList = tokenizer.assignTokens(input);
        REQUIRE(expectedResultList == resultList);
    }

    SECTION("Tokenize GREATER_THAN_OR_EQUAL_TO correctly") {
        std::vector<Token> expectedResultList = {Token(GREATER_THAN_OR_EQUAL_TO, ">=", 0)};
        std::vector<BasicToken> input = {BasicToken(">=", SYMBOL)};
        std::vector<Token> resultList = tokenizer.assignTokens(input);
        REQUIRE(expectedResultList == resultList);
    }

    SECTION("Tokenize EQUAL_CHECK correctly") {
        std::vector<Token> expectedResultList = {Token(EQUAL_CHECK, "==", 0)};
        std::vector<BasicToken> input = {BasicToken("==", SYMBOL)};
        std::vector<Token> resultList = tokenizer.assignTokens(input);
        REQUIRE(expectedResultList == resultList);
    }

    SECTION("Tokenize NOT_EQUAL_CHECK correctly") {
        std::vector<Token> expectedResultList = {Token(NOT_EQUAL_CHECK, "!=", 0)};
        std::vector<BasicToken> input = {BasicToken("!=", SYMBOL)};
        std::vector<Token> resultList = tokenizer.assignTokens(input);
        REQUIRE(expectedResultList == resultList);
    }

    SECTION("Tokenize ADD correctly") {
        std::vector<Token> expectedResultList = {Token(ADD, "+", 0)};
        std::vector<BasicToken> input = {BasicToken("+", SYMBOL)};
        std::vector<Token> resultList = tokenizer.assignTokens(input);
        REQUIRE(expectedResultList == resultList);
    }

    SECTION("Tokenize SUB correctly") {
        std::vector<Token> expectedResultList = {Token(SUB, "-", 0)};
        std::vector<BasicToken> input = {BasicToken("-", SYMBOL)};
        std::vector<Token> resultList = tokenizer.assignTokens(input);
        REQUIRE(expectedResultList == resultList);
    }

    SECTION("Tokenize MUL correctly") {
        std::vector<Token> expectedResultList = {Token(MUL, "*", 0)};
        std::vector<BasicToken> input = {BasicToken("*", SYMBOL)};
        std::vector<Token> resultList = tokenizer.assignTokens(input);
        REQUIRE(expectedResultList == resultList);
    }

    SECTION("Tokenize DIV correctly") {
        std::vector<Token> expectedResultList = {Token(DIV, "/", 0)};
        std::vector<BasicToken> input = {BasicToken("/", SYMBOL)};
        std::vector<Token> resultList = tokenizer.assignTokens(input);
        REQUIRE(expectedResultList == resultList);
    }

    SECTION("Tokenize MOD correctly") {
        std::vector<Token> expectedResultList = {Token(MOD, "%", 0)};
        std::vector<BasicToken> input = {BasicToken("%", SYMBOL)};
        std::vector<Token> resultList = tokenizer.assignTokens(input);
        REQUIRE(expectedResultList == resultList);
    }

    SECTION("Procedure definitions should not have statement numbers") {
        std::vector<Token> expectedResultList = {Token(PROCEDURE, "procedure", -1), Token(NAME, "example", -1),
                                                 Token(OPEN_CURLY_BRACE, "{", -1),  Token(READ, "read", 1),
                                                 Token(NAME, "example", 1),         Token(SEMICOLON, ";", 1),
                                                 Token(CLOSE_CURLY_BRACE, "}", -1)};

        std::vector<BasicToken> input = {BasicToken("procedure", KEYWORD), BasicToken("example", _NAME),
                                         BasicToken("{", SYMBOL),          BasicToken("read", KEYWORD),
                                         BasicToken("example", _NAME),     BasicToken(";", SYMBOL),
                                         BasicToken("}", SYMBOL)};
        std::vector<Token> resultList = tokenizer.assignTokens(input);
        REQUIRE(expectedResultList == resultList);
    }

    SECTION("Statement number correctly handled for while statements") {
        std::vector<Token> expectedResultList = {Token(WHILE, "while", 1),     Token(OPEN_BRACKET, "(", 1),
                                                 Token(CLOSE_BRACKET, ")", 1), Token(OPEN_CURLY_BRACE, "{", 1),
                                                 Token(READ, "read", 2),       Token(NAME, "example", 2),
                                                 Token(SEMICOLON, ";", 2),     Token(CLOSE_CURLY_BRACE, "}", -1)};

        std::vector<BasicToken> input = {BasicToken("while", KEYWORD), BasicToken("(", SYMBOL),
                                         BasicToken(")", SYMBOL),      BasicToken("{", SYMBOL),
                                         BasicToken("read", KEYWORD),  BasicToken("example", _NAME),
                                         BasicToken(";", SYMBOL),      BasicToken("}", SYMBOL)};
        std::vector<Token> resultList = tokenizer.assignTokens(input);
        REQUIRE(expectedResultList == resultList);
    }

    SECTION("Statement number correctly handled for if statements") {
        std::vector<Token> expectedResultList = {
            Token(IF, "if", 1),        Token(OPEN_BRACKET, "(", 1),      Token(CLOSE_BRACKET, ")", 1),
            Token(THEN, "then", 1),    Token(OPEN_CURLY_BRACE, "{", 1),  Token(READ, "read", 2),
            Token(NAME, "example", 2), Token(SEMICOLON, ";", 2),         Token(CLOSE_CURLY_BRACE, "}", -1),
            Token(ELSE, "else", -1),   Token(OPEN_CURLY_BRACE, "{", -1), Token(READ, "read", 3),
            Token(NAME, "example", 3), Token(SEMICOLON, ";", 3),         Token(CLOSE_CURLY_BRACE, "}", -1)};

        std::vector<BasicToken> input = {
            BasicToken("if", KEYWORD),    BasicToken("(", SYMBOL), BasicToken(")", SYMBOL),
            BasicToken("then", KEYWORD),  BasicToken("{", SYMBOL), BasicToken("read", KEYWORD),
            BasicToken("example", _NAME), BasicToken(";", SYMBOL), BasicToken("}", SYMBOL),
            BasicToken("else", KEYWORD),  BasicToken("{", SYMBOL), BasicToken("read", KEYWORD),
            BasicToken("example", _NAME), BasicToken(";", SYMBOL), BasicToken("}", SYMBOL)};
        std::vector<Token> resultList = tokenizer.assignTokens(input);
        REQUIRE(expectedResultList == resultList);
    }

    SECTION("Statement number correctly handled for read statements") {
        std::vector<Token> expectedResultList = {Token(READ, "read", 1), Token(NAME, "happy", 1),
                                                 Token(SEMICOLON, ";", 1)};

        std::vector<BasicToken> input = {BasicToken("read", KEYWORD), BasicToken("happy", _NAME),
                                         BasicToken(";", SYMBOL)};
        std::vector<Token> resultList = tokenizer.assignTokens(input);
        REQUIRE(expectedResultList == resultList);
    }

    SECTION("Statement number correctly handled for assign statements") {
        std::vector<Token> expectedResultList = {Token(NAME, "x", 1), Token(EQUAL, "=", 1), Token(INTEGER, "4", 1),
                                                 Token(SEMICOLON, ";", 1)};

        std::vector<BasicToken> input = {BasicToken("x", _NAME), BasicToken("=", SYMBOL), BasicToken("4", _INTEGER),
                                         BasicToken(";", SYMBOL)};
        std::vector<Token> resultList = tokenizer.assignTokens(input);
        REQUIRE(expectedResultList == resultList);
    }

    SECTION("Statement number correctly handled for call statements") {
        std::vector<Token> expectedResultList = {Token(CALL, "call", 1), Token(NAME, "happy", 1),
                                                 Token(SEMICOLON, ";", 1)};

        std::vector<BasicToken> input = {BasicToken("call", KEYWORD), BasicToken("happy", _NAME),
                                         BasicToken(";", SYMBOL)};
        std::vector<Token> resultList = tokenizer.assignTokens(input);
        REQUIRE(expectedResultList == resultList);
    }

    SECTION("Statement number correctly handled for print statements") {
        std::vector<Token> expectedResultList = {Token(PRINT, "print", 1), Token(NAME, "happy", 1),
                                                 Token(SEMICOLON, ";", 1)};

        std::vector<BasicToken> input = {BasicToken("print", KEYWORD), BasicToken("happy", _NAME),
                                         BasicToken(";", SYMBOL)};
        std::vector<Token> resultList = tokenizer.assignTokens(input);
        REQUIRE(expectedResultList == resultList);
    }
}
