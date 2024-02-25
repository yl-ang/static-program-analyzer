#include <qps/validator/Validator.h>

#include <string>
#include <vector>

#include "catch.hpp"

class TestValidator : public Validator {
public:  // NOLINT
    // Constructors
    TestValidator() {}

    explicit TestValidator(const SynonymStore& storage) {
        this->synonymStore = storage;
    }

    // Test Methods
    bool testValidate(std::vector<std::string> inputStatements) {
        try {
            validate(inputStatements);
        } catch (Exception e) {
            return false;
        }
        return true;
    }

    bool testIsValidDeclarationStatement(const std::string& inputString) {
        try {
            validateDeclarationStatement(inputString);
        } catch (Exception e) {
            return false;
        }
        return true;
    }

    bool testIsValidSelectStatement(const std::string& inputString) {
        try {
            validateSelectStatement(inputString);
        } catch (Exception e) {
            return false;
        }
        return true;
    }

    bool testIsValidSuchThatClause(const std::string& inputString) {
        try {
            validateSuchThatClause(inputString);
        } catch (Exception e) {
            return false;
        }
        return true;
    }

    bool testIsValidPatternClause(const std::string& inputString) {
        try {
            validatePatternClause(inputString);
        } catch (Exception e) {
            return false;
        }
        return true;
    }
};

TEST_CASE("validateDeclarationStatement") {
    TestValidator validator;

    std::string inputString_OneSynonym = "assign a;";
    std::string inputString_TwoSynonyms = "assign a, b;";
    std::string inputString_MultipleSynonyms = "assign a, b, c;";
    std::string inputString_NoSynonyms = "assign;";
    std::string inputString_NoSynonyms2 = "assign ;";
    std::string inputString_MissingSemiColon = "assign a";
    std::string inputString_SynonymWithSpace = "assign a b;";
    std::string inputString_SynonymInteger = "assign 1;";

    REQUIRE(validator.testIsValidDeclarationStatement(inputString_OneSynonym));
    REQUIRE(validator.testIsValidDeclarationStatement(inputString_TwoSynonyms));
    REQUIRE(validator.testIsValidDeclarationStatement(inputString_MultipleSynonyms));

    REQUIRE_FALSE(validator.testIsValidDeclarationStatement(inputString_NoSynonyms));
    REQUIRE_FALSE(validator.testIsValidDeclarationStatement(inputString_NoSynonyms2));
    REQUIRE_FALSE(validator.testIsValidDeclarationStatement(inputString_MissingSemiColon));
    REQUIRE_FALSE(validator.testIsValidDeclarationStatement(inputString_SynonymWithSpace));
    REQUIRE_FALSE(validator.testIsValidDeclarationStatement(inputString_SynonymInteger));
}

TEST_CASE("isValidSelectStatment") {
    std::string inputString_SelectOne = "Select v";
    std::string inputString_ExtraSemiColon = "Select v;";
    std::string inputString_SelectNothing = "Select";
    std::string inputString_SelectNothingWithSpace = "Select ";
    std::string inputString_OneSuchThat = "Select v such that Follows(1,2)";
    std::string inputString_OnePattern = "Select v pattern a(v,2)";

    SynonymStore testStore;
    testStore.storeSynonym("v", "variable");
    testStore.storeSynonym("a", "assign");
    TestValidator validator(testStore);

    REQUIRE(validator.testIsValidSelectStatement(inputString_SelectOne));
    REQUIRE_FALSE(validator.testIsValidSelectStatement(inputString_ExtraSemiColon));
    REQUIRE_FALSE(validator.testIsValidSelectStatement(inputString_SelectNothing));
    REQUIRE_FALSE(validator.testIsValidSelectStatement(inputString_SelectNothingWithSpace));
}

