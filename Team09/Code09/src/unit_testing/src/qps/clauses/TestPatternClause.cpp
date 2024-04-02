#include <algorithm>

#include "PKB/PKBClient/PKBFacadeWriter.h"
#include "catch.hpp"
#include "qps/clauseArguments/Wildcard.h"
#include "qps/clauses/PatternClause.h"
#include "sp/Utils.h"

bool equalVector(std::vector<std::string> vec1, std::vector<std::string> vec2) {
    std::sort(vec1.begin(), vec1.end());

    std::sort(vec2.begin(), vec2.end());

    return vec1 == vec2;
}

#define REQUIRE_EQUAL_VECTORS(vec1, vec2) REQUIRE(equalVector(vec1, vec2))

TEST_CASE("Assign Pattern evaluate") {
    PKB pkb{};
    PKBFacadeWriter writer{pkb};
    PKBFacadeReader reader{pkb};

    /*
     * procedure test {
     *   read y;
     *   x = x + y;
     *   x = 2 * x;
     *   x = 2;
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

    std::unordered_set<std::pair<StmtNum, std::pair<std::string, std::string>>> assignPatternStoreEntries = {
        {2, {"x # # ", "add x # # y # # "}}, {3, {"x # # ", "mul 2 # # x # # "}}, {4, {"x # # ", "2 # # "}}};
    writer.setAssignPatternStore(isExactMatch, isPartialMatch, assignPatternStoreEntries);

    std::shared_ptr<Synonym> assignSyn = std::make_shared<Synonym>(DesignEntityType::ASSIGN, "a");

    std::shared_ptr<Literal> literal = std::make_shared<Literal>("1");
    SECTION("Test Wildcard and Wildcard") {
        std::shared_ptr<Wildcard> wildcard = std::make_shared<Wildcard>();

        PatternClause pc = PatternClause(assignSyn, {wildcard, wildcard});
        ClauseResult result = pc.runEvaluation(reader);
        REQUIRE_FALSE(result.isBoolean());
        REQUIRE(result.getSynonyms() == std::vector<Synonym>{*assignSyn});
        std::vector<SynonymValues> synValues = result.getAllSynonymValues();
        REQUIRE_EQUAL_VECTORS(synValues[0], std::vector<std::string>({"2", "3", "4"}));
    }

    SECTION("Test Wildcard and ExpressionSpec") {
        std::shared_ptr<Wildcard> wildcard = std::make_shared<Wildcard>();
        std::shared_ptr<ExpressionSpec> varExp = std::make_shared<ExpressionSpec>("_\"y\"_");
        std::shared_ptr<ExpressionSpec> literalExp = std::make_shared<ExpressionSpec>("_\"2\"_");
        std::shared_ptr<ExpressionSpec> literalExp_False = std::make_shared<ExpressionSpec>("_\"1\"_");

        PatternClause pc = PatternClause(assignSyn, {wildcard, varExp});
        ClauseResult result = pc.runEvaluation(reader);
        REQUIRE_FALSE(result.isBoolean());
        REQUIRE(result.getSynonyms() == std::vector<Synonym>{*assignSyn});
        std::vector<SynonymValues> synValues = result.getAllSynonymValues();
        REQUIRE(synValues == std::vector<SynonymValues>{{"2"}});

        PatternClause pc2 = PatternClause(assignSyn, {wildcard, literalExp});
        result = pc2.runEvaluation(reader);
        REQUIRE_FALSE(result.isBoolean());
        REQUIRE(result.getSynonyms() == std::vector<Synonym>{*assignSyn});
        synValues = result.getAllSynonymValues();
        REQUIRE_EQUAL_VECTORS(synValues[0], std::vector<std::string>({"3", "4"}));

        PatternClause pc3 = PatternClause(assignSyn, {wildcard, literalExp_False});
        result = pc3.runEvaluation(reader);
        REQUIRE_FALSE(result.isBoolean());
        REQUIRE(result.getSynonyms() == std::vector<Synonym>{*assignSyn});
        synValues = result.getAllSynonymValues();
        REQUIRE(synValues == std::vector<SynonymValues>{{}});
    }

    SECTION("Test Synonym and Wildcard") {
        std::shared_ptr<Synonym> variableSyn = std::make_shared<Synonym>(DesignEntityType::VARIABLE, "v");
        std::shared_ptr<Wildcard> wildcard = std::make_shared<Wildcard>();

        PatternClause pc = PatternClause(assignSyn, {variableSyn, wildcard});
        ClauseResult result = pc.runEvaluation(reader);
        REQUIRE_FALSE(result.isBoolean());
        REQUIRE(result.getSynonyms() == std::vector<Synonym>{*assignSyn, *variableSyn});
        std::vector<SynonymValues> synValues = result.getAllSynonymValues();
        REQUIRE_EQUAL_VECTORS(synValues[0], std::vector<std::string>({"2", "3", "4"}));
        REQUIRE_EQUAL_VECTORS(synValues[1], std::vector<std::string>({"x", "x", "x"}));
    }

    SECTION("Test Synonym and ExpressionSpec") {
        std::shared_ptr<Synonym> variableSyn = std::make_shared<Synonym>(DesignEntityType::VARIABLE, "v");
        std::shared_ptr<ExpressionSpec> varExp = std::make_shared<ExpressionSpec>("_\"(x+y)\"_");
        std::shared_ptr<ExpressionSpec> literalExp = std::make_shared<ExpressionSpec>("_\"2\"_");
        std::shared_ptr<ExpressionSpec> literalExp_False = std::make_shared<ExpressionSpec>("_\"1\"_");

        PatternClause pc = PatternClause(assignSyn, {variableSyn, varExp});
        ClauseResult result = pc.runEvaluation(reader);
        REQUIRE_FALSE(result.isBoolean());
        REQUIRE(result.getSynonyms() == std::vector<Synonym>{*assignSyn, *variableSyn});
        std::vector<SynonymValues> synValues = result.getAllSynonymValues();
        REQUIRE(synValues == std::vector<SynonymValues>{{"2"}, {"x"}});

        PatternClause pc2 = PatternClause(assignSyn, {variableSyn, literalExp});
        result = pc2.runEvaluation(reader);
        REQUIRE_FALSE(result.isBoolean());
        REQUIRE(result.getSynonyms() == std::vector<Synonym>{*assignSyn, *variableSyn});
        synValues = result.getAllSynonymValues();
        REQUIRE_EQUAL_VECTORS(synValues[0], std::vector<std::string>({"4", "3"}));
        REQUIRE_EQUAL_VECTORS(synValues[1], std::vector<std::string>({"x", "x"}));

        PatternClause pc3 = PatternClause(assignSyn, {variableSyn, literalExp_False});
        result = pc3.runEvaluation(reader);
        REQUIRE_FALSE(result.isBoolean());
        REQUIRE(result.getSynonyms() == std::vector<Synonym>{*assignSyn, *variableSyn});
        synValues = result.getAllSynonymValues();
        REQUIRE(synValues == std::vector<SynonymValues>{{}, {}});
    }
}

// ai-gen start(copilot, 0, e)
// prompt: use copilot
TEST_CASE("Test pattern getSynonyms") {
    std::shared_ptr<Synonym> assignSyn = std::make_shared<Synonym>(DesignEntityType::ASSIGN, "a");
    std::shared_ptr<Synonym> variableSyn = std::make_shared<Synonym>(DesignEntityType::VARIABLE, "v");
    std::shared_ptr<Wildcard> wildcard = std::make_shared<Wildcard>();
    std::shared_ptr<ExpressionSpec> varExp = std::make_shared<ExpressionSpec>("_\"y\"_");
    std::shared_ptr<ExpressionSpec> literalExp = std::make_shared<ExpressionSpec>("_\"2\"_");

    SECTION("Test Wildcard and Wildcard") {
        PatternClause pc = PatternClause(assignSyn, {wildcard, wildcard});
        std::vector<Synonym> synonyms = pc.getSynonyms();
        REQUIRE(synonyms == std::vector<Synonym>{*assignSyn});
    }

    SECTION("Test variable") {
        PatternClause pc = PatternClause(assignSyn, {variableSyn, varExp});
        std::vector<Synonym> synonyms = pc.getSynonyms();
        REQUIRE(synonyms == std::vector<Synonym>{*assignSyn, *variableSyn});
    }
    SECTION("Test wildcard and literal") {
        PatternClause pc3 = PatternClause(assignSyn, {wildcard, literalExp});
        std::vector<Synonym> synonyms = pc3.getSynonyms();
        REQUIRE(synonyms == std::vector<Synonym>{*assignSyn});
    }
}
// ai-gen end

TEST_CASE("While Pattern evaluate") {
    PKB pkb{};
    PKBFacadeWriter writer{pkb};
    PKBFacadeReader reader{pkb};

    /*
     * procedure test {
     *   read y;1
     *   while (1 == 2) {2
     *     while (x == y || abc == 1) {3
     *        print z;4
     *        z = y;5
     *     }
     *   }
     *   while (z == y) {6
     *     s = w;7
     *     while (234 == 42) {8
     *       read r;9
     *     }
     *   }
     * }
     */
    std::unordered_set<Stmt> stmtStoreEntries = {
        {StatementType::READ, 1},   {StatementType::WHILE, 2},  {StatementType::WHILE, 3},
        {StatementType::PRINT, 4},  {StatementType::ASSIGN, 5}, {StatementType::WHILE, 6},
        {StatementType::ASSIGN, 7}, {StatementType::WHILE, 8},  {StatementType::READ, 9}};
    writer.setStmts(stmtStoreEntries);

    std::unordered_set<Variable> variableStoreEntries = {"x", "y", "z", "abc", "s", "w", "r"};
    writer.setVariables(variableStoreEntries);

    std::unordered_set<std::pair<StmtNum, Variable>> whilePatternEntries = {
        {3, "x"}, {3, "y"}, {3, "abc"}, {6, "z"}, {6, "y"}};
    writer.setWhilePatternStore(whilePatternEntries);

    std::vector<std::string> allWhileStmtsWithVariables{"3", "3", "3", "6", "6"};
    std::vector<std::string> allVarsInWhile{"x", "y", "abc", "z", "y"};

    std::shared_ptr<Synonym> whileSyn = std::make_shared<Synonym>(DesignEntityType::WHILE, "w");
    std::shared_ptr<Synonym> varSyn = std::make_shared<Synonym>(DesignEntityType::VARIABLE, "v");
    std::shared_ptr<Literal> literalValue = std::make_shared<Literal>("x");
    std::shared_ptr<Wildcard> wildcard = std::make_shared<Wildcard>();

    SECTION("While(Variable, _)") {
        PatternClause whilePattern = {whileSyn, {varSyn, wildcard}};
        ClauseResult clauseResult = whilePattern.runEvaluation(reader);
        std::vector<Synonym> synResults = clauseResult.getSynonyms();
        std::vector<SynonymValues> valueResults = clauseResult.getAllSynonymValues();
        REQUIRE(synResults == std::vector<Synonym>{*whileSyn, *varSyn});
        REQUIRE_EQUAL_VECTORS(valueResults[0], allWhileStmtsWithVariables);
        REQUIRE_EQUAL_VECTORS(valueResults[1], allVarsInWhile);
    }

    SECTION("While(\"x\", _)") {
        PatternClause whilePattern = {whileSyn, {literalValue, wildcard}};
        ClauseResult clauseResult = whilePattern.runEvaluation(reader);
        std::vector<Synonym> synResults = clauseResult.getSynonyms();
        std::vector<SynonymValues> valueResults = clauseResult.getAllSynonymValues();
        REQUIRE(synResults == std::vector<Synonym>{*whileSyn});
        REQUIRE_EQUAL_VECTORS(valueResults[0], {"3"});
    }

    SECTION("While(Literal not in store, _)") {
        std::shared_ptr<Literal> notInStoreLiteral = std::make_shared<Literal>("l");
        PatternClause whilePattern = {whileSyn, {notInStoreLiteral, wildcard}};
        ClauseResult clauseResult = whilePattern.runEvaluation(reader);
        std::vector<Synonym> synResults = clauseResult.getSynonyms();
        std::vector<SynonymValues> valueResults = clauseResult.getAllSynonymValues();
        REQUIRE(synResults == std::vector<Synonym>{*whileSyn});
        REQUIRE_EQUAL_VECTORS(valueResults[0], {});
    }

    SECTION("While(Literal in store not in condition, _)") {
        std::shared_ptr<Literal> notInStoreLiteral = std::make_shared<Literal>("s");
        PatternClause whilePattern = {whileSyn, {notInStoreLiteral, wildcard}};
        ClauseResult clauseResult = whilePattern.runEvaluation(reader);
        std::vector<Synonym> synResults = clauseResult.getSynonyms();
        std::vector<SynonymValues> valueResults = clauseResult.getAllSynonymValues();
        REQUIRE(synResults == std::vector<Synonym>{*whileSyn});
        REQUIRE_EQUAL_VECTORS(valueResults[0], {});
    }

    SECTION("While(_, _)") {
        PatternClause whilePattern = {whileSyn, {wildcard, wildcard}};
        ClauseResult clauseResult = whilePattern.runEvaluation(reader);
        std::vector<Synonym> synResults = clauseResult.getSynonyms();
        std::vector<SynonymValues> valueResults = clauseResult.getAllSynonymValues();
        REQUIRE(synResults == std::vector<Synonym>{*whileSyn});
        REQUIRE_EQUAL_VECTORS(valueResults[0], {allWhileStmtsWithVariables});
    }
}

