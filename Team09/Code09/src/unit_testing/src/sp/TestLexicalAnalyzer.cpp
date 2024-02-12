#include "catch.hpp"
#include "sp/tokenizer/LexicalAnalyzer.h"

std::vector<BasicToken> convertTokenPtrToToken(std::vector<BasicToken*> tokenPtrs) {
    std::vector<BasicToken> derefTokens;
    for (const auto& ptr : tokenPtrs) {
        derefTokens.push_back(*ptr);  // Dereference the pointer and push the token into the vector
    }
    // Clean up the memory allocated for token pointers
    for (const auto& ptr : tokenPtrs) {
        delete ptr;  // Delete each dynamically allocated token
    }
    return derefTokens;
}

TEST_CASE("Lexical Analyzer Tests") {
  LexicalAnalyzer lexicalAnalyzer;


  SECTION("preprocess") {
    std::vector<BasicToken*> expectedResultList = {new BasicToken("x", _NAME)};
    std::vector<std::string> inputString = {"x"};
    std::vector<BasicToken*> resultList = lexicalAnalyzer.preprocess(inputString);

    std::vector<BasicToken> expectedTokens = convertTokenPtrToToken(expectedResultList);
    std::vector<BasicToken> resultTokens = convertTokenPtrToToken(resultList);

    REQUIRE(expectedTokens == resultTokens);
  }

  SECTION("assignType") {
    BasicToken* expectedResult = new BasicToken("=", SYMBOL);
    BasicToken* result = lexicalAnalyzer.assignType("=", "x", "1");
    REQUIRE(*expectedResult == *result);
  }

  SECTION("disambiguate") {
    BasicToken* expectedResult = new BasicToken("procedure", KEYWORD);
    BasicToken* result = lexicalAnalyzer.disambiguate("procedure", "", "x");
    REQUIRE(*expectedResult == *result);
  }
}
