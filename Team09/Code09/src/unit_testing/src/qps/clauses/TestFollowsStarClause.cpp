#include "PKB/PKBClient/PKBFacadeWriter.h"
#include "TesterHelper.h"
#include "catch.hpp"
#include "qps/clauseArguments/Wildcard.h"
#include "qps/clauses/SuchThatClause.h"

TEST_CASE("SuchThatClause evaluate for follows* relationship with no synonyms") {
    PKB pkb{};
    PKBFacadeWriter pfw{pkb};
    PKBFacadeReader pfr{pkb};

    SECTION("FollowsT(Integer, Integer)") {
        std::unordered_set<std::pair<int, int>> followsStoreEntries{std::pair<int, int>{1, 2},
                                                                    std::pair<int, int>{2, 3}};
        pfw.setFollowsStore(followsStoreEntries);
        FollowsStarTester{pfr, new Integer("1"), new Integer("2")}.testBoolean(true);
        FollowsStarTester{pfr, new Integer("1"), new Integer("3")}.testBoolean(true);
        FollowsStarTester{pfr, new Integer("2"), new Integer("3")}.testBoolean(true);
        FollowsStarTester{pfr, new Integer("2"), new Integer("1")}.testBoolean(false);
        FollowsStarTester{pfr, new Integer("1"), new Integer("1")}.testBoolean(false);
        FollowsStarTester{pfr, new Integer("2"), new Integer("2")}.testBoolean(false);
        FollowsStarTester{pfr, new Integer("3"), new Integer("1")}.testBoolean(false);
        FollowsStarTester{pfr, new Integer("1"), new Integer("5")}.testBoolean(false);
    }

    SECTION("FollowsT(Integer, Integer) / empty store") {
        pfw.setFollowsStore({});
        FollowsStarTester{pfr, new Integer("1"), new Integer("3")}.testBoolean(false);
    }

    SECTION("FollowsT(Integer, Wildcard)") {
        std::unordered_set<std::pair<int, int>> followsStoreEntries{std::pair<int, int>{1, 2},
                                                                    std::pair<int, int>{2, 3}};
        pfw.setFollowsStore(followsStoreEntries);
        FollowsStarTester{pfr, new Integer("1"), new Wildcard()}.testBoolean(true);
        FollowsStarTester{pfr, new Integer("2"), new Wildcard()}.testBoolean(true);
        FollowsStarTester{pfr, new Integer("3"), new Wildcard()}.testBoolean(false);
    }

    SECTION("FollowsT(Integer, Wildcard) / empty store") {
        pfw.setFollowsStore({});
        FollowsStarTester{pfr, new Integer("1"), new Wildcard()}.testBoolean(false);
        FollowsStarTester{pfr, new Wildcard(), new Wildcard()}.testBoolean(false);
    }

    SECTION("FollowsT(Wildcard, Integer)") {
        std::unordered_set<std::pair<int, int>> followsStoreEntries{std::pair<int, int>{1, 2},
                                                                    std::pair<int, int>{2, 3}};
        pfw.setFollowsStore(followsStoreEntries);
        FollowsStarTester{pfr, new Wildcard(), new Integer("1")}.testBoolean(false);
        FollowsStarTester{pfr, new Wildcard(), new Integer("2")}.testBoolean(true);
        FollowsStarTester{pfr, new Wildcard(), new Integer("3")}.testBoolean(true);
    }

    SECTION("FollowsT(Wildcard, Wildcard)") {
        std::unordered_set<std::pair<int, int>> followsStoreEntries{std::pair<int, int>{1, 2}};
        pfw.setFollowsStore(followsStoreEntries);
        FollowsStarTester{pfr, new Wildcard(), new Wildcard()}.testBoolean(true);
    }

    SECTION("FollowsT(Wildcard, Wildcard) / empty store") {
        pfw.setFollowsStore({});
        FollowsStarTester{pfr, new Wildcard(), new Wildcard()}.testBoolean(false);
    }
}

