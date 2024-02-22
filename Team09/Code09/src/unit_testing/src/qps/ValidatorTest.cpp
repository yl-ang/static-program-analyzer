#include <qps/validator/Validator.h>

#include <string>
#include <vector>

#include "catch.hpp"

class TestValidator : public Validator {
public:  // NOLINT
    TestValidator() {}

    explicit TestValidator(const SynonymStore& storage) {
        this->synonymStore = storage;
    }

    bool testIsValidDeclarationStatement(const std::string& inputString, bool expectedResult) {
        return isValidDeclarationStatement(inputString) == expectedResult;
    }

    bool testIsValidSelectStatement(const std::string& inputString, bool expectedResult) {
        return isValidSelectStatement(inputString) == expectedResult;
    }
};

TEST_CASE("Syntax Validator") {
    TestValidator validator;

    std::string inputString_OneSynonym = "assign a;";
    std::string inputString_TwoSynonyms = "assign a, b;";
    std::string inputString_MultipleSynonyms = "assign a, b, c;";
    std::string inputString_NoSynonyms = "assign;";
    std::string inputString_NoSynonyms2 = "assign ;";
    std::string inputString_MissingSemiColon = "assign a";
    std::string inputString_SynonymWithSpace = "assign a b;";
    std::string inputString_SynonymInteger = "assign 1;";

    REQUIRE(validator.testIsValidDeclarationStatement(inputString_OneSynonym, true));
    REQUIRE(validator.testIsValidDeclarationStatement(inputString_TwoSynonyms, true));
    REQUIRE(validator.testIsValidDeclarationStatement(inputString_MultipleSynonyms, true));
    REQUIRE(validator.testIsValidDeclarationStatement(inputString_NoSynonyms, false));
    REQUIRE(validator.testIsValidDeclarationStatement(inputString_NoSynonyms2, false));
    REQUIRE(validator.testIsValidDeclarationStatement(inputString_MissingSemiColon, false));
    REQUIRE(validator.testIsValidDeclarationStatement(inputString_SynonymWithSpace, false));
    REQUIRE(validator.testIsValidDeclarationStatement(inputString_SynonymInteger, false));
}

TEST_CASE("isValidSelectStatment") {
    std::string inputString_SelectOne = "Select v";
    std::string inputString_ExtraSemiColon = "Select v;";
    std::string inputString_SelectNothing = "Select";
    std::string inputString_SelectNothingWithSpace = "Select ";

    SynonymStore store;
    store.storeSynonym("v", "variable");
    TestValidator validator(store);

    REQUIRE(validator.testIsValidSelectStatement(inputString_SelectOne, true));
    REQUIRE(validator.testIsValidSelectStatement(inputString_ExtraSemiColon, false));
    REQUIRE(validator.testIsValidSelectStatement(inputString_SelectNothing, false));
    REQUIRE(validator.testIsValidSelectStatement(inputString_SelectNothingWithSpace, false));
}

TEST_CASE("isValidSelectStatement") {
    Validator validator;
    validator.validate({"Select va such that Follows(1,2) pattern a(1,2)"});
    REQUIRE(true);
}
