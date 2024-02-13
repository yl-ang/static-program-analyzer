#include "catch.hpp"
#include "sp/tokenizer/Tokenizer.h"

TEST_CASE("Tokenizer Tests") {
  SpTokenizer tokenizer;

  SECTION("Token Equality Test") {
    Token tokenA = Token(NAME, "x", 1);
    Token tokenB = Token(NAME, "x", 1);

    REQUIRE(tokenA == tokenB);
  }

  SECTION("tokenize") {
    std::vector<Token> expectedResultList = {Token(PROCEDURE, "procedure", -1),
                                             Token(NAME, "example", -1),
                                             Token(OPEN_CURLY_BRACE, "{", -1),
                                             Token(NAME, "x", 1),
                                             Token(EQUAL, "=", 1),
                                             Token(INTEGER, "1", 1),
                                             Token(SEMICOLON, ";", 1),
                                             Token(WHILE, "while", 2),
                                             Token(OPEN_BRACKET, "(", 2),
                                             Token(NAME, "x", 2),
                                             Token(GREATER_THAN, ">", 2),
                                             Token(INTEGER, "0", 2),
                                             Token(CLOSE_BRACKET, ")", 2),
                                             Token(OPEN_CURLY_BRACE, "{", 2),
                                             Token(NAME, "x", 3),
                                             Token(EQUAL, "=", 3),
                                             Token(NAME, "x", 3),
                                             Token(SUB, "-", 3),
                                             Token(INTEGER, "1", 3),
                                             Token(SEMICOLON, ";", 3),
                                             Token(CLOSE_CURLY_BRACE, "}", -1),
                                             Token(CLOSE_CURLY_BRACE, "}", -1)};
    std::vector<std::string> inputString = {
        "procedure", "example", "{", "x", "=", "1", ";", "while",
        "(",         "x",       ">", "0", ")", "{", "x", "=",
        "x",         "-",       "1", ";", "}", "}"};
    std::vector<Token> resultList = tokenizer.tokenize(inputString);
    REQUIRE(expectedResultList == resultList);
  }
}
