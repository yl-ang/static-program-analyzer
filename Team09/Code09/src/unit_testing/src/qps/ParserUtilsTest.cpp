#include "catch.hpp"
#include "qps/ParserUtils.h"

TEST_CASE("Trim Test") {
    std::string expectedString = "Trim Test";

    std::string inputString_WithSpaces = " Trim Test ";
    std::string inputString_WithNewLine = "\nTrim Test\n";
    std::string inputString_WithTab = "\tTrim Test\t";

    std::string inputString_WithMultipleSpaces = "    Trim Test   ";
    std::string inputString_WithMixedWhitespaces = "\n \t Trim Test \f \b\r";

    REQUIRE(trim(inputString_WithSpaces) == expectedString);
    REQUIRE(trim(inputString_WithNewLine) == expectedString);
    REQUIRE(trim(inputString_WithTab) == expectedString);
    REQUIRE(trim(inputString_WithMultipleSpaces) == expectedString);
    REQUIRE(trim(inputString_WithMixedWhitespaces) == expectedString);
}

// TEST_CASE("Substring By Delimiter") {
//     std::string expectedString = "Substring";
//
//     std::string resultString;
//     size_t index;
//     substringByDelimiter("Substring Test", " ", resultString, index);
//
//     REQUIRE(resultString == expectedString);
// }

TEST_CASE("Removing Whitespace Characters") {
    std::string expectedString = "This is the perfect sentence without problems.";

    std::string testString = "This is\tthe \nperfect sentence   without \tproblems.";
    std::string resultString = replaceAllWhitespaces(testString);

    REQUIRE(resultString == expectedString);
}

TEST_CASE("getAllClauses") {
    std::string inputString_Empty = "";
    std::string inputString_OneSuchThat = "such that Follows(1, 2)";
    std::string inputString_TwoSuchThat = "such that Follows(1, 2) such that Follows(2, 3)";
    std::string inputString_OneSuchThatOnePattern = "such that Follows(1, 2) pattern a(a, \"_\")";
    std::string inputString_OnePattern = "pattern a(a, \"_\")";

    std::vector<std::string> expectedList_Empty = {};
    std::vector<std::string> expectedList_OneSuchThat = {"such that Follows(1, 2)"};
    std::vector<std::string> expectedList_TwoSuchThat = {"such that Follows(1, 2) ", "such that Follows(2, 3)"};
    std::vector<std::string> expectedList_OneSuchThatOnePattern = {"such that Follows(1, 2) ", "pattern a(a, \"_\")"};
    std::vector<std::string> expectedList_OnePattern = {"pattern a(a, \"_\")"};

    REQUIRE(getAllClauses(inputString_Empty) == expectedList_Empty);
    REQUIRE(getAllClauses(inputString_OneSuchThat) == expectedList_OneSuchThat);
    REQUIRE(getAllClauses(inputString_TwoSuchThat) == expectedList_TwoSuchThat);
    REQUIRE(getAllClauses(inputString_OneSuchThatOnePattern) == expectedList_OneSuchThatOnePattern);
    REQUIRE(getAllClauses(inputString_OnePattern) == expectedList_OnePattern);
}

TEST_CASE("getClauseIndices") {
    const std::string suchThatClause = "such that";
    const std::string patternClause = "pattern";
    std::string inputString_Empty = "";
    std::string inputString_OneSuchThat = "such that Follows(1, 2)";
    std::string inputString_TwoSuchThat = "such that Follows(1, 2) such that Follows(2, 3)";
    std::string inputString_OneSuchThatOnePattern = "such that Follows(1, 2) pattern a(a, \"_\")";
    std::string inputString_OnePattern = "pattern a(a, \"_\")";

    // Find "such that"
    std::vector<size_t> expectedList_Empty = {};
    std::vector<size_t> expectedList_OneSuchThat_suchthat = {0};
    std::vector<size_t> expectedList_TwoSuchThat_suchthat = {0, 24};
    std::vector<size_t> expectedList_OneSuchThatOnePattern_suchthat = {0};
    std::vector<size_t> expectedList_OnePattern_suchthat = {};

    REQUIRE(getClauseIndices(inputString_Empty, suchThatClause) == expectedList_Empty);
    REQUIRE(getClauseIndices(inputString_OneSuchThat, suchThatClause) == expectedList_OneSuchThat_suchthat);
    REQUIRE(getClauseIndices(inputString_TwoSuchThat, suchThatClause) == expectedList_TwoSuchThat_suchthat);
    REQUIRE(getClauseIndices(inputString_OneSuchThatOnePattern, suchThatClause) ==
            expectedList_OneSuchThatOnePattern_suchthat);
    REQUIRE(getClauseIndices(inputString_OnePattern, suchThatClause) == expectedList_OnePattern_suchthat);

    // Find "pattern"
    std::vector<size_t> expectedList_TwoSuchThat_pattern = {};
    std::vector<size_t> expectedList_OneSuchThatOnePattern_pattern = {24};
    std::vector<size_t> expectedList_OnePattern_pattern = {0};

    REQUIRE(getClauseIndices(inputString_TwoSuchThat, patternClause) == expectedList_TwoSuchThat_pattern);
    REQUIRE(getClauseIndices(inputString_OneSuchThatOnePattern, patternClause) ==
            expectedList_OneSuchThatOnePattern_pattern);
    REQUIRE(getClauseIndices(inputString_OnePattern, patternClause) == expectedList_OnePattern_pattern);
}
