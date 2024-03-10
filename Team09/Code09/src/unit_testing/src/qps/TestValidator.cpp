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

    SECTION("One synonym_NoThrow") {
        std::string oneSyn_valid = "assign a;";
        REQUIRE_NOTHROW(validator.isValidDeclareStmt(oneSyn_valid));
    }

    SECTION("Two synonyms_NoThrow") {
        std::string twoSyn_valid = "assign a, b;";
        REQUIRE_NOTHROW(validator.isValidDeclareStmt(twoSyn_valid));
    }

    SECTION("Multiple synonyms_NoThrow") {
        std::string multipleSyn_valid = "assign a, b, c;";
        REQUIRE_NOTHROW(validator.isValidDeclareStmt(multipleSyn_valid));
    }

    SECTION("No synonyms_ThrowSyntaxError") {
        std::string noSyn_syntax = "assign;";
        REQUIRE_THROW_SYNTAX_ERROR(validator.isValidDeclareStmt(noSyn_syntax));
    }

    SECTION("No synonyms with extra space_ThrowSyntaxError") {
        std::string noSynWithSpace_syntax = "assign ;";
        REQUIRE_THROW_SYNTAX_ERROR(validator.isValidDeclareStmt(noSynWithSpace_syntax));
    }

    SECTION("Missing semi-colon_ThrowSyntaxError") {
        std::string missingSemiColon_syntax = "assign a";
        REQUIRE_THROW_SYNTAX_ERROR(validator.isValidDeclareStmt(missingSemiColon_syntax));
    }

    SECTION("Multiple synonyms without comma_ThrowSyntaxError") {
        std::string missingComma_syntax = "assign a b;";
        REQUIRE_THROW_SYNTAX_ERROR(validator.isValidDeclareStmt(missingComma_syntax));
    }

    SECTION("Integer as synonym_ThrowSyntaxError") {
        std::string integer_syntax = "assign 1;";
        REQUIRE_THROW_SYNTAX_ERROR(validator.isValidDeclareStmt(integer_syntax));
    }

    SECTION("Extra comma_ThrowSyntaxError") {
        std::string extraComma_syntax = "assign a, b,;";
        REQUIRE_THROW_SYNTAX_ERROR(validator.isValidDeclareStmt(extraComma_syntax));
    }
}

TEST_CASE("isValidSelectStatment") {
    SynonymStore testStore;
    testStore.storeSynonymWithStatement("variable v;");
    testStore.storeSynonymWithStatement("assign a;");
    TestValidator validator(testStore);

    SECTION("Select one variable_NoThrow") {
        std::string one = "Select v";
        REQUIRE_NOTHROW(validator.isValidSelectStmt(one));
    }

    SECTION("Select one variable with such that clause_NoThrow") {
        std::string oneST = "Select v such that Follows(1,2)";
        REQUIRE_NOTHROW(validator.isValidSelectStmt(oneST));
    }

    SECTION("Select multiple variables_NoThrow") {
        std::string multi = "Select <v, a>";
        REQUIRE_NOTHROW(validator.isValidSelectStmt(multi));
    }

    SECTION("Select multiple variables with one clause_NoThrow") {
        std::string multiPattern = "Select <v, a> pattern a(v, \"2\")";
        REQUIRE_NOTHROW(validator.isValidSelectStmt(multiPattern));
    }

    SECTION("Extra semi-colon_ThrowSyntaxError") {
        std::string extraSemiColon = "Select v;";
        REQUIRE_THROW_SYNTAX_ERROR(validator.isValidSelectStmt(extraSemiColon));
    }

    SECTION("No Synonyms_ThrowSyntaxError") {
        std::string selectNothing = "Select";
        REQUIRE_THROW_SYNTAX_ERROR(validator.isValidSelectStmt(selectNothing));
    }

    SECTION("No Synonyms with space_ThrowSyntaxError") {
        std::string selectSpace = "Select ";
        REQUIRE_THROW_SYNTAX_ERROR(validator.isValidSelectStmt(selectSpace));
    }

    SECTION("Missing angular bracket for multiple returns_ThrowSyntaxError") {
        std::string missingLessThan = "Select v, a> pattern a(v, \"2\")";
        REQUIRE_THROW_SYNTAX_ERROR(validator.isValidSelectStmt(missingLessThan));
    }

    SECTION("Possible semantic error before syntax error_ThrowSyntaxError") {
        std::string semanticBeforeSyntax = "Select v such that Uses(v, _) pattern a(1, _)";
        REQUIRE_THROW_SYNTAX_ERROR(validator.isValidSelectStmt(semanticBeforeSyntax));
    }
}