TEST_CASE("SuchThatClause evaluate for follows* relationship with 1 synonym") {
    PKB pkb{};
    PKBFacadeWriter pfw{pkb};
    PKBFacadeReader pfr{pkb};

    SECTION("FollowsT(Synonym, Integer)") {
        std::unordered_set<std::pair<int, int>> followsStoreEntries{std::pair<int, int>{1, 2},
                                                                    std::pair<int, int>{2, 3}};
        pfw.setFollowsStore(followsStoreEntries);
        Synonym* stmtSyn = new Synonym(DesignEntityType::STMT, "s");

        // Select s such that FollowsT(s, 3)
        FollowsStarTester{pfr, stmtSyn, new Integer("3")}.testSynonyms({*stmtSyn}).testSynonymValues({{"1", "2"}});
        // Select s such that FollowsT(s, 2)
        FollowsStarTester{pfr, stmtSyn, new Integer("2")}.testSynonyms({*stmtSyn}).testSynonymValues({{"1"}});
        // Select s such that FollowsT(s, 1)
        FollowsStarTester{pfr, stmtSyn, new Integer("1")}.testSynonyms({*stmtSyn}).testSynonymValues({{}});

        // Select s such that FollowsT(s, 5), out of bounds
        FollowsStarTester{pfr, stmtSyn, new Integer("5")}.testSynonyms({*stmtSyn}).testSynonymValues({{}});
    }

    SECTION("FollowsT(Integer, Synonym)") {
        std::unordered_set<std::pair<int, int>> followsStoreEntries{std::pair<int, int>{1, 2},
                                                                    std::pair<int, int>{2, 3}};
        pfw.setFollowsStore(followsStoreEntries);
        Synonym* stmtSyn = new Synonym(DesignEntityType::STMT, "s");

        // Select s such that FollowsT(1, s)
        FollowsStarTester{pfr, new Integer("1"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{"2", "3"}});
        // Select s such that FollowsT(2, s)
        FollowsStarTester{pfr, new Integer("2"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{"3"}});
        // Select s such that FollowsT(3, s)
        FollowsStarTester{pfr, new Integer("3"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{}});

        // Select s such that FollowsT(5, s), out of bounds
        FollowsStarTester{pfr, new Integer("5"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{}});
    }

    SECTION("FollowsT(Synonym, Wildcard)") {
        std::unordered_set<Stmt> stmts = {Stmt{StatementType::ASSIGN, 1}, Stmt{StatementType::ASSIGN, 2},
                                          Stmt{StatementType::ASSIGN, 3}, Stmt{StatementType::ASSIGN, 6}};

        std::unordered_set<std::pair<int, int>> followsStoreEntries{
            std::pair<int, int>{1, 2}, std::pair<int, int>{2, 3}, std::pair<int, int>{3, 6}};

        pfw.setStmts(stmts);
        pfw.setFollowsStore(followsStoreEntries);
        Synonym* stmtSyn = new Synonym(DesignEntityType::STMT, "s");

        // Select s such that FollowsT(s, _)
        FollowsStarTester{pfr, stmtSyn, new Wildcard()}.testSynonyms({*stmtSyn}).testSynonymValues({{"1", "2", "3"}});
    }

    SECTION("FollowsT(Wildcard, Synonym)") {
        std::unordered_set<Stmt> stmts = {Stmt{StatementType::ASSIGN, 1}, Stmt{StatementType::ASSIGN, 2},
                                          Stmt{StatementType::ASSIGN, 3}, Stmt{StatementType::ASSIGN, 6}};

        std::unordered_set<std::pair<int, int>> followsStoreEntries{
            std::pair<int, int>{1, 2}, std::pair<int, int>{2, 3}, std::pair<int, int>{3, 6}};

        pfw.setStmts(stmts);
        pfw.setFollowsStore(followsStoreEntries);
        Synonym* stmtSyn = new Synonym(DesignEntityType::STMT, "s");

        // Select s such that FollowsT(_, s)
        FollowsStarTester{pfr, new Wildcard(), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{"2", "3", "6"}});
    }
}

TEST_CASE("SuchThatClause evaluate for follows* relationship with 2 synonyms") {
    PKB pkb{};
    PKBFacadeWriter pfw{pkb};
    PKBFacadeReader pfr{pkb};

    SECTION("FollowsT(Synonym, Synonym)") {
        std::unordered_set<std::pair<int, int>> followsStoreEntries{
            std::pair<int, int>{1, 2}, std::pair<int, int>{2, 3}, std::pair<int, int>{3, 4}};

        std::unordered_set<Stmt> stmts = {Stmt{StatementType::ASSIGN, 1}, Stmt{StatementType::ASSIGN, 2},
                                          Stmt{StatementType::ASSIGN, 3}, Stmt{StatementType::ASSIGN, 4}};

        pfw.setStmts(stmts);
        pfw.setFollowsStore(followsStoreEntries);
        Synonym* stmtSyn1 = new Synonym(DesignEntityType::STMT, "s1");
        Synonym* stmtSyn2 = new Synonym(DesignEntityType::STMT, "s2");

        // Select s1 such that FollowsT(s1, s2)
        FollowsStarTester{pfr, stmtSyn1, stmtSyn2}
            .testSynonyms({*stmtSyn1, *stmtSyn2})
            .testSynonymValues({{"1", "1", "1", "2", "2", "3"}, {"2", "3", "4", "3", "4", "4"}});
    }
}
