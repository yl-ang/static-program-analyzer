#include <algorithm>

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

    Literal literal = Literal("1");
    SECTION("Test Wildcard and Wildcard") {
        Wildcard wildcard = Wildcard();

        PatternClause pc = PatternClause(&assignSyn, &wildcard, &wildcard);
        ClauseResult result = pc.evaluate(reader);
        REQUIRE_FALSE(result.isBoolean());
        REQUIRE(result.getSynonyms() == std::vector<Synonym>{assignSyn});
        std::vector<SynonymValues> synValues = result.getAllSynonymValues();
        // REQUIRE(synValues == std::vector<SynonymValues>{{"2", "3", "4"}}); KIV
    }

    SECTION("Test Wildcard and ExpressionSpec") {
        Wildcard wildcard = Wildcard();
        ExpressionSpec varExp = ExpressionSpec("_\"y\"_");
        ExpressionSpec literalExp = ExpressionSpec("_\"2\"_");
        ExpressionSpec literalExp_False = ExpressionSpec("_\"1\"_");

        PatternClause pc = PatternClause(&assignSyn, &wildcard, &varExp);
        ClauseResult result = pc.evaluate(reader);
        REQUIRE_FALSE(result.isBoolean());
        REQUIRE(result.getSynonyms() == std::vector<Synonym>{assignSyn});
        std::vector<SynonymValues> synValues = result.getAllSynonymValues();
        REQUIRE(synValues == std::vector<SynonymValues>{{"2"}});

        PatternClause pc2 = PatternClause(&assignSyn, &wildcard, &literalExp);
        result = pc2.evaluate(reader);
        REQUIRE_FALSE(result.isBoolean());
        REQUIRE(result.getSynonyms() == std::vector<Synonym>{assignSyn});
        synValues = result.getAllSynonymValues();
        // REQUIRE(synValues == std::vector<SynonymValues>{{"3", "4"}});

        PatternClause pc3 = PatternClause(&assignSyn, &wildcard, &literalExp_False);
        result = pc3.evaluate(reader);
        REQUIRE_FALSE(result.isBoolean());
        REQUIRE(result.getSynonyms() == std::vector<Synonym>{assignSyn});
        synValues = result.getAllSynonymValues();
        REQUIRE(synValues == std::vector<SynonymValues>{{}});
    }

    SECTION("Test Synonym and Wildcard") {
        Synonym variableSyn = Synonym(DesignEntityType::VARIABLE, "v");
        Wildcard wildcard = Wildcard();

        PatternClause pc = PatternClause(&assignSyn, &variableSyn, &wildcard);
        ClauseResult result = pc.evaluate(reader);
        REQUIRE_FALSE(result.isBoolean());
        REQUIRE(result.getSynonyms() == std::vector<Synonym>{assignSyn, variableSyn});
        std::vector<SynonymValues> synValues = result.getAllSynonymValues();
        // REQUIRE(synValues == std::vector<SynonymValues>{{"2", "3", "4"}, {"x", "x", "x"}});
    }

    SECTION("Test Synonym and ExpressionSpec") {
        Synonym variableSyn = Synonym(DesignEntityType::VARIABLE, "v");
        ExpressionSpec varExp = ExpressionSpec("_\"y\"_");
        ExpressionSpec literalExp = ExpressionSpec("_\"2\"_");
        ExpressionSpec literalExp_False = ExpressionSpec("_\"1\"_");

        PatternClause pc = PatternClause(&assignSyn, &variableSyn, &varExp);
        ClauseResult result = pc.evaluate(reader);
        REQUIRE_FALSE(result.isBoolean());
        REQUIRE(result.getSynonyms() == std::vector<Synonym>{assignSyn, variableSyn});
        std::vector<SynonymValues> synValues = result.getAllSynonymValues();
        REQUIRE(synValues == std::vector<SynonymValues>{{"2"}, {"x"}});

        PatternClause pc2 = PatternClause(&assignSyn, &variableSyn, &literalExp);
        result = pc2.evaluate(reader);
        REQUIRE_FALSE(result.isBoolean());
        REQUIRE(result.getSynonyms() == std::vector<Synonym>{assignSyn, variableSyn});
        synValues = result.getAllSynonymValues();
        // REQUIRE(synValues == std::vector<SynonymValues>{{"3", "4"}, {"x", "x"}});

        PatternClause pc3 = PatternClause(&assignSyn, &variableSyn, &literalExp_False);
        result = pc3.evaluate(reader);
        REQUIRE_FALSE(result.isBoolean());
        REQUIRE(result.getSynonyms() == std::vector<Synonym>{assignSyn, variableSyn});
        synValues = result.getAllSynonymValues();
        REQUIRE(synValues == std::vector<SynonymValues>{{}, {}});
    }
}