TEST_CASE("validateSuchThatClause") {
    SynonymStore testStore;
    testStore.storeSynonym("s1", "stmt");
    testStore.storeSynonym("v", "variable");
    testStore.storeSynonym("re", "read");

    TestValidator validator(testStore);

    std::string inputString_Follows = "such that Follows(1,2)";
    REQUIRE(validator.testIsValidSuchThatClause(inputString_Follows));

    std::string inputString_FollowsT = "such that Follows*(1,2)";
    REQUIRE(validator.testIsValidSuchThatClause(inputString_FollowsT));

    std::string inputString_Follows_Stmt_VALID = "such that Follows(s1,2)";
    REQUIRE(validator.testIsValidSuchThatClause(inputString_Follows_Stmt_VALID));

    std::string inputString_Parent_VALID = "such that Parent(re,s1)";
    REQUIRE(validator.testIsValidSuchThatClause(inputString_Parent_VALID));

    std::string inputString_Modifies_VALID = "such that Modifies(s1,v)";
    REQUIRE(validator.testIsValidSuchThatClause(inputString_Modifies_VALID));

    std::string inputString_Uses_VALID = "such that Uses(s1,v)";
    REQUIRE(validator.testIsValidSuchThatClause(inputString_Uses_VALID));

    std::string inputString_Follows_Stmt_INVALID = "such that Follows(s2,2)";
    REQUIRE_FALSE(validator.testIsValidSuchThatClause(inputString_Follows_Stmt_INVALID));

    std::string inputString_Parent_FirstArgVariable_INVALID = "such that Parent(v,s1)";
    REQUIRE_FALSE(validator.testIsValidSuchThatClause(inputString_Parent_FirstArgVariable_INVALID));

    std::string inputString_Modifies_FirstArgWildcard_INVALID = "such that Modifies(_,v)";
    REQUIRE_FALSE(validator.testIsValidSuchThatClause(inputString_Modifies_FirstArgWildcard_INVALID));

    std::string inputString_Modifies_SecondArgVar_INVALID = "such that Modifies(v,s1)";
    REQUIRE_FALSE(validator.testIsValidSuchThatClause(inputString_Modifies_SecondArgVar_INVALID));

    std::string inputString_Uses_FirstArgWildcard_INVALID = "such that Uses(_,v)";
    REQUIRE_FALSE(validator.testIsValidSuchThatClause(inputString_Uses_FirstArgWildcard_INVALID));
}

TEST_CASE("validatePatternClause") {
    SynonymStore testStore;
    testStore.storeSynonym("a", "assign");
    testStore.storeSynonym("v", "variable");

    TestValidator validator(testStore);

    std::string inputString_VALID = "pattern a(v, _\"x\"_)";
    REQUIRE(validator.testIsValidPatternClause(inputString_VALID));

    std::string inputString_Space_VALID = "pattern a (v, _\"x\"_)";
    REQUIRE(validator.testIsValidPatternClause(inputString_Space_VALID));

    std::string inputString_Integer_VALID = "pattern a(v, _\"2\"_)";
    REQUIRE(validator.testIsValidPatternClause(inputString_Integer_VALID));

    std::string inputString_Wildcard_VALID = "pattern a(_, _\"x\"_)";
    REQUIRE(validator.testIsValidPatternClause(inputString_Wildcard_VALID));

    std::string inputString_Wildcards_VALID = "pattern a(_, _)";
    REQUIRE(validator.testIsValidPatternClause(inputString_Wildcards_VALID));

    std::string inputString_INVALID = "pattern a(v, _x\"_)";
    REQUIRE_FALSE(validator.testIsValidPatternClause(inputString_INVALID));
}

TEST_CASE("validateSelectStatement_STPattern_VALID") {
    TestValidator validator;

    std::vector<std::string> inputStatements = {"assign a;", "variable v;",
                                                "Select v such that Follows(1,2) pattern a(v,_\"2\"_)"};
    REQUIRE(validator.testValidate(inputStatements));
}

TEST_CASE("validateSelectStatement_PatternST_VALID") {
    TestValidator validator;

    std::vector<std::string> inputStatements = {"assign a;", "variable v;",
                                                "Select v pattern a(v,_\"2\"_) such that Follows(1,2)"};
    REQUIRE(validator.testValidate(inputStatements));
}

TEST_CASE("validateSelectStatement_UndeclaredSynonym_INVALID") {
    TestValidator validator;
    std::vector<std::string> inputStatements = {"assign a;", "variable v;",
                                                "Select v such that Follows(1,2) pattern a(v1,_\"2\"_)"};
    REQUIRE_FALSE(validator.testValidate(inputStatements));
}

TEST_CASE("validateSelectStatement_DuplicateSynonym_INVALID") {
    TestValidator validator;
    std::vector<std::string> inputStatements = {"assign a;", "variable v, v;",
                                                "Select v such that Follows(1,2) pattern a(v,_\"2\"_)"};
    REQUIRE_FALSE(validator.testValidate(inputStatements));
}

// Order of Select and declaration is not checked.
// However, it doesn't have to be since if it is a valid Select statement,
// it would not be able to retrieve the synonyms, resulting in Semantic Error
// Wrong error will be thrown (Semantic instead of Syntax).
TEST_CASE("validateSelectStatement_INVALID") {
    TestValidator validator;
    std::vector<std::string> inputStatements = {"Select v such that Follows(1,2) pattern a(v,_\"2\"_)", "assign a;",
                                                "variable v, v;"};
    REQUIRE_FALSE(validator.testValidate(inputStatements));
}
