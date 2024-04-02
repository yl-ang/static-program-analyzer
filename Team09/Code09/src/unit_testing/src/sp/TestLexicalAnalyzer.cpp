#include "catch.hpp"
#include "sp/tokenizer/LexicalAnalyzer.h"

TEST_CASE("Lexical Analyzer Tests") {
    LexicalAnalyzer lexicalAnalyzer;

    SECTION("preprocess") {
        std::vector<BasicToken> expectedResultList = {BasicToken("x", _NAME)};
        std::vector<std::string> inputString = {"x"};
        std::vector<BasicToken> resultList = lexicalAnalyzer.preprocess(inputString);

        REQUIRE(expectedResultList == resultList);
    }

    SECTION("assignType") {
        BasicToken expectedResult = BasicToken("=", SYMBOL);
        BasicToken result = lexicalAnalyzer.assignType("=", "x", "1");
        REQUIRE(expectedResult == result);
    }

    SECTION("disambiguate") {
        BasicToken expectedResult = BasicToken("procedure", KEYWORD);
        BasicToken result = lexicalAnalyzer.disambiguate("procedure", "", "x");
        REQUIRE(expectedResult == result);
    }
}