TEST_CASE("validateSuchThatClause") {
    SynonymStore testStore;
    testStore.storeSynonymWithStatement("stmt s1, s2;");
    testStore.storeSynonymWithStatement("variable v;");
    testStore.storeSynonymWithStatement("read re;");
    testStore.storeSynonymWithStatement("constant c;");

    TestValidator validator(testStore);

    SECTION("Follows Integer, Integer_NoThrow") {
        std::string followsIntInt = "such that Follows(1,2)";
        REQUIRE_NOTHROW(validator.isValidSTClause(followsIntInt));
    }

    SECTION("Follows* Integer, Integer_NoThrow") {
        std::string followsTIntInt = "such that Follows*(1,2)";
        REQUIRE_NOTHROW(validator.isValidSTClause(followsTIntInt));
    }

    SECTION("Follows Statement, Integer_NoThrow") {
        std::string followStmtInt = "such that Follows(s1,2)";
        REQUIRE_NOTHROW(validator.isValidSTClause(followStmtInt));
    }

    SECTION("Follows Integer, Statement_NoThrow") {
        std::string followsIntStmt = "such that Follows(1,s1)";
        REQUIRE_NOTHROW(validator.isValidSTClause(followsIntStmt));
    }

    SECTION("Follows Statement, Statement_NoThrow") {
        std::string followsStmtStmt = "such that Follows(s1,s2)";
        REQUIRE_NOTHROW(validator.isValidSTClause(followsStmtStmt));
    }

    SECTION("Follows Wildcard, Statement_NoThrow") {
        std::string followsWildcardStmt = "such that Follows(_,s2)";
        REQUIRE_NOTHROW(validator.isValidSTClause(followsWildcardStmt));
    }

    SECTION("Parent Read, Statement_NoThrow") {
        std::string parentReadStmt = "such that Parent(re,s1)";
        REQUIRE_NOTHROW(validator.isValidSTClause(parentReadStmt));
    }

    SECTION("Modifies Read, Variable_NoThrow") {
        std::string modifiesReadVar = "such that Modifies(re,v)";
        REQUIRE_NOTHROW(validator.isValidSTClause(modifiesReadVar));
    }

    SECTION("Uses Read, Variable_NoThrow") {
        std::string usesStmtVar = "such that Uses(s1,v)";
        REQUIRE_NOTHROW(validator.isValidSTClause(usesStmtVar));
    }

    SECTION("Next Stmt, Read_NoThrow") {
        std::string nextStmtRead = "such that Next(s1,re)";
        REQUIRE_NOTHROW(validator.isValidSTClause(nextStmtRead));
    }

    SECTION("Next Stmt, Integer_NoThrow") {
        std::string nextStmtInt = "such that Next(s1,10)";
        REQUIRE_NOTHROW(validator.isValidSTClause(nextStmtInt));
    }

    SECTION("Follows Missing Synonym in Store_ThrowSemanticError") {
        std::string followsNoSynInStore = "such that Follows(s4,2)";
        REQUIRE_THROW_SEMANTIC_ERROR(validator.isValidSTClause(followsNoSynInStore));
    }

    SECTION("Follows *_ThrowSyntaxError") {
        std::string followsSpaceStar = "such that Follows *(s1,2)";
        REQUIRE_THROW_SYNTAX_ERROR(validator.isValidSTClause(followsSpaceStar));
    }

    SECTION("Parent first Argument Variable_ThrowSemanticError") {
        std::string parentFirstArgVar = "such that Parent(v,s1)";
        REQUIRE_THROW_SEMANTIC_ERROR(validator.isValidSTClause(parentFirstArgVar));
    }

    SECTION("Parent second Argument Variable_ThrowSemanticError") {
        std::string parentSecondArgVar = "such that Parent(s1,v)";
        REQUIRE_THROW_SEMANTIC_ERROR(validator.isValidSTClause(parentSecondArgVar));
    }

    SECTION("Modifies first Argument Wildcard_ThrowSemanticError") {
        std::string modifiesFirstArgWildcard = "such that Modifies(_,v)";
        REQUIRE_THROW_SEMANTIC_ERROR(validator.isValidSTClause(modifiesFirstArgWildcard));
    }

    SECTION("Modifies first Argument Variable_ThrowSemanticError") {
        std::string modifiesFirstArgVar = "such that Modifies(v,_)";
        REQUIRE_THROW_SEMANTIC_ERROR(validator.isValidSTClause(modifiesFirstArgVar));
    }

    SECTION("Modifies second Argument Stmt_ThrowSemanticError") {
        std::string modifiesSecondArgStmt = "such that Modifies(v,s1)";
        REQUIRE_THROW_SEMANTIC_ERROR(validator.isValidSTClause(modifiesSecondArgStmt));
    }

    SECTION("Uses first Argument Wildcard_ThrowSemanticError") {
        std::string usesFirstArgWildcard = "such that Uses(_,v)";
        REQUIRE_THROW_SEMANTIC_ERROR(validator.isValidSTClause(usesFirstArgWildcard));
    }

    SECTION("Next Missing Synonym in Store_ThrowSemantic") {
        std::string nextSynNotInStore = "such that Next(s1,s100)";
        REQUIRE_THROW_SEMANTIC_ERROR(validator.isValidSTClause(nextSynNotInStore));
    }

    SECTION("Next Stmt, Constant_ThrowSemanticError") {
        std::string nextSynConst = "such that Next(s1,c)";
        REQUIRE_THROW_SEMANTIC_ERROR(validator.isValidSTClause(nextSynConst));
    }
}

