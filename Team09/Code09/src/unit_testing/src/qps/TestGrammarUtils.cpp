#include "catch.hpp"
#include "qps/GrammarUtils.h"

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

    REQUIRE(isSelectStatement("Select v") == true);
}

// TEST_CASE("Syntax Validator") {
//     std::string inputString_OneSynonym = "assign a;";
//     std::string inputString_TwoSynonyms = "assign a, b;";
//     std::string inputString_MultipleSynonyms = "assign a, b, c;";
//     std::string inputString_NoSynonyms = "assign;";
//     std::string inputString_NoSynonyms2 = "assign ;";
//     std::string inputString_MissingSemiColon = "assign a";
//     std::string inputString_SynonymWithSpace = "assign a b;";
//     std::string inputString_SynonymInteger = "assign 1;";
//
//     REQUIRE(isValidDeclarationStatement(inputString_OneSynonym) == true);
//     REQUIRE(isValidDeclarationStatement(inputString_TwoSynonyms) == true);
//     REQUIRE(isValidDeclarationStatement(inputString_MultipleSynonyms) == true);
//     REQUIRE(isValidDeclarationStatement(inputString_NoSynonyms) == false);
//     REQUIRE(isValidDeclarationStatement(inputString_NoSynonyms2) == false);
//     REQUIRE(isValidDeclarationStatement(inputString_MissingSemiColon) == false);
//     REQUIRE(isValidDeclarationStatement(inputString_SynonymWithSpace) == false);
//     REQUIRE(isValidDeclarationStatement(inputString_SynonymInteger) == false);
//
//     std::string inputString_SelectOne = "Select v";
//     std::string inputString_ExtraSemiColon = "Select v;";
//     std::string inputString_SelectNothing = "Select";
//     std::string inputString_SelectNothing2 = "Select ";
//     REQUIRE(isValidSelectStatement(inputString_SelectOne) == true);
//     REQUIRE(isValidSelectStatement(inputString_ExtraSemiColon) == false);
//     REQUIRE(isValidSelectStatement(inputString_SelectNothing) == false);
//     REQUIRE(isValidSelectStatement(inputString_SelectNothing2) == false);
// }
