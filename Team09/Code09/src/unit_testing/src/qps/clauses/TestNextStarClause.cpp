#include "PKB/PKBClient/PKBFacadeWriter.h"
#include "TesterHelper.h"
#include "catch.hpp"
#include "qps/clauseArguments/Wildcard.h"
#include "qps/clauses/SuchThatClause.h"

TEST_CASE("SuchThatClause evaluate for Next relationship with no synonyms") {
    PKB pkb{};
    PKBFacadeWriter pfw{pkb};
    PKBFacadeReader pfr{pkb};

    SECTION("Next(Integer, Integer)") {
        std::unordered_set<std::pair<int, int>> nextStoreEntries{std::pair<int, int>{1, 2}, std::pair<int, int>{2, 3}};
        pfw.setNextStore(nextStoreEntries);
        NextStarTester{pfr, new Integer("1"), new Integer("2")}.testBoolean(true);
        NextStarTester{pfr, new Integer("1"), new Integer("3")}.testBoolean(true);
        NextStarTester{pfr, new Integer("2"), new Integer("1")}.testBoolean(false);
        NextStarTester{pfr, new Integer("1"), new Integer("1")}.testBoolean(false);
        NextStarTester{pfr, new Integer("2"), new Integer("2")}.testBoolean(false);
    }

    SECTION("Next(Integer, Integer) / empty store") {
        pfw.setNextStore({});
        NextStarTester{pfr, new Integer("1"), new Integer("2")}.testBoolean(false);
    }

    SECTION("Next(Integer, Wildcard)") {
        std::unordered_set<std::pair<int, int>> nextStoreEntries{std::pair<int, int>{1, 2}, std::pair<int, int>{2, 3}};
        pfw.setNextStore(nextStoreEntries);
        NextStarTester{pfr, new Integer("1"), new Wildcard()}.testBoolean(true);
        NextStarTester{pfr, new Integer("2"), new Wildcard()}.testBoolean(true);
        NextStarTester{pfr, new Integer("3"), new Wildcard()}.testBoolean(false);
    }

    SECTION("Next(Integer, Wildcard) / empty store") {
        pfw.setNextStore({});
        NextStarTester{pfr, new Integer("1"), new Wildcard()}.testBoolean(false);
        NextStarTester{pfr, new Wildcard(), new Wildcard()}.testBoolean(false);
    }

    SECTION("Next(Wildcard, Integer)") {
        std::unordered_set<std::pair<int, int>> nextStoreEntries{std::pair<int, int>{1, 2}};
        pfw.setNextStore(nextStoreEntries);
        NextStarTester{pfr, new Wildcard(), new Integer("2")}.testBoolean(true);
        NextStarTester{pfr, new Wildcard(), new Integer("1")}.testBoolean(false);
    }
}

TEST_CASE("SuchThatClause evaluate for Next relationship with 1 synonym") {
    PKB pkb{};
    PKBFacadeWriter pfw{pkb};
    PKBFacadeReader pfr{pkb};

    SECTION("Next(Synonym, Integer)") {
        std::unordered_set<std::pair<int, int>> nextStoreEntries{std::pair<int, int>{1, 2}};
        pfw.setNextStore(nextStoreEntries);

        Synonym* stmtSyn = new Synonym(DesignEntityType::STMT, "s");
        NextStarTester{pfr, stmtSyn, new Integer("2")}.testSynonyms({*stmtSyn}).testSynonymValues({{"1"}});
        NextStarTester{pfr, stmtSyn, new Integer("1")}.testSynonyms({*stmtSyn}).testSynonymValues({{}});
    }

    SECTION("Next(Integer, Synonym)") {
        std::unordered_set<std::pair<int, int>> nextStoreEntries{std::pair<int, int>{1, 2}};
        pfw.setNextStore(nextStoreEntries);

        Synonym* stmtSyn = new Synonym(DesignEntityType::STMT, "s");
        NextStarTester{pfr, new Integer("1"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{"2"}});
        NextStarTester{pfr, new Integer("2"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{}});
    }

    SECTION("Next(Synonym, Wildcard)") {
        std::unordered_set<std::pair<int, int>> nextStoreEntries{std::pair<int, int>{1, 2}, {3, 4}};
        pfw.setNextStore(nextStoreEntries);

        std::unordered_set<Stmt> stmts = {Stmt{StatementType::ASSIGN, 1}, Stmt{StatementType::ASSIGN, 2},
                                          Stmt{StatementType::ASSIGN, 3}, Stmt{StatementType::ASSIGN, 4}};
        pfw.setStmts(stmts);

        Synonym* stmtSyn = new Synonym(DesignEntityType::STMT, "s");
        NextStarTester{pfr, stmtSyn, new Wildcard()}.testSynonyms({*stmtSyn}).testSynonymValues({{"1", "3"}});
    }

    SECTION("Next(Wildcard, Synonym)") {
        std::unordered_set<std::pair<int, int>> nextStoreEntries{std::pair<int, int>{1, 2}, {3, 4}};
        pfw.setNextStore(nextStoreEntries);

        std::unordered_set<Stmt> stmts = {Stmt{StatementType::ASSIGN, 1}, Stmt{StatementType::ASSIGN, 2},
                                          Stmt{StatementType::ASSIGN, 3}, Stmt{StatementType::ASSIGN, 4}};
        pfw.setStmts(stmts);

        Synonym* stmtSyn = new Synonym(DesignEntityType::STMT, "s");
        NextStarTester{pfr, new Wildcard(), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{"2", "4"}});
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

        Synonym* stmtSyn1 = new Synonym(DesignEntityType::STMT, "s1");
        Synonym* stmtSyn2 = new Synonym(DesignEntityType::STMT, "s2");
        NextStarTester{pfr, stmtSyn1, stmtSyn2}
            .testSynonyms({*stmtSyn1, *stmtSyn2})
            .testSynonymValues({{"1", "3"}, {"2", "4"}});
    }
}
