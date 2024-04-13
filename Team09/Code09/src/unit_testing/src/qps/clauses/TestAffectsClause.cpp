#include "TesterHelper.h"
#include "catch.hpp"
#include "pkb/PKBClient/PKBFacadeWriter.h"
#include "qps/clauseArguments/Wildcard.h"
#include "qps/clauses/SuchThatClause.h"

TEST_CASE("SuchThatClause evaluate for affects relationship, no synonym") {
    PKB pkb{};
    PKBFacadeWriter pfw{pkb};
    PKBFacadeReader pfr{pkb};

    SECTION("Affects(integer, integer)") {
        std::unordered_set<Variable> variables = {"x", "y"};

        std::unordered_set<Stmt> stmts = {Stmt{StatementType::ASSIGN, 1}, Stmt{StatementType::PRINT, 2},
                                          Stmt{StatementType::ASSIGN, 3}, Stmt{StatementType::ASSIGN, 4}};

        /**
         * x = 1 -- assign/modifies x
         * print x -- print/uses x
         * x = x + 1 -- assign/modifies x/ uses x
         * y = x + 1 -- assign/uses x
         */

        std::unordered_set<std::pair<int, int>> nextStoreEntries{std::pair<int, int>{1, 2}, std::pair<int, int>{2, 3},
                                                                 std::pair<int, int>{3, 4}};

        std::unordered_set<std::pair<int, std::string>> usesStoreEntries{std::pair<int, std::string>{2, "x"},
                                                                         std::pair<int, std::string>{3, "x"},
                                                                         std::pair<int, std::string>{4, "x"}};

        std::unordered_set<std::pair<int, std::string>> modifiesStoreEntries{std::pair<int, std::string>{1, "x"},
                                                                             std::pair<int, std::string>{3, "x"},
                                                                             std::pair<int, std::string>{4, "y"}};

        pfw.setVariables(variables);
        pfw.setStmts(stmts);
        pfw.setNextStore(nextStoreEntries);
        pfw.setStatementUsesStore(usesStoreEntries);
        pfw.setStatementModifiesStore(modifiesStoreEntries);

        AffectsTester{pfr, std::make_shared<Integer>("1"), std::make_shared<Integer>("2")}.testBoolean(false);
        AffectsTester{pfr, std::make_shared<Integer>("1"), std::make_shared<Integer>("3")}.testBoolean(true);
        AffectsTester{pfr, std::make_shared<Integer>("1"), std::make_shared<Integer>("4")}.testBoolean(false);
        AffectsTester{pfr, std::make_shared<Integer>("2"), std::make_shared<Integer>("3")}.testBoolean(false);
        AffectsTester{pfr, std::make_shared<Integer>("2"), std::make_shared<Integer>("4")}.testBoolean(false);
        AffectsTester{pfr, std::make_shared<Integer>("3"), std::make_shared<Integer>("4")}.testBoolean(true);

        AffectsTester{pfr, std::make_shared<Integer>("4"), std::make_shared<Integer>("3")}.testBoolean(false);
        AffectsTester{pfr, std::make_shared<Integer>("4"), std::make_shared<Integer>("2")}.testBoolean(false);
        AffectsTester{pfr, std::make_shared<Integer>("4"), std::make_shared<Integer>("1")}.testBoolean(false);
        AffectsTester{pfr, std::make_shared<Integer>("3"), std::make_shared<Integer>("2")}.testBoolean(false);
        AffectsTester{pfr, std::make_shared<Integer>("3"), std::make_shared<Integer>("1")}.testBoolean(false);
        AffectsTester{pfr, std::make_shared<Integer>("2"), std::make_shared<Integer>("1")}.testBoolean(false);

        AffectsTester{pfr, std::make_shared<Integer>("1"), std::make_shared<Integer>("1")}.testBoolean(false);
        AffectsTester{pfr, std::make_shared<Integer>("2"), std::make_shared<Integer>("2")}.testBoolean(false);
        AffectsTester{pfr, std::make_shared<Integer>("3"), std::make_shared<Integer>("3")}.testBoolean(false);
        AffectsTester{pfr, std::make_shared<Integer>("4"), std::make_shared<Integer>("4")}.testBoolean(false);
    }

    SECTION("Affects(integer, integer) / empty store") {
        std::unordered_set<Stmt> stmts = {Stmt{StatementType::ASSIGN, 1}, Stmt{StatementType::PRINT, 2},
                                          Stmt{StatementType::ASSIGN, 3}, Stmt{StatementType::ASSIGN, 4}};
        pfw.setVariables({});
        pfw.setStmts(stmts);
        pfw.setNextStore({});
        pfw.setStatementUsesStore({});
        pfw.setStatementModifiesStore({});

        AffectsTester{pfr, std::make_shared<Integer>("1"), std::make_shared<Integer>("2")}.testBoolean(false);
        AffectsTester{pfr, std::make_shared<Integer>("1"), std::make_shared<Integer>("3")}.testBoolean(false);
        AffectsTester{pfr, std::make_shared<Integer>("1"), std::make_shared<Integer>("4")}.testBoolean(false);
        AffectsTester{pfr, std::make_shared<Integer>("2"), std::make_shared<Integer>("3")}.testBoolean(false);
        AffectsTester{pfr, std::make_shared<Integer>("2"), std::make_shared<Integer>("4")}.testBoolean(false);
        AffectsTester{pfr, std::make_shared<Integer>("3"), std::make_shared<Integer>("4")}.testBoolean(false);

        AffectsTester{pfr, std::make_shared<Integer>("4"), std::make_shared<Integer>("1")}.testBoolean(false);
        AffectsTester{pfr, std::make_shared<Integer>("4"), std::make_shared<Integer>("2")}.testBoolean(false);
        AffectsTester{pfr, std::make_shared<Integer>("4"), std::make_shared<Integer>("3")}.testBoolean(false);
        AffectsTester{pfr, std::make_shared<Integer>("3"), std::make_shared<Integer>("2")}.testBoolean(false);
        AffectsTester{pfr, std::make_shared<Integer>("3"), std::make_shared<Integer>("1")}.testBoolean(false);
        AffectsTester{pfr, std::make_shared<Integer>("2"), std::make_shared<Integer>("1")}.testBoolean(false);

        AffectsTester{pfr, std::make_shared<Integer>("1"), std::make_shared<Integer>("1")}.testBoolean(false);
        AffectsTester{pfr, std::make_shared<Integer>("2"), std::make_shared<Integer>("2")}.testBoolean(false);
        AffectsTester{pfr, std::make_shared<Integer>("3"), std::make_shared<Integer>("3")}.testBoolean(false);
        AffectsTester{pfr, std::make_shared<Integer>("4"), std::make_shared<Integer>("4")}.testBoolean(false);
    }

    SECTION("Affects(wildcard, wildcard)") {
        std::unordered_set<Variable> variables = {"x", "y"};

        std::unordered_set<Stmt> stmts = {Stmt{StatementType::ASSIGN, 1}, Stmt{StatementType::PRINT, 2},
                                          Stmt{StatementType::ASSIGN, 3}, Stmt{StatementType::ASSIGN, 4}};

        /**
         * x = 1 -- assign/modifies x
         * print x -- print/uses x
         * x = x + 1 -- assign/modifies x/ uses x
         * y = x + 1 -- assign/uses x
         */

        std::unordered_set<std::pair<int, int>> nextStoreEntries{std::pair<int, int>{1, 2}, std::pair<int, int>{2, 3},
                                                                 std::pair<int, int>{3, 4}};

        std::unordered_set<std::pair<int, std::string>> usesStoreEntries{std::pair<int, std::string>{2, "x"},
                                                                         std::pair<int, std::string>{3, "x"},
                                                                         std::pair<int, std::string>{4, "x"}};

        std::unordered_set<std::pair<int, std::string>> modifiesStoreEntries{std::pair<int, std::string>{1, "x"},
                                                                             std::pair<int, std::string>{3, "x"},
                                                                             std::pair<int, std::string>{4, "y"}};

        pfw.setVariables(variables);
        pfw.setStmts(stmts);
        pfw.setNextStore(nextStoreEntries);
        pfw.setStatementUsesStore(usesStoreEntries);
        pfw.setStatementModifiesStore(modifiesStoreEntries);

        AffectsTester{pfr, std::make_shared<Wildcard>(), std::make_shared<Wildcard>()}.testBoolean(true);
    }

    SECTION("Affects(wildcard, wildcard) / empty store") {
        std::unordered_set<Stmt> stmts = {Stmt{StatementType::ASSIGN, 1}, Stmt{StatementType::PRINT, 2},
                                          Stmt{StatementType::ASSIGN, 3}, Stmt{StatementType::ASSIGN, 4}};
        pfw.setVariables({});
        pfw.setStmts(stmts);
        pfw.setNextStore({});
        pfw.setStatementUsesStore({});
        pfw.setStatementModifiesStore({});

        AffectsTester{pfr, std::make_shared<Wildcard>(), std::make_shared<Wildcard>()}.testBoolean(false);
    }

    SECTION("Affects(integer, wildcard)") {
        std::unordered_set<Variable> variables = {"x", "y"};

        std::unordered_set<Stmt> stmts = {Stmt{StatementType::ASSIGN, 1}, Stmt{StatementType::PRINT, 2},
                                          Stmt{StatementType::ASSIGN, 3}, Stmt{StatementType::ASSIGN, 4}};

        /**
         * x = 1 -- assign/modifies x
         * print x -- print/uses x
         * x = x + 1 -- assign/modifies x/ uses x
         * y = x + 1 -- assign/uses x
         */

        std::unordered_set<std::pair<int, int>> nextStoreEntries{std::pair<int, int>{1, 2}, std::pair<int, int>{2, 3},
                                                                 std::pair<int, int>{3, 4}};

        std::unordered_set<std::pair<int, std::string>> usesStoreEntries{std::pair<int, std::string>{2, "x"},
                                                                         std::pair<int, std::string>{3, "x"},
                                                                         std::pair<int, std::string>{4, "x"}};

        std::unordered_set<std::pair<int, std::string>> modifiesStoreEntries{std::pair<int, std::string>{1, "x"},
                                                                             std::pair<int, std::string>{3, "x"},
                                                                             std::pair<int, std::string>{4, "y"}};

        pfw.setVariables(variables);
        pfw.setStmts(stmts);
        pfw.setNextStore(nextStoreEntries);
        pfw.setStatementUsesStore(usesStoreEntries);
        pfw.setStatementModifiesStore(modifiesStoreEntries);

        AffectsTester{pfr, std::make_shared<Integer>("1"), std::make_shared<Wildcard>()}.testBoolean(true);
        AffectsTester{pfr, std::make_shared<Integer>("2"), std::make_shared<Wildcard>()}.testBoolean(false);
        AffectsTester{pfr, std::make_shared<Integer>("3"), std::make_shared<Wildcard>()}.testBoolean(true);
        AffectsTester{pfr, std::make_shared<Integer>("4"), std::make_shared<Wildcard>()}.testBoolean(false);

        AffectsTester{pfr, std::make_shared<Wildcard>(), std::make_shared<Integer>("4")}.testBoolean(true);
        AffectsTester{pfr, std::make_shared<Wildcard>(), std::make_shared<Integer>("3")}.testBoolean(true);
        AffectsTester{pfr, std::make_shared<Wildcard>(), std::make_shared<Integer>("2")}.testBoolean(false);
        AffectsTester{pfr, std::make_shared<Wildcard>(), std::make_shared<Integer>("1")}.testBoolean(false);
    }

    SECTION("Affects(integer, wildcard) / empty store") {
        std::unordered_set<Stmt> stmts = {Stmt{StatementType::ASSIGN, 1}, Stmt{StatementType::PRINT, 2},
                                          Stmt{StatementType::ASSIGN, 3}, Stmt{StatementType::ASSIGN, 4}};

        pfw.setVariables({});
        pfw.setStmts(stmts);
        pfw.setNextStore({});
        pfw.setStatementUsesStore({});
        pfw.setStatementModifiesStore({});

        AffectsTester{pfr, std::make_shared<Integer>("1"), std::make_shared<Wildcard>()}.testBoolean(false);
        AffectsTester{pfr, std::make_shared<Integer>("2"), std::make_shared<Wildcard>()}.testBoolean(false);
        AffectsTester{pfr, std::make_shared<Integer>("3"), std::make_shared<Wildcard>()}.testBoolean(false);
        AffectsTester{pfr, std::make_shared<Integer>("4"), std::make_shared<Wildcard>()}.testBoolean(false);

        AffectsTester{pfr, std::make_shared<Wildcard>(), std::make_shared<Integer>("4")}.testBoolean(false);
        AffectsTester{pfr, std::make_shared<Wildcard>(), std::make_shared<Integer>("3")}.testBoolean(false);
        AffectsTester{pfr, std::make_shared<Wildcard>(), std::make_shared<Integer>("2")}.testBoolean(false);
        AffectsTester{pfr, std::make_shared<Wildcard>(), std::make_shared<Integer>("1")}.testBoolean(false);
    }
}

