#include "catch.hpp"
#include "sp/tokenizer/Tokenizer.h"

TEST_CASE("Tokenizer Tests") {
  Tokenizer tokenizer;

  SECTION("Token Equality Test") {
    Token tokenA = Token(NAME, "x", 1);
    Token tokenB = Token(NAME, "x", 1);

    REQUIRE(tokenA == tokenB);
  }

  SECTION("A") {
    std::vector<Token> expectedResultList = {Token(NAME, "x", 1)};
    std::vector<std::string> inputString = {"x"};
    std::vector<Token> resultList = tokenizer.tokenize(inputString);
    REQUIRE(expectedResultList == resultList);
  }
}
