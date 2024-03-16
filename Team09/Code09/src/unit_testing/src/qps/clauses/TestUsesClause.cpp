#include "PKB/PKBClient/PKBFacadeWriter.h"
#include "TesterHelper.h"
#include "catch.hpp"
#include "qps/clauseArguments/Literal.h"
#include "qps/clauseArguments/Wildcard.h"
#include "qps/clauses/SuchThatClause.h"

TEST_CASE("SuchThatClause evaluate for Uses relationship with no synonyms") {
    PKB pkb{};
    PKBFacadeWriter pfw{pkb};
    PKBFacadeReader pfr{pkb};

    SECTION("Uses(Integer, Variable)") {
        std::unordered_set<std::pair<int, std::string>> usesStoreEntries{std::pair<int, std::string>{1, "x"}};
        pfw.setStatementUsesStore(usesStoreEntries);
        UsesTester{pfr, new Integer("1"), new Literal("x")}.testBoolean(true);
        UsesTester{pfr, new Integer("2"), new Literal("x")}.testBoolean(false);
        UsesTester{pfr, new Integer("1"), new Literal("y")}.testBoolean(false);
    }

    SECTION("Uses(Integer, Variable) / empty store") {
        pfw.setStatementUsesStore({});
        UsesTester{pfr, new Integer("1"), new Literal("x")}.testBoolean(false);
    }

    SECTION("Uses(Integer, Wildcard)") {
        std::unordered_set<std::pair<int, std::string>> usesStoreEntries{std::pair<int, std::string>{1, "x"},
                                                                         std::pair<int, std::string>{2, "y"}};
        pfw.setStatementUsesStore(usesStoreEntries);
        UsesTester{pfr, new Integer("1"), new Wildcard()}.testBoolean(true);
        UsesTester{pfr, new Integer("2"), new Wildcard()}.testBoolean(true);
        UsesTester{pfr, new Integer("3"), new Wildcard()}.testBoolean(false);
    }

    SECTION("Uses(Integer, Wildcard) / empty store") {
        pfw.setStatementUsesStore({});
        UsesTester{pfr, new Integer("1"), new Wildcard()}.testBoolean(false);
        UsesTester{pfr, new Wildcard(), new Wildcard()}.testBoolean(false);
    }
}