TEST_CASE("SuchThatClause evaluate for affects relationship, one synonym") {
    PKB pkb{};
    PKBFacadeWriter pfw{pkb};
    PKBFacadeReader pfr{pkb};

    SECTION("Affects(synonym, integer)") {
        std::unordered_set<Variable> variables = {"x", "y"};

        std::unordered_set<Stmt> stmts = {Stmt{StatementType::ASSIGN, 1}, Stmt{StatementType::PRINT, 2},
                                          Stmt{StatementType::ASSIGN, 3}, Stmt{StatementType::ASSIGN, 4}};

        /**
         * x = 1 -- assign/modifies x
         * print x -- print/uses x
         * x = x + 1 -- assign/modifies x/ uses x
         * y = x + 1 -- assign/uses x
         */

        std::unordered_set<std::pair<int, int>> nextStoreEntries{std::pair<int, int>{1, 2}, std::pair<int, int>{2, 3},
                                                                 std::pair<int, int>{3, 4}};

        std::unordered_set<std::pair<int, std::string>> usesStoreEntries{std::pair<int, std::string>{2, "x"},
                                                                         std::pair<int, std::string>{3, "x"},
                                                                         std::pair<int, std::string>{4, "x"}};

        std::unordered_set<std::pair<int, std::string>> modifiesStoreEntries{std::pair<int, std::string>{1, "x"},
                                                                             std::pair<int, std::string>{3, "x"},
                                                                             std::pair<int, std::string>{4, "y"}};

        pfw.setVariables(variables);
        pfw.setStmts(stmts);
        pfw.setNextStore(nextStoreEntries);
        pfw.setStatementUsesStore(usesStoreEntries);
        pfw.setStatementModifiesStore(modifiesStoreEntries);

        std::shared_ptr<Synonym> stmtSyn = std::make_shared<Synonym>(DesignEntityType::ASSIGN, "s");

        // Select s such that Affects(s, 4)
        AffectsTester{pfr, stmtSyn, std::make_shared<Integer>("4")}.testSynonyms({*stmtSyn}).testSynonymValues({{"3"}});
        // Select s such that Affects(s, 3)
        AffectsTester{pfr, stmtSyn, std::make_shared<Integer>("3")}.testSynonyms({*stmtSyn}).testSynonymValues({{"1"}});
        // Select s such that Affects(s, 2)
        AffectsTester{pfr, stmtSyn, std::make_shared<Integer>("2")}.testSynonyms({*stmtSyn}).testSynonymValues({{}});
        // Select s such that Affects(s, 1)
        AffectsTester{pfr, stmtSyn, std::make_shared<Integer>("1")}.testSynonyms({*stmtSyn}).testSynonymValues({{}});

        // Select s such that Affects(s, 5), out of bounds
        AffectsTester{pfr, stmtSyn, std::make_shared<Integer>("5")}.testSynonyms({*stmtSyn}).testSynonymValues({{}});
    }

    SECTION("Affects(integer, synonym)") {
        std::unordered_set<Variable> variables = {"x", "y"};

        std::unordered_set<Stmt> stmts = {Stmt{StatementType::ASSIGN, 1}, Stmt{StatementType::PRINT, 2},
                                          Stmt{StatementType::ASSIGN, 3}, Stmt{StatementType::ASSIGN, 4}};

        /**
         * x = 1 -- assign/modifies x
         * print x -- print/uses x
         * x = x + 1 -- assign/modifies x/ uses x
         * y = x + 1 -- assign/uses x
         */

        std::unordered_set<std::pair<int, int>> nextStoreEntries{std::pair<int, int>{1, 2}, std::pair<int, int>{2, 3},
                                                                 std::pair<int, int>{3, 4}};

        std::unordered_set<std::pair<int, std::string>> usesStoreEntries{std::pair<int, std::string>{2, "x"},
                                                                         std::pair<int, std::string>{3, "x"},
                                                                         std::pair<int, std::string>{4, "x"}};

        std::unordered_set<std::pair<int, std::string>> modifiesStoreEntries{std::pair<int, std::string>{1, "x"},
                                                                             std::pair<int, std::string>{3, "x"},
                                                                             std::pair<int, std::string>{4, "y"}};

        pfw.setVariables(variables);
        pfw.setStmts(stmts);
        pfw.setNextStore(nextStoreEntries);
        pfw.setStatementUsesStore(usesStoreEntries);
        pfw.setStatementModifiesStore(modifiesStoreEntries);

        std::shared_ptr<Synonym> stmtSyn = std::make_shared<Synonym>(DesignEntityType::ASSIGN, "s");

        // Select s such that Affects(4, s)
        AffectsTester{pfr, std::make_shared<Integer>("4"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{}});
        // Select s such that Affects(3, s)
        AffectsTester{pfr, std::make_shared<Integer>("3"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{"4"}});
        // Select s such that Affects(2, s)
        AffectsTester{pfr, std::make_shared<Integer>("2"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{}});
        // Select s such that Affects(1, s)
        AffectsTester{pfr, std::make_shared<Integer>("1"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{"3"}});
        // Select s such that Affects(s, 5), out of bounds
        AffectsTester{pfr, std::make_shared<Integer>("5"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{}});
    }

    SECTION("Affects(synonym, wildcard)") {
        std::unordered_set<Variable> variables = {"x", "y"};

        std::unordered_set<Stmt> stmts = {Stmt{StatementType::ASSIGN, 1}, Stmt{StatementType::PRINT, 2},
                                          Stmt{StatementType::ASSIGN, 3}, Stmt{StatementType::ASSIGN, 4}};

        /**
         * x = 1 -- assign/modifies x
         * print x -- print/uses x
         * x = x + 1 -- assign/modifies x/ uses x
         * y = x + 1 -- assign/uses x
         */

        std::unordered_set<std::pair<int, int>> nextStoreEntries{std::pair<int, int>{1, 2}, std::pair<int, int>{2, 3},
                                                                 std::pair<int, int>{3, 4}};

        std::unordered_set<std::pair<int, std::string>> usesStoreEntries{std::pair<int, std::string>{2, "x"},
                                                                         std::pair<int, std::string>{3, "x"},
                                                                         std::pair<int, std::string>{4, "x"}};

        std::unordered_set<std::pair<int, std::string>> modifiesStoreEntries{std::pair<int, std::string>{1, "x"},
                                                                             std::pair<int, std::string>{3, "x"},
                                                                             std::pair<int, std::string>{4, "y"}};

        pfw.setVariables(variables);
        pfw.setStmts(stmts);
        pfw.setNextStore(nextStoreEntries);
        pfw.setStatementUsesStore(usesStoreEntries);
        pfw.setStatementModifiesStore(modifiesStoreEntries);

        std::shared_ptr<Synonym> stmtSyn = std::make_shared<Synonym>(DesignEntityType::ASSIGN, "s");

        AffectsTester{pfr, stmtSyn, std::make_shared<Wildcard>()}
            .testSynonyms({*stmtSyn})
            .testSynonymValues({{"1", "3"}});
    }

    SECTION("Affects(wildcard, synonym)") {
        std::unordered_set<Variable> variables = {"x", "y"};

        std::unordered_set<Stmt> stmts = {Stmt{StatementType::ASSIGN, 1}, Stmt{StatementType::PRINT, 2},
                                          Stmt{StatementType::ASSIGN, 3}, Stmt{StatementType::ASSIGN, 4}};

        /**
         * x = 1 -- assign/modifies x
         * print x -- print/uses x
         * x = x + 1 -- assign/modifies x/ uses x
         * y = x + 1 -- assign/uses x
         */

        std::unordered_set<std::pair<int, int>> nextStoreEntries{std::pair<int, int>{1, 2}, std::pair<int, int>{2, 3},
                                                                 std::pair<int, int>{3, 4}};

        std::unordered_set<std::pair<int, std::string>> usesStoreEntries{std::pair<int, std::string>{2, "x"},
                                                                         std::pair<int, std::string>{3, "x"},
                                                                         std::pair<int, std::string>{4, "x"}};

        std::unordered_set<std::pair<int, std::string>> modifiesStoreEntries{std::pair<int, std::string>{1, "x"},
                                                                             std::pair<int, std::string>{3, "x"},
                                                                             std::pair<int, std::string>{4, "y"}};

        pfw.setVariables(variables);
        pfw.setStmts(stmts);
        pfw.setNextStore(nextStoreEntries);
        pfw.setStatementUsesStore(usesStoreEntries);
        pfw.setStatementModifiesStore(modifiesStoreEntries);

        std::shared_ptr<Synonym> stmtSyn = std::make_shared<Synonym>(DesignEntityType::ASSIGN, "s");

        AffectsTester{pfr, std::make_shared<Wildcard>(), stmtSyn}
            .testSynonyms({*stmtSyn})
            .testSynonymValues({{"3", "4"}});
    }
}

