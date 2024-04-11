#include "PKB/PKBClient/PKBFacadeWriter.h"
#include "TesterHelper.h"
#include "catch.hpp"
#include "qps/clauseArguments/Wildcard.h"
#include "qps/clauses/SuchThatClause.h"

TEST_CASE("SuchThatClause evaluate for follows relationship with no synonyms") {
    PKB pkb{};
    PKBFacadeWriter pfw{pkb};
    PKBFacadeReader pfr{pkb};

    SECTION("Follows(Integer, Integer)") {
        std::unordered_set<std::pair<int, int>> followsStoreEntries{std::pair<int, int>{1, 2}};
        pfw.setFollowsStore(followsStoreEntries);
        FollowsTester{pfr, std::make_shared<Integer>("1"), std::make_shared<Integer>("2")}.testBoolean(true);
        FollowsTester{pfr, std::make_shared<Integer>("2"), std::make_shared<Integer>("1")}.testBoolean(false);
        FollowsTester{pfr, std::make_shared<Integer>("1"), std::make_shared<Integer>("1")}.testBoolean(false);
        FollowsTester{pfr, std::make_shared<Integer>("2"), std::make_shared<Integer>("2")}.testBoolean(false);

        FollowsTester{pfr, std::make_shared<Integer>("1"), std::make_shared<Integer>("2"), true}.testBoolean(false);
        FollowsTester{pfr, std::make_shared<Integer>("2"), std::make_shared<Integer>("1"), true}.testBoolean(true);
        FollowsTester{pfr, std::make_shared<Integer>("1"), std::make_shared<Integer>("1"), true}.testBoolean(true);
        FollowsTester{pfr, std::make_shared<Integer>("2"), std::make_shared<Integer>("2"), true}.testBoolean(true);
    }

    SECTION("Follows(Integer, Integer) / empty store") {
        pfw.setFollowsStore({});
        FollowsTester{pfr, std::make_shared<Integer>("1"), std::make_shared<Integer>("2")}.testBoolean(false);
    }

    SECTION("Follows(Integer, Wildcard)") {
        std::unordered_set<std::pair<int, int>> followsStoreEntries{std::pair<int, int>{1, 2}};
        pfw.setFollowsStore(followsStoreEntries);
        FollowsTester{pfr, std::make_shared<Integer>("1"), std::make_shared<Wildcard>()}.testBoolean(true);
        FollowsTester{pfr, std::make_shared<Integer>("2"), std::make_shared<Wildcard>()}.testBoolean(false);
    }

    SECTION("Follows(Integer, Wildcard) / empty store") {
        pfw.setFollowsStore({});
        FollowsTester{pfr, std::make_shared<Integer>("1"), std::make_shared<Wildcard>()}.testBoolean(false);
        FollowsTester{pfr, std::make_shared<Wildcard>(), std::make_shared<Wildcard>()}.testBoolean(false);
    }

    SECTION("Follows(Wildcard, Integer)") {
        std::unordered_set<std::pair<int, int>> followsStoreEntries{std::pair<int, int>{1, 2}};
        pfw.setFollowsStore(followsStoreEntries);
        FollowsTester{pfr, std::make_shared<Wildcard>(), std::make_shared<Integer>("2")}.testBoolean(true);
        FollowsTester{pfr, std::make_shared<Wildcard>(), std::make_shared<Integer>("1")}.testBoolean(false);
    }

    SECTION("Follows(Wildcard, Wildcard)") {
        std::unordered_set<std::pair<int, int>> followsStoreEntries{std::pair<int, int>{1, 2}};
        pfw.setFollowsStore(followsStoreEntries);
        FollowsTester{pfr, std::make_shared<Wildcard>(), std::make_shared<Wildcard>()}.testBoolean(true);
    }

    SECTION("Follows(Wildcard, Wildcard) / empty store") {
        pfw.setFollowsStore({});
        FollowsTester{pfr, std::make_shared<Wildcard>(), std::make_shared<Wildcard>()}.testBoolean(false);
    }
}

