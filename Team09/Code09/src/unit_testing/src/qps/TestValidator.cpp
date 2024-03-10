#include <string>
#include <vector>

#include "catch.hpp"
#include "qps/exceptions/QPSSemanticError.h"
#include "qps/exceptions/QPSSyntaxError.h"
#include "qps/validator/Validator.h"

// CUSTOM TESTING MACROS
#define REQUIRE_THROW_SEMANTIC_ERROR(expression) REQUIRE_THROWS_AS(expression, QPSSemanticError)
#define REQUIRE_THROW_SYNTAX_ERROR(expression) REQUIRE_THROWS_AS(expression, QPSSyntaxError)

class TestValidator : public Validator {
public:  // NOLINT
    // Constructors
    TestValidator() {}

    explicit TestValidator(const SynonymStore& storage) {
        this->synonymStore = storage;
    }

    // Test Methods
    void isValidValidate(std::vector<std::string> inputStatements) {
        validate(inputStatements);
    }

    void isValidDeclareStmt(const std::string& inputString) {
        validateDeclarationStatement(inputString);
    }

    void isValidSelectStmt(const std::string& inputString) {
        validateSelectStatement(inputString);
    }

    void isValidSTClause(const std::string& inputString) {
        validateSuchThatClause(inputString);
    }