TEST_CASE("If Pattern evaluate") {
    PKB pkb{};
    PKBFacadeWriter writer{pkb};
    PKBFacadeReader reader{pkb};

    /*
     * procedure test {
     *   read y;1
     *   if (1 == 2) then{2
     *     read y;3
     *   } else {
     *     read y;4
     *   }
     *   if (x == y) then {5
     *     if (z == w || w > y) {6
     *       read y;7
     *     } else {
     *       read y;8
     *     }
     *   else {
     *     if (2 == z) then {9
     *       read abc;10
     *     } else {
     *       read xyz;11
     *     }
     *   }
     *  print 1;12
     */
    std::unordered_set<Stmt> stmtStoreEntries = {
        {StatementType::READ, 1}, {StatementType::IF, 2},    {StatementType::READ, 3},  {StatementType::READ, 4},
        {StatementType::IF, 5},   {StatementType::IF, 6},    {StatementType::READ, 7},  {StatementType::READ, 8},
        {StatementType::IF, 9},   {StatementType::READ, 10}, {StatementType::READ, 11}, {StatementType::PRINT, 12}};
    writer.setStmts(stmtStoreEntries);

    std::unordered_set<Variable> variableStoreEntries = {"x", "y", "z", "abc", "xyz", "w"};
    writer.setVariables(variableStoreEntries);

    std::unordered_set<std::pair<StmtNum, Variable>> ifPatternEntries = {{5, "x"}, {5, "y"}, {6, "z"},
                                                                         {6, "w"}, {6, "y"}, {9, "z"}};
    writer.setIfPatternStore(ifPatternEntries);

    std::vector<std::string> allIfStmtsWithVariables{"5", "5", "6", "6", "6", "9"};
    std::vector<std::string> allVarsInIf{"x", "y", "z", "w", "y", "z"};

    std::shared_ptr<Synonym> ifSyn = std::make_shared<Synonym>(DesignEntityType::IF, "ii");
    std::shared_ptr<Synonym> varSyn = std::make_shared<Synonym>(DesignEntityType::VARIABLE, "v");
    std::shared_ptr<Literal> literalValue = std::make_shared<Literal>("y");
    std::shared_ptr<Wildcard> wildcard = std::make_shared<Wildcard>();

    SECTION("If(Variable, _)") {
        PatternClause ifPattern = {ifSyn, {varSyn, wildcard, wildcard}};
        ClauseResult clauseResult = ifPattern.runEvaluation(reader);
        std::vector<Synonym> synResults = clauseResult.getSynonyms();
        std::vector<SynonymValues> valueResults = clauseResult.getAllSynonymValues();
        REQUIRE(synResults == std::vector<Synonym>{*ifSyn, *varSyn});
        REQUIRE_EQUAL_VECTORS(valueResults[0], allIfStmtsWithVariables);
        REQUIRE_EQUAL_VECTORS(valueResults[1], allVarsInIf);
    }

    SECTION("If(\"y\", _)") {
        PatternClause ifPattern = {ifSyn, {literalValue, wildcard, wildcard}};
        ClauseResult clauseResult = ifPattern.runEvaluation(reader);
        std::vector<Synonym> synResults = clauseResult.getSynonyms();
        std::vector<SynonymValues> valueResults = clauseResult.getAllSynonymValues();
        REQUIRE(synResults == std::vector<Synonym>{*ifSyn});
        REQUIRE_EQUAL_VECTORS(valueResults[0], std::vector<std::string>({"5", "6"}));
    }

    SECTION("If(Literal not in store, _)") {
        std::shared_ptr<Literal> notInStoreLiteral = std::make_shared<Literal>("l");
        PatternClause ifPattern = {ifSyn, {notInStoreLiteral, wildcard, wildcard}};
        ClauseResult clauseResult = ifPattern.runEvaluation(reader);
        std::vector<Synonym> synResults = clauseResult.getSynonyms();
        std::vector<SynonymValues> valueResults = clauseResult.getAllSynonymValues();
        REQUIRE(synResults == std::vector<Synonym>{*ifSyn});
        REQUIRE_EQUAL_VECTORS(valueResults[0], {});
    }

    SECTION("If(Literal in store not in condition, _)") {
        std::shared_ptr<Literal> notInStoreLiteral = std::make_shared<Literal>("abc");
        PatternClause ifPattern = {ifSyn, {notInStoreLiteral, wildcard, wildcard}};
        ClauseResult clauseResult = ifPattern.runEvaluation(reader);
        std::vector<Synonym> synResults = clauseResult.getSynonyms();
        std::vector<SynonymValues> valueResults = clauseResult.getAllSynonymValues();
        REQUIRE(synResults == std::vector<Synonym>{*ifSyn});
        REQUIRE_EQUAL_VECTORS(valueResults[0], {});
    }

    SECTION("If(_, _)") {
        PatternClause ifPattern = {ifSyn, {wildcard, wildcard, wildcard}};
        ClauseResult clauseResult = ifPattern.runEvaluation(reader);
        std::vector<Synonym> synResults = clauseResult.getSynonyms();
        std::vector<SynonymValues> valueResults = clauseResult.getAllSynonymValues();
        REQUIRE(synResults == std::vector<Synonym>{*ifSyn});
        REQUIRE_EQUAL_VECTORS(valueResults[0], {allIfStmtsWithVariables});
    }
}