TEST_CASE("SuchThatClause evaluate for follows relationship with 1 synonym") {
    PKB pkb{};
    PKBFacadeWriter pfw{pkb};
    PKBFacadeReader pfr{pkb};

    SECTION("Follows(Synonym, Integer)") {
        std::unordered_set<std::pair<int, int>> followsStoreEntries{std::pair<int, int>{1, 2}};
        pfw.setFollowsStore(followsStoreEntries);
        std::shared_ptr<Synonym> stmtSyn = std::make_shared<Synonym>(DesignEntityType::STMT, "s");

        std::unordered_set<Stmt> stmts = {Stmt{StatementType::ASSIGN, 1}, Stmt{StatementType::ASSIGN, 2},
                                          Stmt{StatementType::ASSIGN, 3}};

        pfw.setStmts(stmts);

        // Select s such that Follows(s, 2)
        FollowsTester{pfr, stmtSyn, std::make_shared<Integer>("2")}.testSynonyms({*stmtSyn}).testSynonymValues({{"1"}});
        // Select s such that Follows(s, 1)
        FollowsTester{pfr, stmtSyn, std::make_shared<Integer>("1")}.testSynonyms({*stmtSyn}).testSynonymValues({{}});

        // Select s such that Follows(s, 5), out of bounds
        FollowsTester{pfr, stmtSyn, std::make_shared<Integer>("5")}.testSynonyms({*stmtSyn}).testSynonymValues({{}});
    }

    SECTION("Follows(Integer, Synonym)") {
        std::unordered_set<std::pair<int, int>> followsStoreEntries{std::pair<int, int>{1, 2}};
        pfw.setFollowsStore(followsStoreEntries);
        std::shared_ptr<Synonym> stmtSyn = std::make_shared<Synonym>(DesignEntityType::STMT, "s");

        std::unordered_set<Stmt> stmts = {Stmt{StatementType::ASSIGN, 1}, Stmt{StatementType::ASSIGN, 2},
                                          Stmt{StatementType::ASSIGN, 3}};

        pfw.setStmts(stmts);

        // Select s such that Follows(1, s)
        FollowsTester{pfr, std::make_shared<Integer>("1"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{"2"}});
        // Select s such that Follows(2, s)
        FollowsTester{pfr, std::make_shared<Integer>("2"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{}});

        // Select s such that Follows(5, s), out of bounds
        FollowsTester{pfr, std::make_shared<Integer>("5"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{}});
    }

    SECTION("Follows(Synonym, Wildcard)") {
        std::unordered_set<Stmt> stmts = {Stmt{StatementType::ASSIGN, 1}, Stmt{StatementType::ASSIGN, 2},
                                          Stmt{StatementType::ASSIGN, 3}};

        std::unordered_set<std::pair<int, int>> followsStoreEntries{std::pair<int, int>{1, 2},
                                                                    std::pair<int, int>{2, 3}};

        pfw.setStmts(stmts);
        pfw.setFollowsStore(followsStoreEntries);
        std::shared_ptr<Synonym> stmtSyn = std::make_shared<Synonym>(DesignEntityType::STMT, "s");

        // Select s such that Follows(s, _)
        FollowsTester{pfr, stmtSyn, std::make_shared<Wildcard>()}
            .testSynonyms({*stmtSyn})
            .testSynonymValues({{"1", "2"}});
    }

    SECTION("Follows(Wildcard, Synonym)") {
        std::unordered_set<Stmt> stmts = {Stmt{StatementType::ASSIGN, 1}, Stmt{StatementType::ASSIGN, 2},
                                          Stmt{StatementType::ASSIGN, 3}};

        std::unordered_set<std::pair<int, int>> followsStoreEntries{std::pair<int, int>{1, 2},
                                                                    std::pair<int, int>{2, 3}};

        pfw.setStmts(stmts);
        pfw.setFollowsStore(followsStoreEntries);
        std::shared_ptr<Synonym> stmtSyn = std::make_shared<Synonym>(DesignEntityType::STMT, "s");

        // Select s such that Follows(_, s)
        FollowsTester{pfr, std::make_shared<Wildcard>(), stmtSyn}
            .testSynonyms({*stmtSyn})
            .testSynonymValues({{"2", "3"}});
    }
}

TEST_CASE("SuchThatClause evaluate for follows relationship with 2 synonyms") {
    PKB pkb{};
    PKBFacadeWriter pfw{pkb};
    PKBFacadeReader pfr{pkb};

    SECTION("Follows(Synonym, Synonym)") {
        std::unordered_set<std::pair<int, int>> followsStoreEntries{
            std::pair<int, int>{1, 2}, std::pair<int, int>{2, 3}, std::pair<int, int>{3, 4}};

        std::unordered_set<Stmt> stmts = {Stmt{StatementType::ASSIGN, 1}, Stmt{StatementType::ASSIGN, 2},
                                          Stmt{StatementType::ASSIGN, 3}, Stmt{StatementType::ASSIGN, 4}};

        pfw.setStmts(stmts);
        pfw.setFollowsStore(followsStoreEntries);
        std::shared_ptr<Synonym> stmtSyn1 = std::make_shared<Synonym>(DesignEntityType::STMT, "s1");
        std::shared_ptr<Synonym> stmtSyn2 = std::make_shared<Synonym>(DesignEntityType::STMT, "s2");

        // Select s1 such that Follows(s1, s2)
        FollowsTester{pfr, stmtSyn1, stmtSyn2}
            .testSynonyms({*stmtSyn1, *stmtSyn2})
            .testSynonymValues({{"1", "2", "3"}, {"2", "3", "4"}});

        FollowsTester{pfr, stmtSyn1, stmtSyn2, true}
            .testSynonyms({*stmtSyn1, *stmtSyn2})
            .testSynonymValues({{"1", "1", "1", "2", "2", "2", "3", "3", "3", "4", "4", "4", "4"},
                                {"1", "3", "4", "1", "2", "4", "1", "2", "3", "1", "2", "3", "4"}});
    }
}
