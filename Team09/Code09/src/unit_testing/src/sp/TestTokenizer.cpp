#include "catch.hpp"
#include "sp/tokenizer/Tokenizer.h"

TEST_CASE("Tokenizer Tests") {
  Tokenizer tokenizer;

  SECTION("Token Equality Test") {
    Token tokenA = Token(NAME, "x", 1);
    Token tokenB = Token(NAME, "x", 1);

    REQUIRE(tokenA == tokenB);
  }

  SECTION("tokenize") {
    std::vector<Token> expectedResultList = {
      Token(PROCEDURE, "procedure", -1),
        Token(NAME, "example", -1),
          Token(OPEN_CURLY_BRACE, "{", -1),
            Token(NAME, "x", 1),
              Token(EQUAL, "=", 1),
                Token(INTEGER, "1", 1),
                  Token(CLOSE_CURLY_BRACE, "}", -1)};
    std::vector<std::string> inputString = {"procedure", "example", "{", "x", "=", "1", "}"};
    std::vector<Token> resultList = tokenizer.tokenize(inputString);
    REQUIRE(expectedResultList == resultList);
  }
}
