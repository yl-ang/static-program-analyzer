#include "PKB/PKBClient/PKBFacadeWriter.h"
#include "TesterHelper.h"
#include "catch.hpp"
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

        std::unordered_set<std::pair<int, int>> nextStoreEntries{std::pair<int, int>{1, 2},
                                                                std::pair<int, int>{2, 3},
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

        AffectsTester{pfr, new Integer("1"), new Integer("2")}.testBoolean(false);
        AffectsTester{pfr, new Integer("1"), new Integer("3")}.testBoolean(true);
        AffectsTester{pfr, new Integer("1"), new Integer("4")}.testBoolean(false);
        AffectsTester{pfr, new Integer("2"), new Integer("3")}.testBoolean(false);
        AffectsTester{pfr, new Integer("2"), new Integer("4")}.testBoolean(false);
        AffectsTester{pfr, new Integer("3"), new Integer("4")}.testBoolean(true);

        AffectsTester{pfr, new Integer("4"), new Integer("3")}.testBoolean(false);
        AffectsTester{pfr, new Integer("4"), new Integer("2")}.testBoolean(false);
        AffectsTester{pfr, new Integer("4"), new Integer("1")}.testBoolean(false);
        AffectsTester{pfr, new Integer("3"), new Integer("2")}.testBoolean(false);
        AffectsTester{pfr, new Integer("3"), new Integer("1")}.testBoolean(false);
        AffectsTester{pfr, new Integer("2"), new Integer("1")}.testBoolean(false);

        AffectsTester{pfr, new Integer("1"), new Integer("1")}.testBoolean(false);
        AffectsTester{pfr, new Integer("2"), new Integer("2")}.testBoolean(false);
        AffectsTester{pfr, new Integer("3"), new Integer("3")}.testBoolean(false);
        AffectsTester{pfr, new Integer("4"), new Integer("4")}.testBoolean(false);
    }

    SECTION("Affects(integer, integer) / empty store") {
        std::unordered_set<Stmt> stmts = {Stmt{StatementType::ASSIGN, 1}, Stmt{StatementType::PRINT, 2},
                                          Stmt{StatementType::ASSIGN, 3}, Stmt{StatementType::ASSIGN, 4}};
        pfw.setVariables({});
        pfw.setStmts(stmts);
        pfw.setNextStore({});
        pfw.setStatementUsesStore({});
        pfw.setStatementModifiesStore({});

        AffectsTester{pfr, new Integer("1"), new Integer("2")}.testBoolean(false);
        AffectsTester{pfr, new Integer("1"), new Integer("3")}.testBoolean(false);
        AffectsTester{pfr, new Integer("1"), new Integer("4")}.testBoolean(false);
        AffectsTester{pfr, new Integer("2"), new Integer("3")}.testBoolean(false);
        AffectsTester{pfr, new Integer("2"), new Integer("4")}.testBoolean(false);
        AffectsTester{pfr, new Integer("3"), new Integer("4")}.testBoolean(false);

        AffectsTester{pfr, new Integer("4"), new Integer("1")}.testBoolean(false);
        AffectsTester{pfr, new Integer("4"), new Integer("2")}.testBoolean(false);
        AffectsTester{pfr, new Integer("4"), new Integer("3")}.testBoolean(false);
        AffectsTester{pfr, new Integer("3"), new Integer("2")}.testBoolean(false);
        AffectsTester{pfr, new Integer("3"), new Integer("1")}.testBoolean(false);
        AffectsTester{pfr, new Integer("2"), new Integer("1")}.testBoolean(false);

        AffectsTester{pfr, new Integer("1"), new Integer("1")}.testBoolean(false);
        AffectsTester{pfr, new Integer("2"), new Integer("2")}.testBoolean(false);
        AffectsTester{pfr, new Integer("3"), new Integer("3")}.testBoolean(false);
        AffectsTester{pfr, new Integer("4"), new Integer("4")}.testBoolean(false);
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

        std::unordered_set<std::pair<int, int>> nextStoreEntries{std::pair<int, int>{1, 2},
                                                                std::pair<int, int>{2, 3},
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

        AffectsTester{pfr, new Wildcard(), new Wildcard()}.testBoolean(true);
    }

    SECTION("Affects(wildcard, wildcard) / empty store") {
        std::unordered_set<Stmt> stmts = {Stmt{StatementType::ASSIGN, 1}, Stmt{StatementType::PRINT, 2},
                                          Stmt{StatementType::ASSIGN, 3}, Stmt{StatementType::ASSIGN, 4}};
        pfw.setVariables({});
        pfw.setStmts(stmts);
        pfw.setNextStore({});
        pfw.setStatementUsesStore({});
        pfw.setStatementModifiesStore({});

        AffectsTester{pfr, new Wildcard(), new Wildcard()}.testBoolean(false);
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

        std::unordered_set<std::pair<int, int>> nextStoreEntries{std::pair<int, int>{1, 2},
                                                                std::pair<int, int>{2, 3},
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

        AffectsTester{pfr, new Integer("1"), new Wildcard()}.testBoolean(true);
        AffectsTester{pfr, new Integer("2"), new Wildcard()}.testBoolean(false);
        AffectsTester{pfr, new Integer("3"), new Wildcard()}.testBoolean(true);
        AffectsTester{pfr, new Integer("4"), new Wildcard()}.testBoolean(false);

        AffectsTester{pfr, new Wildcard(), new Integer("4")}.testBoolean(true);
        AffectsTester{pfr, new Wildcard(), new Integer("3")}.testBoolean(true);
        AffectsTester{pfr, new Wildcard(), new Integer("2")}.testBoolean(false);
        AffectsTester{pfr, new Wildcard(), new Integer("1")}.testBoolean(false);
    }

    SECTION("Affects(integer, wildcard) / empty store") {
        std::unordered_set<Stmt> stmts = {Stmt{StatementType::ASSIGN, 1}, Stmt{StatementType::PRINT, 2},
                                          Stmt{StatementType::ASSIGN, 3}, Stmt{StatementType::ASSIGN, 4}};

        pfw.setVariables({});
        pfw.setStmts(stmts);
        pfw.setNextStore({});
        pfw.setStatementUsesStore({});
        pfw.setStatementModifiesStore({});

        AffectsTester{pfr, new Integer("1"), new Wildcard()}.testBoolean(false);
        AffectsTester{pfr, new Integer("2"), new Wildcard()}.testBoolean(false);
        AffectsTester{pfr, new Integer("3"), new Wildcard()}.testBoolean(false);
        AffectsTester{pfr, new Integer("4"), new Wildcard()}.testBoolean(false);

        AffectsTester{pfr, new Wildcard(), new Integer("4")}.testBoolean(false);
        AffectsTester{pfr, new Wildcard(), new Integer("3")}.testBoolean(false);
        AffectsTester{pfr, new Wildcard(), new Integer("2")}.testBoolean(false);
        AffectsTester{pfr, new Wildcard(), new Integer("1")}.testBoolean(false);
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

        std::unordered_set<std::pair<int, int>> nextStoreEntries{std::pair<int, int>{1, 2},
                                                                std::pair<int, int>{2, 3},
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

        Synonym* stmtSyn = new Synonym(DesignEntityType::ASSIGN, "s");

        // Select s such that Affects(s, 4)
        AffectsTester{pfr, stmtSyn, new Integer("4")}.testSynonyms({*stmtSyn}).testSynonymValues({{"3"}});
        // Select s such that Affects(s, 3)
        AffectsTester{pfr, stmtSyn, new Integer("3")}.testSynonyms({*stmtSyn}).testSynonymValues({{"1"}});
        // Select s such that Affects(s, 2)
        AffectsTester{pfr, stmtSyn, new Integer("2")}.testSynonyms({*stmtSyn}).testSynonymValues({{}});
        // Select s such that Affects(s, 1)
        AffectsTester{pfr, stmtSyn, new Integer("1")}.testSynonyms({*stmtSyn}).testSynonymValues({{}});

        // Select s such that Affects(s, 5), out of bounds
        AffectsTester{pfr, stmtSyn, new Integer("5")}.testSynonyms({*stmtSyn}).testSynonymValues({{}});
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

        std::unordered_set<std::pair<int, int>> nextStoreEntries{std::pair<int, int>{1, 2},
                                                                std::pair<int, int>{2, 3},
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

        Synonym* stmtSyn = new Synonym(DesignEntityType::ASSIGN, "s");

        // Select s such that Affects(4, s)
        AffectsTester{pfr, new Integer("4"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{}});
        // Select s such that Affects(3, s)
        AffectsTester{pfr, new Integer("3"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{"4"}});
        // Select s such that Affects(2, s)
        AffectsTester{pfr, new Integer("2"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{}});
        // Select s such that Affects(1, s)
        AffectsTester{pfr, new Integer("1"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{"3"}});

        // Select s such that Affects(s, 5), out of bounds
        AffectsTester{pfr, new Integer("5"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{}});
    }

    // SECTION("Affects(synonym, wildcard)") {

    // }

    // SECTION("Affects(wildcard, synonym)") {

    // }
}

// TEST_CASE("SuchThatClause evaluate for affects relationship, both synonym") {

// }

// TEST_CASE("SuchThatClause evaluate for affects relationship, no synonym") {
//     PKB pkb{};
//     PKBFacadeWriter pfw{pkb};
//     PKBFacadeReader pfr{pkb};

//     SECTION("Affects(synonym, synonym)") {
//         std::unordered_set<std::pair<int, int>> parentStoreEntries{std::pair<int, int>{1, 2},
//                                                                    std::pair<int, int>{2, 3}};
//         pfw.setParentStore(parentStoreEntries);
//         ParentStarTester{pfr, new Integer("1"), new Integer("2")}.testBoolean(true);
//         ParentStarTester{pfr, new Integer("1"), new Integer("3")}.testBoolean(true);
//         ParentStarTester{pfr, new Integer("2"), new Integer("3")}.testBoolean(true);
//         ParentStarTester{pfr, new Integer("2"), new Integer("1")}.testBoolean(false);
//         ParentStarTester{pfr, new Integer("1"), new Integer("1")}.testBoolean(false);
//         ParentStarTester{pfr, new Integer("2"), new Integer("2")}.testBoolean(false);
//     }

//     SECTION("ParentT(Integer, Integer) / empty store") {
//         pfw.setParentStore({});
//         ParentStarTester{pfr, new Integer("1"), new Integer("2")}.testBoolean(false);
//     }

//     SECTION("ParentT(Integer, Wildcard)") {
//         std::unordered_set<std::pair<int, int>> parentStoreEntries{std::pair<int, int>{1, 2},
//                                                                    std::pair<int, int>{2, 3}};
//         pfw.setParentStore(parentStoreEntries);
//         ParentStarTester{pfr, new Integer("1"), new Wildcard()}.testBoolean(true);
//         ParentStarTester{pfr, new Integer("2"), new Wildcard()}.testBoolean(true);
//         ParentStarTester{pfr, new Integer("3"), new Wildcard()}.testBoolean(false);
//     }

//     SECTION("ParentT(Integer, Wildcard) / empty store") {
//         pfw.setParentStore({});
//         ParentStarTester{pfr, new Integer("1"), new Wildcard()}.testBoolean(false);
//         ParentStarTester{pfr, new Wildcard(), new Wildcard()}.testBoolean(false);
//     }

//     SECTION("ParentT(Wildcard, Integer)") {
//         std::unordered_set<std::pair<int, int>> parentStoreEntries{std::pair<int, int>{1, 2},
//                                                                    std::pair<int, int>{2, 3}};
//         pfw.setParentStore(parentStoreEntries);
//         ParentStarTester{pfr, new Wildcard(), new Integer("3")}.testBoolean(true);
//         ParentStarTester{pfr, new Wildcard(), new Integer("2")}.testBoolean(true);
//         ParentStarTester{pfr, new Wildcard(), new Integer("1")}.testBoolean(false);
//     }

//     SECTION("ParentT(Wildcard, Wildcard)") {
//         std::unordered_set<std::pair<int, int>> parentStoreEntries{std::pair<int, int>{1, 2},
//                                                                    std::pair<int, int>{2, 3}};
//         pfw.setParentStore(parentStoreEntries);
//         ParentStarTester{pfr, new Wildcard(), new Wildcard()}.testBoolean(true);
//     }

//     SECTION("ParentT(Wildcard, Wildcard) / empty store") {
//         pfw.setParentStore({});
//         ParentStarTester{pfr, new Wildcard(), new Wildcard()}.testBoolean(false);
//     }
// }

// TEST_CASE("SuchThatClause evaluate for affects relationship with 1 synonym") {
//     PKB pkb{};
//     PKBFacadeWriter pfw{pkb};
//     PKBFacadeReader pfr{pkb};

//     SECTION("ParentT(Synonym, Integer)") {
//         std::unordered_set<std::pair<int, int>> parentStoreEntries{std::pair<int, int>{1, 2},
//                                                                    std::pair<int, int>{2, 3}};
//         pfw.setParentStore(parentStoreEntries);
//         Synonym* stmtSyn = new Synonym(DesignEntityType::STMT, "s");

//         // Select s such that ParentT(s, 2)
//         ParentStarTester{pfr, stmtSyn, new Integer("2")}.testSynonyms({*stmtSyn}).testSynonymValues({{"1"}});
//         // Select s such that ParentT(s, 3)
//         ParentStarTester{pfr, stmtSyn, new Integer("3")}.testSynonyms({*stmtSyn}).testSynonymValues({{"1", "2"}});
//         // Select s such that ParentT(s, 1)
//         ParentStarTester{pfr, stmtSyn, new Integer("1")}.testSynonyms({*stmtSyn}).testSynonymValues({{}});

//         // Select s such that ParentT(s, 5), out of bounds
//         ParentStarTester{pfr, stmtSyn, new Integer("5")}.testSynonyms({*stmtSyn}).testSynonymValues({{}});
//     }

//     SECTION("ParentT(Integer, Synonym)") {
//         std::unordered_set<std::pair<int, int>> parentStoreEntries{std::pair<int, int>{1, 2},
//                                                                    std::pair<int, int>{2, 3}};
//         pfw.setParentStore(parentStoreEntries);
//         Synonym* stmtSyn = new Synonym(DesignEntityType::STMT, "s");

//         // Select s such that ParentT(1, s)
//         ParentStarTester{pfr, new Integer("1"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{"2", "3"}});
//         // Select s such that ParentT(2, s)
//         ParentStarTester{pfr, new Integer("2"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{"3"}});
//         // Select s such that ParentT(3, s)
//         ParentStarTester{pfr, new Integer("3"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{}});

//         // Select s such that ParentT(5, s), out of bounds
//         ParentStarTester{pfr, new Integer("5"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{}});
//     }

//     SECTION("ParentT(Synonym, Wildcard)") {
//         std::unordered_set<Stmt> stmts = {Stmt{StatementType::ASSIGN, 1}, Stmt{StatementType::ASSIGN, 2},
//                                           Stmt{StatementType::ASSIGN, 3}};

//         std::unordered_set<std::pair<int, int>> parentStoreEntries{std::pair<int, int>{1, 2},
//                                                                    std::pair<int, int>{2, 3}};

//         pfw.setStmts(stmts);
//         pfw.setParentStore(parentStoreEntries);
//         Synonym* stmtSyn = new Synonym(DesignEntityType::STMT, "s");

//         // Select s such that ParentT(s, _)
//         ParentStarTester{pfr, stmtSyn, new Wildcard()}.testSynonyms({*stmtSyn}).testSynonymValues({{"1", "2"}});
//     }

//     SECTION("ParentT(Wildcard, Synonym)") {
//         std::unordered_set<Stmt> stmts = {Stmt{StatementType::ASSIGN, 1}, Stmt{StatementType::ASSIGN, 2},
//                                           Stmt{StatementType::ASSIGN, 3}};

//         std::unordered_set<std::pair<int, int>> parentStoreEntries{std::pair<int, int>{1, 2},
//                                                                    std::pair<int, int>{2, 3}};

//         pfw.setStmts(stmts);
//         pfw.setParentStore(parentStoreEntries);
//         Synonym* stmtSyn = new Synonym(DesignEntityType::STMT, "s");

//         // Select s such that ParentT(_, s)
//         ParentStarTester{pfr, new Wildcard(), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{"2", "3"}});
//     }
// }

// TEST_CASE("SuchThatClause evaluate for affects relationship with 2 synonyms") {
//     PKB pkb{};
//     PKBFacadeWriter pfw{pkb};
//     PKBFacadeReader pfr{pkb};

//     SECTION("ParentT(Synonym, Synonym)") {
//         std::unordered_set<std::pair<int, int>> parentStoreEntries{std::pair<int, int>{1, 2}, std::pair<int, int>{1, 3},
//                                                                    std::pair<int, int>{3, 4}};

//         std::unordered_set<Stmt> stmts = {Stmt{StatementType::ASSIGN, 1}, Stmt{StatementType::ASSIGN, 2},
//                                           Stmt{StatementType::ASSIGN, 3}, Stmt{StatementType::ASSIGN, 4}};

//         pfw.setStmts(stmts);
//         pfw.setParentStore(parentStoreEntries);
//         Synonym* stmtSyn1 = new Synonym(DesignEntityType::STMT, "s1");
//         Synonym* stmtSyn2 = new Synonym(DesignEntityType::STMT, "s2");

//         // Select s1 such that ParentT(s1, s2)
//         ParentStarTester{pfr, stmtSyn1, stmtSyn2}
//             .testSynonyms({*stmtSyn1, *stmtSyn2})
//             .testSynonymValues({{"1", "1", "1", "3"}, {"2", "3", "4", "4"}});
//     }
// }