TEST_CASE("validatePatternClause") {
    SynonymStore testStore;
    testStore.storeSynonymWithStatement("assign a;");
    testStore.storeSynonymWithStatement("variable v;");
    testStore.storeSynonymWithStatement("stmt iff;");

    TestValidator validator(testStore);

    SECTION("Assign(Variable, ExprSpecName)_NoThrow") {
        std::string assignVarExprSpecName = "pattern a(v, _\"x\"_)";
        REQUIRE_NOTHROW(validator.isValidPatternClause(assignVarExprSpecName));
    }

    SECTION("Assign (Variable, ExprSpec)_NoThrow") {
        std::string assignVarExprSpecSpace = "pattern a (v, _\"x\"_)";
        REQUIRE_NOTHROW(validator.isValidPatternClause(assignVarExprSpecSpace));
    }

    SECTION("Assign(Variable, ExprSpecInt)_NoThrow") {
        std::string assignVarExprSpecInt = "pattern a(v, _\"2\"_)";
        REQUIRE_NOTHROW(validator.isValidPatternClause(assignVarExprSpecInt));
    }

    SECTION("Assign(Wildcard, ExprSpecName)_NoThrow") {
        std::string assignWildcardExprSpecName = "pattern a(_, _\"x\"_)";
        REQUIRE_NOTHROW(validator.isValidPatternClause(assignWildcardExprSpecName));
    }

    SECTION("Assign(Wildcard, Wildcard)_NoThrow") {
        std::string assignWildcardWildcard = "pattern a(_, _)";
        REQUIRE_NOTHROW(validator.isValidPatternClause(assignWildcardWildcard));
    }

    SECTION("Assign(Wildcard, LongExprSpec)_NoThrow") {
        std::string assignWildcardLongExprSpec = "pattern a ( _ , _ \" ( x + y ) / 10 + ( 2 * x ) \" _ )";
        REQUIRE_NOTHROW(validator.isValidPatternClause(assignWildcardLongExprSpec));
    }

    SECTION("Assign(Wildcard, MissingQuoteName)_ThrowSyntaxError") {
        std::string missingQuote = "pattern a(v, _x\"_)";
        REQUIRE_THROW_SYNTAX_ERROR(validator.isValidPatternClause(missingQuote));
    }

    SECTION("Assign Not In Store(Wildcard, MissingQuoteName)_ThrowSyntaxError") {
        std::string notInStoreAndMissingQuote = "pattern a1(v, _x\"_)";
        REQUIRE_THROW_SYNTAX_ERROR(validator.isValidPatternClause(notInStoreAndMissingQuote));
    }

    SECTION("Assign Not Synonym(Variable, ExprSpecName)_ThrowSyntaxError") {
        std::string assignVarExprSpecName = "pattern 1 (v, _\"x\"_)";
        REQUIRE_THROW_SYNTAX_ERROR(validator.isValidPatternClause(assignVarExprSpecName));
    }

    SECTION("Statement(Variable, ExprSpecName)_ThrowSemanticError") {
        std::string stmtVarExprSpecName = "pattern iff (v, _\"x\"_)";
        REQUIRE_THROW_SEMANTIC_ERROR(validator.isValidPatternClause(stmtVarExprSpecName));
    }

    SECTION("Assign Not In Store(Variable, ExprSpecName)_ThrowSemanticError") {
        std::string assignVarExprSpecName = "pattern a1 (v, _\"x\"_)";
        REQUIRE_THROW_SEMANTIC_ERROR(validator.isValidPatternClause(assignVarExprSpecName));
    }
}

