#include "TesterHelper.h"
#include "catch.hpp"
#include "pkb/PKBClient/PKBFacadeWriter.h"
#include "qps/clauseArguments/Wildcard.h"
#include "qps/clauses/SuchThatClause.h"

TEST_CASE("SuchThatClause evaluate for Next relationship with no synonyms") {
    PKB pkb{};
    PKBFacadeWriter pfw{pkb};
    PKBFacadeReader pfr{pkb};

    SECTION("Next(Integer, Integer)") {
        std::unordered_set<std::pair<int, int>> nextStoreEntries{std::pair<int, int>{1, 2}};
        pfw.setNextStore(nextStoreEntries);
        NextTester{pfr, std::make_shared<Integer>("1"), std::make_shared<Integer>("2")}.testBoolean(true);
        NextTester{pfr, std::make_shared<Integer>("2"), std::make_shared<Integer>("1")}.testBoolean(false);
        NextTester{pfr, std::make_shared<Integer>("1"), std::make_shared<Integer>("1")}.testBoolean(false);
        NextTester{pfr, std::make_shared<Integer>("2"), std::make_shared<Integer>("2")}.testBoolean(false);
    }

    SECTION("Next(Integer, Integer) / empty store") {
        pfw.setNextStore({});
        NextTester{pfr, std::make_shared<Integer>("1"), std::make_shared<Integer>("2")}.testBoolean(false);
    }

    SECTION("Next(Integer, Wildcard)") {
        std::unordered_set<std::pair<int, int>> nextStoreEntries{std::pair<int, int>{1, 2}};
        pfw.setNextStore(nextStoreEntries);
        NextTester{pfr, std::make_shared<Integer>("1"), std::make_shared<Wildcard>()}.testBoolean(true);
        NextTester{pfr, std::make_shared<Integer>("2"), std::make_shared<Wildcard>()}.testBoolean(false);
    }

    SECTION("Next(Integer, Wildcard) / empty store") {
        pfw.setNextStore({});
        NextTester{pfr, std::make_shared<Integer>("1"), std::make_shared<Wildcard>()}.testBoolean(false);
        NextTester{pfr, std::make_shared<Wildcard>(), std::make_shared<Wildcard>()}.testBoolean(false);
    }

    SECTION("Next(Wildcard, Integer)") {
        std::unordered_set<std::pair<int, int>> nextStoreEntries{std::pair<int, int>{1, 2}};
        pfw.setNextStore(nextStoreEntries);
        NextTester{pfr, std::make_shared<Wildcard>(), std::make_shared<Integer>("2")}.testBoolean(true);
        NextTester{pfr, std::make_shared<Wildcard>(), std::make_shared<Integer>("1")}.testBoolean(false);
    }
}

TEST_CASE("SuchThatClause evaluate for Next relationship with 1 synonym") {
    PKB pkb{};
    PKBFacadeWriter pfw{pkb};
    PKBFacadeReader pfr{pkb};

    SECTION("Next(Synonym, Integer)") {
        std::unordered_set<std::pair<int, int>> nextStoreEntries{std::pair<int, int>{1, 2}};
        pfw.setNextStore(nextStoreEntries);

        std::unordered_set<Stmt> stmts = {Stmt{StatementType::ASSIGN, 1}, Stmt{StatementType::ASSIGN, 2},
                                          Stmt{StatementType::ASSIGN, 3}, Stmt{StatementType::ASSIGN, 4}};
        pfw.setStmts(stmts);

        std::shared_ptr<Synonym> stmtSyn = std::make_shared<Synonym>(DesignEntityType::STMT, "s");
        // Next(s, 2) --> 1
        NextTester{pfr, stmtSyn, std::make_shared<Integer>("2")}.testSynonyms({*stmtSyn}).testSynonymValues({{"1"}});
        // Next(s, 1) --> none
        NextTester{pfr, stmtSyn, std::make_shared<Integer>("1")}.testSynonyms({*stmtSyn}).testSynonymValues({});
    }

    SECTION("Next(Integer, Synonym)") {
        std::unordered_set<std::pair<int, int>> nextStoreEntries{std::pair<int, int>{1, 2}};
        pfw.setNextStore(nextStoreEntries);

        std::unordered_set<Stmt> stmts = {Stmt{StatementType::ASSIGN, 1}, Stmt{StatementType::ASSIGN, 2},
                                          Stmt{StatementType::ASSIGN, 3}, Stmt{StatementType::ASSIGN, 4}};
        pfw.setStmts(stmts);

        std::shared_ptr<Synonym> stmtSyn = std::make_shared<Synonym>(DesignEntityType::STMT, "s");
        NextTester{pfr, std::make_shared<Integer>("1"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{"2"}});
        NextTester{pfr, std::make_shared<Integer>("2"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({});
    }

    SECTION("Next(Synonym, Wildcard)") {
        std::unordered_set<std::pair<int, int>> nextStoreEntries{std::pair<int, int>{1, 2}, {3, 4}};
        pfw.setNextStore(nextStoreEntries);

        std::unordered_set<Stmt> stmts = {Stmt{StatementType::ASSIGN, 1}, Stmt{StatementType::ASSIGN, 2},
                                          Stmt{StatementType::ASSIGN, 3}, Stmt{StatementType::ASSIGN, 4}};
        pfw.setStmts(stmts);

        std::shared_ptr<Synonym> stmtSyn = std::make_shared<Synonym>(DesignEntityType::STMT, "s");
        NextTester{pfr, stmtSyn, std::make_shared<Wildcard>()}.testSynonyms({*stmtSyn}).testSynonymValues({{"1", "3"}});
    }

    SECTION("Next(Wildcard, Synonym)") {
        std::unordered_set<std::pair<int, int>> nextStoreEntries{std::pair<int, int>{1, 2}, {3, 4}};
        pfw.setNextStore(nextStoreEntries);

        std::unordered_set<Stmt> stmts = {Stmt{StatementType::ASSIGN, 1}, Stmt{StatementType::ASSIGN, 2},
                                          Stmt{StatementType::ASSIGN, 3}, Stmt{StatementType::ASSIGN, 4}};
        pfw.setStmts(stmts);

        std::shared_ptr<Synonym> stmtSyn = std::make_shared<Synonym>(DesignEntityType::STMT, "s");
        NextTester{pfr, std::make_shared<Wildcard>(), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{"2", "4"}});
    }
}

TEST_CASE("SuchThatClause evaluate for Next relationship with 2 synonyms") {
    PKB pkb{};
    PKBFacadeWriter pfw{pkb};
    PKBFacadeReader pfr{pkb};

    SECTION("Next(Synonym, Synonym)") {
        std::unordered_set<std::pair<int, int>> nextStoreEntries{std::pair<int, int>{1, 2}, {3, 4}};
        pfw.setNextStore(nextStoreEntries);

        std::unordered_set<Stmt> stmts = {Stmt{StatementType::ASSIGN, 1}, Stmt{StatementType::ASSIGN, 2},
                                          Stmt{StatementType::ASSIGN, 3}, Stmt{StatementType::ASSIGN, 4}};
        pfw.setStmts(stmts);

        std::shared_ptr<Synonym> stmtSyn1 = std::make_shared<Synonym>(DesignEntityType::STMT, "s1");
        std::shared_ptr<Synonym> stmtSyn2 = std::make_shared<Synonym>(DesignEntityType::STMT, "s2");
        NextTester{pfr, stmtSyn1, stmtSyn2}
            .testSynonyms({*stmtSyn1, *stmtSyn2})
            .testSynonymValues({{"1", "3"}, {"2", "4"}});
    }
}