TEST_CASE("SuchThatClause evaluate for Uses relationship with 1 synonym") {
    PKB pkb{};
    PKBFacadeWriter pfw{pkb};
    PKBFacadeReader pfr{pkb};

    SECTION("Uses(Synonym, Integer)") {
        std::unordered_set<std::pair<int, std::string>> usesStoreEntries{std::pair<int, std::string>{1, "x"},
                                                                         std::pair<int, std::string>{2, "y"},
                                                                         std::pair<int, std::string>{3, "y"}};
        pfw.setStatementUsesStore(usesStoreEntries);

        std::unordered_set<Stmt> stmts = {Stmt{StatementType::READ, 1}, Stmt{StatementType::ASSIGN, 2},
                                          Stmt{StatementType::READ, 3}, Stmt{StatementType::PRINT, 4}};
        pfw.setStmts(stmts);

        Synonym* stmtSyn = new Synonym(DesignEntityType::STMT, "s");
        // Select s such that Uses(s, "x")
        UsesTester{pfr, stmtSyn, new Literal("x")}.testSynonyms({*stmtSyn}).testSynonymValues({{"1"}});
        // Select s such that Uses(s, "y")
        UsesTester{pfr, stmtSyn, new Literal("y")}.testSynonyms({*stmtSyn}).testSynonymValues({{"2", "3"}});

        // Select s such that Uses(s, "z"), non-existent
        UsesTester{pfr, stmtSyn, new Literal("z")}.testSynonyms({*stmtSyn}).testSynonymValues({{}});

        Synonym* readStmtSyn = new Synonym(DesignEntityType::READ, "s1");
        // Select s such that Uses(s, "x")
        UsesTester{pfr, readStmtSyn, new Literal("x")}.testSynonyms({*readStmtSyn}).testSynonymValues({{"1"}});
        // Select s such that Uses(s, "y") -- should not match modifier#2 since it is not correct type
        UsesTester{pfr, readStmtSyn, new Literal("y")}.testSynonyms({*readStmtSyn}).testSynonymValues({{"3"}});
    }

    SECTION("Uses(Integer, Synonym)") {
        std::unordered_set<std::pair<int, std::string>> usesStoreEntries{std::pair<int, std::string>{1, "x"},
                                                                         std::pair<int, std::string>{1, "y"},
                                                                         std::pair<int, std::string>{2, "y"}};
        pfw.setStatementUsesStore(usesStoreEntries);
        Synonym* stmtSyn = new Synonym(DesignEntityType::VARIABLE, "s");

        // Select s such that Uses(1, s)
        UsesTester{pfr, new Integer("1"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{"x", "y"}});
        // Select s such that Uses(2, s)
        UsesTester{pfr, new Integer("2"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{"y"}});
        // Select s such that Uses(3, s)
        UsesTester{pfr, new Integer("3"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{}});

        // Select s such that Uses(5, s), out of bounds
        UsesTester{pfr, new Integer("5"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{}});
    }

    SECTION("Uses(Synonym, Wildcard)") {
        std::unordered_set<std::pair<int, std::string>> usesStoreEntries{std::pair<int, std::string>{1, "x"},
                                                                         std::pair<int, std::string>{2, "y"},
                                                                         std::pair<int, std::string>{3, "y"}};
        pfw.setStatementUsesStore(usesStoreEntries);

        std::unordered_set<Stmt> stmts = {Stmt{StatementType::READ, 1}, Stmt{StatementType::ASSIGN, 2},
                                          Stmt{StatementType::READ, 3}, Stmt{StatementType::PRINT, 4}};
        pfw.setStmts(stmts);

        std::unordered_set<Variable> variables = {"x", "y", "z"};
        pfw.setVariables(variables);

        Synonym* stmtSyn = new Synonym(DesignEntityType::STMT, "s");
        // Select s such that Uses(s, _)
        UsesTester{pfr, stmtSyn, new Wildcard()}.testSynonyms({*stmtSyn}).testSynonymValues({{"1", "2", "3"}});

        Synonym* readStmtSyn = new Synonym(DesignEntityType::ASSIGN, "s1");
        // Select s such that Uses(s, _)
        UsesTester{pfr, readStmtSyn, new Wildcard()}.testSynonyms({*readStmtSyn}).testSynonymValues({{"2"}});
    }
}

TEST_CASE("SuchThatClause evaluate for Uses relationship with 2 synonyms") {
    PKB pkb{};
    PKBFacadeWriter pfw{pkb};
    PKBFacadeReader pfr{pkb};

    SECTION("Uses(Synonym, Synonym)") {
        std::unordered_set<std::pair<int, std::string>> usesStoreEntries{
            std::pair<int, std::string>{1, "x"}, std::pair<int, std::string>{1, "y"},
            std::pair<int, std::string>{2, "x"}, std::pair<int, std::string>{3, "y"}};

        std::unordered_set<Stmt> stmts = {Stmt{StatementType::ASSIGN, 1}, Stmt{StatementType::READ, 2},
                                          Stmt{StatementType::ASSIGN, 3}, Stmt{StatementType::ASSIGN, 4}};

        std::unordered_set<Variable> variables = {"x", "y", "z"};
        pfw.setVariables(variables);

        pfw.setStmts(stmts);
        pfw.setStatementUsesStore(usesStoreEntries);
        Synonym* stmtSyn = new Synonym(DesignEntityType::STMT, "s1");
        Synonym* varSyn = new Synonym(DesignEntityType::VARIABLE, "v");

        // Select s1 such that Uses(s1, v)
        UsesTester{pfr, stmtSyn, varSyn}
            .testSynonyms({*stmtSyn, *varSyn})
            .testSynonymValues({{"1", "1", "2", "3"}, {"x", "y", "x", "y"}});

        Synonym* readStmtSyn = new Synonym(DesignEntityType::READ, "s2");
        // Select s2 such that Uses(s2, v) -- should only retrieve read statements.
        UsesTester{pfr, readStmtSyn, varSyn}.testSynonyms({*readStmtSyn, *varSyn}).testSynonymValues({{"2"}, {"x"}});
    }
}