TEST_CASE("validate") {
    TestValidator validator;
    SECTION("such that and pattern_NoThrow") {
        std::vector<std::string> inputStatements = {"assign a;", "variable v;",
                                                    "Select v such that Follows(1,2) pattern a(v,_\"2\"_)"};
        REQUIRE_NOTHROW(validator.isValidValidate(inputStatements));
    }

    SECTION("pattern and such that_NoThrow") {
        std::vector<std::string> inputStatements = {"assign a;", "variable v;",
                                                    "Select v pattern a(v,_\"2\"_) such that Follows(1,2)"};
        REQUIRE_NOTHROW(validator.isValidValidate(inputStatements));
    }

    SECTION("Missing declaration_ThrowSemanticError") {
        std::vector<std::string> inputStatements = {"assign a;", "variable v;",
                                                    "Select v such that Follows(1,2) pattern a(v1,_\"2\"_)"};
        REQUIRE_THROW_SEMANTIC_ERROR(validator.isValidValidate(inputStatements));
    }

    SECTION("Duplicate synonym same line_ThrowSemanticError") {
        std::vector<std::string> inputStatements = {"assign a;", "variable v, v;",
                                                    "Select v such that Follows(1,2) pattern a(v,_\"2\"_)"};
        REQUIRE_THROW_SEMANTIC_ERROR(validator.isValidValidate(inputStatements));
    }

    SECTION("Duplicate synonym different line_ThrowSemanticError") {
        std::vector<std::string> inputStatements = {"variable v;", "assign a;", "variable v;",
                                                    "Select v such that Follows(1,2) pattern a(v,_\"2\"_)"};
        REQUIRE_THROW_SEMANTIC_ERROR(validator.isValidValidate(inputStatements));
    }

    SECTION("Select before declaration_ThrowSyntaxError") {
        std::vector<std::string> inputStatements = {"Select v such that Follows(1,2) pattern a(v,_\"2\"_)", "assign a;",
                                                    "variable v, v;"};
        REQUIRE_THROW_SYNTAX_ERROR(validator.isValidValidate(inputStatements));
    }

    SECTION("Two Selects_ThrowSyntaxError") {
        std::vector<std::string> inputStatements = {"variable v;", "assign a;", "Select v;",
                                                    "Select v such that Follows(1,2) pattern a(v,_\"2\"_)"};
        REQUIRE_THROW_SYNTAX_ERROR(validator.isValidValidate(inputStatements));
    }
}