TEST_CASE("SuchThatClause evaluate for affects relationship with both synonym") {
    PKB pkb{};
    PKBFacadeWriter pfw{pkb};
    PKBFacadeReader pfr{pkb};

    SECTION("Affects(synonym, synonym)") {
        std::unordered_set<Variable> variables = {"x", "y"};

        std::unordered_set<Stmt> stmts = {Stmt{StatementType::ASSIGN, 1}, Stmt{StatementType::PRINT, 2},
                                          Stmt{StatementType::ASSIGN, 3}, Stmt{StatementType::ASSIGN, 4}};

        /**
         * x = 1 -- assign/modifies x
         * print x -- print/uses x
         * x = x + 1 -- assign/modifies x/ uses x
         * y = x + 1 -- assign/uses x
         */

        std::unordered_set<std::pair<int, int>> nextStoreEntries{std::pair<int, int>{1, 2}, std::pair<int, int>{2, 3},
                                                                 std::pair<int, int>{3, 4}};

        std::unordered_set<std::pair<int, std::string>> usesStoreEntries{std::pair<int, std::string>{2, "x"},
                                                                         std::pair<int, std::string>{3, "x"},
                                                                         std::pair<int, std::string>{4, "x"}};

        std::unordered_set<std::pair<int, std::string>> modifiesStoreEntries{std::pair<int, std::string>{1, "x"},
                                                                             std::pair<int, std::string>{3, "x"},
                                                                             std::pair<int, std::string>{4, "y"}};

        pfw.setVariables(variables);
        pfw.setStmts(stmts);
        pfw.setNextStore(nextStoreEntries);
        pfw.setStatementUsesStore(usesStoreEntries);
        pfw.setStatementModifiesStore(modifiesStoreEntries);

        std::shared_ptr<Synonym> stmtSyn1 = std::make_shared<Synonym>(DesignEntityType::ASSIGN, "s1");
        std::shared_ptr<Synonym> stmtSyn2 = std::make_shared<Synonym>(DesignEntityType::ASSIGN, "s2");

        AffectsTester{pfr, stmtSyn1, stmtSyn2}
            .testSynonyms({*stmtSyn1, *stmtSyn2})
            .testSynonymValues({{"1", "3"}, {"3", "4"}});
    }

    SECTION("Affects(synonym, synonym) / empty store") {
        std::unordered_set<Stmt> stmts = {Stmt{StatementType::ASSIGN, 1}, Stmt{StatementType::PRINT, 2},
                                          Stmt{StatementType::ASSIGN, 3}, Stmt{StatementType::ASSIGN, 4}};

        pfw.setVariables({});
        pfw.setStmts(stmts);
        pfw.setNextStore({});
        pfw.setStatementUsesStore({});
        pfw.setStatementModifiesStore({});

        std::shared_ptr<Synonym> stmtSyn1 = std::make_shared<Synonym>(DesignEntityType::ASSIGN, "s1");
        std::shared_ptr<Synonym> stmtSyn2 = std::make_shared<Synonym>(DesignEntityType::ASSIGN, "s2");

        AffectsTester{pfr, stmtSyn1, stmtSyn2}.testSynonyms({*stmtSyn1, *stmtSyn2}).testSynonymValues({{}, {}});
    }
}
