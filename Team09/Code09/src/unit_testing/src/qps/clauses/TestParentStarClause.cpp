#include "TesterHelper.h"
#include "catch.hpp"
#include "pkb/PKBClient/PKBFacadeWriter.h"
#include "qps/clauseArguments/Wildcard.h"
#include "qps/clauses/SuchThatClause.h"

TEST_CASE("SuchThatClause evaluate for parent* relationship with no synonyms") {
    PKB pkb{};
    PKBFacadeWriter pfw{pkb};
    PKBFacadeReader pfr{pkb};

    SECTION("ParentT(Integer, Integer)") {
        std::unordered_set<std::pair<int, int>> parentStoreEntries{std::pair<int, int>{1, 2},
                                                                   std::pair<int, int>{2, 3}};
        pfw.setParentStore(parentStoreEntries);
        ParentStarTester{pfr, std::make_shared<Integer>("1"), std::make_shared<Integer>("2")}.testBoolean(true);
        ParentStarTester{pfr, std::make_shared<Integer>("1"), std::make_shared<Integer>("3")}.testBoolean(true);
        ParentStarTester{pfr, std::make_shared<Integer>("2"), std::make_shared<Integer>("3")}.testBoolean(true);
        ParentStarTester{pfr, std::make_shared<Integer>("2"), std::make_shared<Integer>("1")}.testBoolean(false);
        ParentStarTester{pfr, std::make_shared<Integer>("1"), std::make_shared<Integer>("1")}.testBoolean(false);
        ParentStarTester{pfr, std::make_shared<Integer>("2"), std::make_shared<Integer>("2")}.testBoolean(false);
    }

    SECTION("ParentT(Integer, Integer) / empty store") {
        pfw.setParentStore({});
        ParentStarTester{pfr, std::make_shared<Integer>("1"), std::make_shared<Integer>("2")}.testBoolean(false);
    }

    SECTION("ParentT(Integer, Wildcard)") {
        std::unordered_set<std::pair<int, int>> parentStoreEntries{std::pair<int, int>{1, 2},
                                                                   std::pair<int, int>{2, 3}};
        pfw.setParentStore(parentStoreEntries);
        ParentStarTester{pfr, std::make_shared<Integer>("1"), std::make_shared<Wildcard>()}.testBoolean(true);
        ParentStarTester{pfr, std::make_shared<Integer>("2"), std::make_shared<Wildcard>()}.testBoolean(true);
        ParentStarTester{pfr, std::make_shared<Integer>("3"), std::make_shared<Wildcard>()}.testBoolean(false);
    }

    SECTION("ParentT(Integer, Wildcard) / empty store") {
        pfw.setParentStore({});
        ParentStarTester{pfr, std::make_shared<Integer>("1"), std::make_shared<Wildcard>()}.testBoolean(false);
        ParentStarTester{pfr, std::make_shared<Wildcard>(), std::make_shared<Wildcard>()}.testBoolean(false);
    }

    SECTION("ParentT(Wildcard, Integer)") {
        std::unordered_set<std::pair<int, int>> parentStoreEntries{std::pair<int, int>{1, 2},
                                                                   std::pair<int, int>{2, 3}};
        pfw.setParentStore(parentStoreEntries);
        ParentStarTester{pfr, std::make_shared<Wildcard>(), std::make_shared<Integer>("3")}.testBoolean(true);
        ParentStarTester{pfr, std::make_shared<Wildcard>(), std::make_shared<Integer>("2")}.testBoolean(true);
        ParentStarTester{pfr, std::make_shared<Wildcard>(), std::make_shared<Integer>("1")}.testBoolean(false);
    }

    SECTION("ParentT(Wildcard, Wildcard)") {
        std::unordered_set<std::pair<int, int>> parentStoreEntries{std::pair<int, int>{1, 2},
                                                                   std::pair<int, int>{2, 3}};
        pfw.setParentStore(parentStoreEntries);
        ParentStarTester{pfr, std::make_shared<Wildcard>(), std::make_shared<Wildcard>()}.testBoolean(true);
    }

    SECTION("ParentT(Wildcard, Wildcard) / empty store") {
        pfw.setParentStore({});
        ParentStarTester{pfr, std::make_shared<Wildcard>(), std::make_shared<Wildcard>()}.testBoolean(false);
    }
}

