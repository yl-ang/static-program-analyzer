#include "qps/syntaxValidator/SyntaxValidator.h"

#include "catch.hpp"

TEST_CASE("Syntax Validator") {
    SyntaxValidator validator;
    std::string inputString_OneSynonym = "assign a;";
    std::string inputString_TwoSynonyms = "assign a, b;";
    std::string inputString_MultipleSynonyms = "assign a, b, c;";
    std::string inputString_NoSynonyms = "assign;";
    std::string inputString_NoSynonyms2 = "assign ;";
    std::string inputString_MissingSemiColon = "assign a";
    std::string inputString_SynonymWithSpace = "assign a b;";
    std::string inputString_SynonymInteger = "assign 1;";

    REQUIRE(validator.isValidDeclarationStatement(inputString_OneSynonym) == true);
    REQUIRE(validator.isValidDeclarationStatement(inputString_TwoSynonyms) == true);
    REQUIRE(validator.isValidDeclarationStatement(inputString_MultipleSynonyms) == true);
    REQUIRE(validator.isValidDeclarationStatement(inputString_NoSynonyms) == false);
    REQUIRE(validator.isValidDeclarationStatement(inputString_NoSynonyms2) == false);
    REQUIRE(validator.isValidDeclarationStatement(inputString_MissingSemiColon) == false);
    REQUIRE(validator.isValidDeclarationStatement(inputString_SynonymWithSpace) == false);
    REQUIRE(validator.isValidDeclarationStatement(inputString_SynonymInteger) == false);

    std::string inputString_SelectOne = "Select v";
    std::string inputString_ExtraSemiColon = "Select v;";
    std::string inputString_SelectNothing = "Select";
    std::string inputString_SelectNothing2 = "Select ";
    REQUIRE(validator.isValidSelectStatement(inputString_SelectOne) == true);
    REQUIRE(validator.isValidSelectStatement(inputString_ExtraSemiColon) == false);
    REQUIRE(validator.isValidSelectStatement(inputString_SelectNothing) == false);
    REQUIRE(validator.isValidSelectStatement(inputString_SelectNothing2) == false);
}
