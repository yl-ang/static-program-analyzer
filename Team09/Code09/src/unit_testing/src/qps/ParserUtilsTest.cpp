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
    std::string expectedString =
        "This is the perfect sentence without problems.";

    std::string testString =
        "This is\tthe \nperfect sentence   without \tproblems.";
    replaceAllWhitespaces(testString);

    REQUIRE(testString == expectedString);
}