TEST_CASE("SuchThatClause evaluate for parent* relationship with 1 synonym") {
    PKB pkb{};
    PKBFacadeWriter pfw{pkb};
    PKBFacadeReader pfr{pkb};

    SECTION("ParentT(Synonym, Integer)") {
        std::unordered_set<std::pair<int, int>> parentStoreEntries{std::pair<int, int>{1, 2},
                                                                   std::pair<int, int>{2, 3}};
        pfw.setParentStore(parentStoreEntries);
        std::shared_ptr<Synonym> stmtSyn = std::make_shared<Synonym>(DesignEntityType::STMT, "s");

        std::unordered_set<Stmt> stmts = {Stmt{StatementType::ASSIGN, 1}, Stmt{StatementType::ASSIGN, 2},
                                          Stmt{StatementType::ASSIGN, 3}, Stmt{StatementType::ASSIGN, 4}};

        pfw.setStmts(stmts);

        // Select s such that ParentT(s, 2)
        ParentStarTester{pfr, stmtSyn, std::make_shared<Integer>("2")}
            .testSynonyms({*stmtSyn})
            .testSynonymValues({{"1"}});
        // Select s such that ParentT(s, 3)
        ParentStarTester{pfr, stmtSyn, std::make_shared<Integer>("3")}
            .testSynonyms({*stmtSyn})
            .testSynonymValues({{"1", "2"}});
        // Select s such that ParentT(s, 1)
        ParentStarTester{pfr, stmtSyn, std::make_shared<Integer>("1")}.testSynonyms({*stmtSyn}).testSynonymValues({{}});

        // Select s such that ParentT(s, 5), out of bounds
        ParentStarTester{pfr, stmtSyn, std::make_shared<Integer>("5")}.testSynonyms({*stmtSyn}).testSynonymValues({{}});
    }

    SECTION("ParentT(Integer, Synonym)") {
        std::unordered_set<std::pair<int, int>> parentStoreEntries{std::pair<int, int>{1, 2},
                                                                   std::pair<int, int>{2, 3}};
        pfw.setParentStore(parentStoreEntries);
        std::shared_ptr<Synonym> stmtSyn = std::make_shared<Synonym>(DesignEntityType::STMT, "s");

        std::unordered_set<Stmt> stmts = {Stmt{StatementType::ASSIGN, 1}, Stmt{StatementType::ASSIGN, 2},
                                          Stmt{StatementType::ASSIGN, 3}, Stmt{StatementType::ASSIGN, 4}};

        pfw.setStmts(stmts);

        // Select s such that ParentT(1, s)
        ParentStarTester{pfr, std::make_shared<Integer>("1"), stmtSyn}
            .testSynonyms({*stmtSyn})
            .testSynonymValues({{"2", "3"}});
        // Select s such that ParentT(2, s)
        ParentStarTester{pfr, std::make_shared<Integer>("2"), stmtSyn}
            .testSynonyms({*stmtSyn})
            .testSynonymValues({{"3"}});
        // Select s such that ParentT(3, s)
        ParentStarTester{pfr, std::make_shared<Integer>("3"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{}});

        // Select s such that ParentT(5, s), out of bounds
        ParentStarTester{pfr, std::make_shared<Integer>("5"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{}});
    }

    SECTION("ParentT(Synonym, Wildcard)") {
        std::unordered_set<Stmt> stmts = {Stmt{StatementType::ASSIGN, 1}, Stmt{StatementType::ASSIGN, 2},
                                          Stmt{StatementType::ASSIGN, 3}};

        std::unordered_set<std::pair<int, int>> parentStoreEntries{std::pair<int, int>{1, 2},
                                                                   std::pair<int, int>{2, 3}};

        pfw.setStmts(stmts);
        pfw.setParentStore(parentStoreEntries);
        std::shared_ptr<Synonym> stmtSyn = std::make_shared<Synonym>(DesignEntityType::STMT, "s");

        // Select s such that ParentT(s, _)
        ParentStarTester{pfr, stmtSyn, std::make_shared<Wildcard>()}
            .testSynonyms({*stmtSyn})
            .testSynonymValues({{"1", "2"}});
    }

    SECTION("ParentT(Wildcard, Synonym)") {
        std::unordered_set<Stmt> stmts = {Stmt{StatementType::ASSIGN, 1}, Stmt{StatementType::ASSIGN, 2},
                                          Stmt{StatementType::ASSIGN, 3}};

        std::unordered_set<std::pair<int, int>> parentStoreEntries{std::pair<int, int>{1, 2},
                                                                   std::pair<int, int>{2, 3}};

        pfw.setStmts(stmts);
        pfw.setParentStore(parentStoreEntries);
        std::shared_ptr<Synonym> stmtSyn = std::make_shared<Synonym>(DesignEntityType::STMT, "s");

        // Select s such that ParentT(_, s)
        ParentStarTester{pfr, std::make_shared<Wildcard>(), stmtSyn}
            .testSynonyms({*stmtSyn})
            .testSynonymValues({{"2", "3"}});
    }
}

TEST_CASE("SuchThatClause evaluate for parent* relationship with 2 synonyms") {
    PKB pkb{};
    PKBFacadeWriter pfw{pkb};
    PKBFacadeReader pfr{pkb};

    SECTION("ParentT(Synonym, Synonym)") {
        std::unordered_set<std::pair<int, int>> parentStoreEntries{std::pair<int, int>{1, 2}, std::pair<int, int>{1, 3},
                                                                   std::pair<int, int>{3, 4}};

        std::unordered_set<Stmt> stmts = {Stmt{StatementType::ASSIGN, 1}, Stmt{StatementType::ASSIGN, 2},
                                          Stmt{StatementType::ASSIGN, 3}, Stmt{StatementType::ASSIGN, 4}};

        pfw.setStmts(stmts);
        pfw.setParentStore(parentStoreEntries);
        std::shared_ptr<Synonym> stmtSyn1 = std::make_shared<Synonym>(DesignEntityType::STMT, "s1");
        std::shared_ptr<Synonym> stmtSyn2 = std::make_shared<Synonym>(DesignEntityType::STMT, "s2");

        // Select s1 such that ParentT(s1, s2)
        ParentStarTester{pfr, stmtSyn1, stmtSyn2}
            .testSynonyms({*stmtSyn1, *stmtSyn2})
            .testSynonymValues({{"1", "1", "1", "3"}, {"2", "3", "4", "4"}});
    }
}
