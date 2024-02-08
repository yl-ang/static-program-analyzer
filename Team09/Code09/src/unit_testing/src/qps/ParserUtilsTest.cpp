#include "qps/ParserUtils.h"

#include "catch.hpp"

TEST_CASE("Parser Utils") {
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