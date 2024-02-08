#include "qps/GrammarUtils.h"

#include "catch.hpp"

TEST_CASE("Grammar Utils") {
    // check ident
    REQUIRE(isIdent("Capitalize") == true);
    REQUIRE(isIdent("camelCase") == true);
    REQUIRE(isIdent("UPPER") == true);
    REQUIRE(isIdent("lower") == true);
    REQUIRE(isIdent("withNumbers091") == true);
    REQUIRE(isIdent("x") == true);

    REQUIRE(isIdent("010startingWithNumbers") == false);
    REQUIRE(isIdent("with space") == false);
    REQUIRE(isIdent("with_underscore") == false);
    REQUIRE(isIdent("with\"Quotes\"") == false);
    REQUIRE(isIdent("\"startsWithQuotes\"") == false);

    // check integer
    REQUIRE(isInteger("0") == true);
    REQUIRE(isInteger("1") == true);
    REQUIRE(isInteger("1000") == true);
    REQUIRE(isInteger("9848723") == true);

    REQUIRE(isInteger("9,848,723") == false);
    REQUIRE(isInteger("9 848 723") == false);
    REQUIRE(isInteger("00912914") == false);
    REQUIRE(isInteger("984nn123") == false);
    REQUIRE(isInteger("0000") == false);

    REQUIRE(isDeclarationStatement("stmt s;") == true);
    REQUIRE(isDeclarationStatement("statement s;") == false);
}