    void isValidPatternClause(const std::string& inputString) {
        validatePatternClause(inputString);
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

    REQUIRE_NOTHROW(validator.isValidDeclareStmt(inputString_OneSynonym));
    REQUIRE_NOTHROW(validator.isValidDeclareStmt(inputString_TwoSynonyms));
    REQUIRE_NOTHROW(validator.isValidDeclareStmt(inputString_MultipleSynonyms));

    REQUIRE_THROW_SYNTAX_ERROR(validator.isValidDeclareStmt(inputString_NoSynonyms));
    REQUIRE_THROW_SYNTAX_ERROR(validator.isValidDeclareStmt(inputString_NoSynonyms2));
    REQUIRE_THROW_SYNTAX_ERROR(validator.isValidDeclareStmt(inputString_MissingSemiColon));
    REQUIRE_THROW_SYNTAX_ERROR(validator.isValidDeclareStmt(inputString_SynonymWithSpace));
    REQUIRE_THROW_SYNTAX_ERROR(validator.isValidDeclareStmt(inputString_SynonymInteger));
}

TEST_CASE("isValidSelectStatment") {
    std::string inputString_SelectOne = "Select v";
    std::string inputString_ExtraSemiColon = "Select v;";
    std::string inputString_SelectNothing = "Select";
    std::string inputString_SelectNothingWithSpace = "Select ";
    std::string inputString_OneSuchThat = "Select v such that Follows(1,2)";
    std::string inputString_OnePattern = "Select v pattern a(v,2)";
    std::string inputString_MultipleReturn = "Select <v, a> pattern a(v, \"2\")";
    std::string inputString_MultipleReturn_MissingBracket = "Select v, a> pattern a(v, \"2\")";

    SynonymStore testStore;
    testStore.storeSynonymWithStatement("variable v;");
    testStore.storeSynonymWithStatement("assign a;");
    TestValidator validator(testStore);

    REQUIRE_NOTHROW(validator.isValidSelectStmt(inputString_SelectOne));
    REQUIRE_THROW_SYNTAX_ERROR(validator.isValidSelectStmt(inputString_ExtraSemiColon));
    REQUIRE_THROW_SYNTAX_ERROR(validator.isValidSelectStmt(inputString_SelectNothing));
    REQUIRE_THROW_SYNTAX_ERROR(validator.isValidSelectStmt(inputString_SelectNothingWithSpace));

    REQUIRE_NOTHROW(validator.isValidSelectStmt(inputString_MultipleReturn));
    REQUIRE_THROW_SYNTAX_ERROR(validator.isValidSelectStmt(inputString_MultipleReturn_MissingBracket));
}

TEST_CASE("validateSuchThatClause") {
    SynonymStore testStore;
    testStore.storeSynonymWithStatement("stmt s1, s2;");
    testStore.storeSynonymWithStatement("variable v;");
    testStore.storeSynonymWithStatement("read re;");
    testStore.storeSynonymWithStatement("constant c;");

    TestValidator validator(testStore);

    std::string inputString_Follows = "such that Follows(1,2)";
    REQUIRE_NOTHROW(validator.isValidSTClause(inputString_Follows));

    std::string inputString_FollowsT = "such that Follows*(1,2)";
    REQUIRE_NOTHROW(validator.isValidSTClause(inputString_FollowsT));

    std::string inputString_Follows_Stmt_VALID = "such that Follows(s1,2)";
    REQUIRE_NOTHROW(validator.isValidSTClause(inputString_Follows_Stmt_VALID));

    std::string inputString_Parent_VALID = "such that Parent(re,s1)";
    REQUIRE_NOTHROW(validator.isValidSTClause(inputString_Parent_VALID));

    std::string inputString_Modifies_VALID = "such that Modifies(s1,v)";
    REQUIRE_NOTHROW(validator.isValidSTClause(inputString_Modifies_VALID));

    std::string inputString_Uses_VALID = "such that Uses(s1,v)";
    REQUIRE_NOTHROW(validator.isValidSTClause(inputString_Uses_VALID));

    std::string inputString_Next_VALID = "such that Next(s1,s2)";
    REQUIRE_NOTHROW(validator.isValidSTClause(inputString_Next_VALID));

    std::string inputString_NextT_VALID = "such that Next*(s1,s2)";
    REQUIRE_NOTHROW(validator.isValidSTClause(inputString_NextT_VALID));

    std::string inputString_Next_SynonymInteger_VALID = "such that Next(s1,10)";
    REQUIRE_NOTHROW(validator.isValidSTClause(inputString_Next_SynonymInteger_VALID));

    std::string inputString_Follows_Stmt_INVALID = "such that Follows(s4,2)";
    REQUIRE_THROW_SEMANTIC_ERROR(validator.isValidSTClause(inputString_Follows_Stmt_INVALID));

    std::string inputString_Follows_StarWithSpace_INVALID = "such that Follows *(s1,2)";
    REQUIRE_THROW_SYNTAX_ERROR(validator.isValidSTClause(inputString_Follows_StarWithSpace_INVALID));

    std::string inputString_Parent_FirstArgVariable_INVALID = "such that Parent(v,s1)";
    REQUIRE_THROW_SEMANTIC_ERROR(validator.isValidSTClause(inputString_Parent_FirstArgVariable_INVALID));

    std::string inputString_Modifies_FirstArgWildcard_INVALID = "such that Modifies(_,v)";
    REQUIRE_THROW_SEMANTIC_ERROR(validator.isValidSTClause(inputString_Modifies_FirstArgWildcard_INVALID));

    std::string inputString_Modifies_FirstArgVariable_INVALID = "such that Modifies(v,_)";
    REQUIRE_THROW_SEMANTIC_ERROR(validator.isValidSTClause(inputString_Modifies_FirstArgVariable_INVALID));

    std::string inputString_Modifies_SecondArgVar_INVALID = "such that Modifies(v,s1)";
    REQUIRE_THROW_SEMANTIC_ERROR(validator.isValidSTClause(inputString_Modifies_SecondArgVar_INVALID));

    std::string inputString_Uses_FirstArgWildcard_INVALID = "such that Uses(_,v)";
    REQUIRE_THROW_SEMANTIC_ERROR(validator.isValidSTClause(inputString_Uses_FirstArgWildcard_INVALID));

    std::string inputString_Next_MissingSynonym_INVALID = "such that Next(s1,s100)";
    REQUIRE_THROW_SEMANTIC_ERROR(validator.isValidSTClause(inputString_Next_MissingSynonym_INVALID));

    std::string inputString_Next_SynonymConstant_INVALID = "such that Next(s1,c)";
    REQUIRE_THROW_SEMANTIC_ERROR(validator.isValidSTClause(inputString_Next_SynonymConstant_INVALID));

    // TODO(Han Qin): Check SyntaxError thrown before SemanticError
}

TEST_CASE("validatePatternClause") {
    SynonymStore testStore;
    testStore.storeSynonymWithStatement("assign a;");
    testStore.storeSynonymWithStatement("variable v;");
    testStore.storeSynonymWithStatement("stmt iff;");

    TestValidator validator(testStore);

    std::string inputString_VALID = "pattern a(v, _\"x\"_)";
    REQUIRE_NOTHROW(validator.isValidPatternClause(inputString_VALID));

    std::string inputString_Space_VALID = "pattern a (v, _\"x\"_)";
    REQUIRE_NOTHROW(validator.isValidPatternClause(inputString_Space_VALID));

    std::string inputString_Integer_VALID = "pattern a(v, _\"2\"_)";
    REQUIRE_NOTHROW(validator.isValidPatternClause(inputString_Integer_VALID));

    std::string inputString_Wildcard_VALID = "pattern a(_, _\"x\"_)";
    REQUIRE_NOTHROW(validator.isValidPatternClause(inputString_Wildcard_VALID));

    std::string inputString_Wildcards_VALID = "pattern a(_, _)";
    REQUIRE_NOTHROW(validator.isValidPatternClause(inputString_Wildcards_VALID));

    std::string inputString_INVALID = "pattern a(v, _x\"_)";
    REQUIRE_THROW_SYNTAX_ERROR(validator.isValidPatternClause(inputString_INVALID));

    std::string inputString_INVALID_stmt = "pattern iff (v, _\"x\"_)";
    REQUIRE_THROW_SEMANTIC_ERROR(validator.isValidPatternClause(inputString_INVALID_stmt));
}

TEST_CASE("validateSelectStatement_STPattern_VALID") {
    TestValidator validator;

    std::vector<std::string> inputStatements = {"assign a;", "variable v;",
                                                "Select v such that Follows(1,2) pattern a(v,_\"2\"_)"};
    REQUIRE_NOTHROW(validator.isValidValidate(inputStatements));
}

TEST_CASE("validateSelectStatement_PatternST_VALID") {
    TestValidator validator;

    std::vector<std::string> inputStatements = {"assign a;", "variable v;",
                                                "Select v pattern a(v,_\"2\"_) such that Follows(1,2)"};
    REQUIRE_NOTHROW(validator.isValidValidate(inputStatements));
}

TEST_CASE("validateSelectStatement_UndeclaredSynonym_INVALID") {
    TestValidator validator;
    std::vector<std::string> inputStatements = {"assign a;", "variable v;",
                                                "Select v such that Follows(1,2) pattern a(v1,_\"2\"_)"};
    REQUIRE_THROW_SEMANTIC_ERROR(validator.isValidValidate(inputStatements));
}

TEST_CASE("validateSelectStatement_DuplicateSynonym_INVALID") {
    TestValidator validator;
    std::vector<std::string> inputStatements = {"assign a;", "variable v, v;",
                                                "Select v such that Follows(1,2) pattern a(v,_\"2\"_)"};
    REQUIRE_THROW_SEMANTIC_ERROR(validator.isValidValidate(inputStatements));
}

// Order of Select and declaration is not checked.
// However, it doesn't have to be since even if it is a valid Select statement,
// it would not be able to retrieve the synonyms, resulting in Semantic Error
// Wrong error will be thrown (Semantic instead of Syntax).
TEST_CASE("validateSelectStatement_INVALID") {
    TestValidator validator;
    std::vector<std::string> inputStatements = {"Select v such that Follows(1,2) pattern a(v,_\"2\"_)", "assign a;",
                                                "variable v, v;"};
    REQUIRE_THROW_SYNTAX_ERROR(validator.isValidValidate(inputStatements));
}
