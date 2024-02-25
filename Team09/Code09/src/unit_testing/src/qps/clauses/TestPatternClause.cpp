#include "PKB/PKBClient/PKBFacadeWriter.h"
#include "catch.hpp"
#include "qps/clauseArguments/Wildcard.h"
#include "qps/clauses/PatternClause.h"

TEST_CASE("PatternClause evaluate") {
    PKB pkb{};
    PKBFacadeWriter writer{pkb};
    PKBFacadeReader reader{pkb};

    /*
     * procedure test {
     *   read y;
     *   x = x + y;
     *   x = 2 * x;
     *   x = 1;
     *   print z;
     */
    std::unordered_set<Stmt> stmtStoreEntries = {{StatementType::READ, 1},
                                                 {StatementType::ASSIGN, 2},
                                                 {StatementType::ASSIGN, 3},
                                                 {StatementType::ASSIGN, 4},
                                                 {StatementType::PRINT, 5}};
    writer.setStmts(stmtStoreEntries);

    std::unordered_set<Variable> variableStoreEntries = {"x", "y", "z"};
    writer.setVariables(variableStoreEntries);

    std::unordered_set<Constant> constantStoreEntries = {"1", "2"};
    writer.setConstants(constantStoreEntries);

    std::unordered_set<std::pair<StmtNum, std::pair<std::string, std::string>>> patternStoreEntries = {
        {2, {"x", "x"}}, {2, {"x", "y"}}, {3, {"x", "2"}}, {3, {"x", "x"}}, {4, {"x", "2"}}};
    writer.setPatternStore(patternStoreEntries);

    Synonym assignSyn = Synonym(DesignEntityType::ASSIGN, "a");

    Synonym variableSyn = Synonym(DesignEntityType::VARIABLE, "v");
    Literal literal = Literal("1");
    ExpressionSpec varExp = ExpressionSpec("_\"y\"_");
    ExpressionSpec literalExp = ExpressionSpec("_\"1\"_");
    SECTION("Test Wildcard and Wildcard") {
        Wildcard wildcard = Wildcard();

        PatternClause pc = PatternClause(&assignSyn, &wildcard, &wildcard);
        ClauseResult result = pc.evaluate(reader);
        REQUIRE_FALSE(result.isBoolean());
        std::vector<Synonym> resSyn = result.getSynonyms();
        result.getAllSynonymValues();
    }
}
