#include "PKB/PKBClient/PKBFacadeWriter.h"
#include "TesterHelper.h"
#include "catch.hpp"
#include "qps/clauseArguments/Literal.h"
#include "qps/clauseArguments/Wildcard.h"
#include "qps/clauses/SuchThatClause.h"

TEST_CASE("SuchThatClause evaluate for Modifies relationship with no synonyms") {
    PKB pkb{};
    PKBFacadeWriter pfw{pkb};
    PKBFacadeReader pfr{pkb};

    SECTION("Modifies(Integer, Variable)") {
        std::unordered_set<std::pair<int, std::string>> modifiesStoreEntries{std::pair<int, std::string>{1, "x"}};
        pfw.setStatementModifiesStore(modifiesStoreEntries);
        ModifiesTester{pfr, new Integer("1"), new Literal("x")}.testBoolean(true);
        ModifiesTester{pfr, new Integer("2"), new Literal("x")}.testBoolean(false);
        ModifiesTester{pfr, new Integer("1"), new Literal("y")}.testBoolean(false);
    }

    SECTION("Modifies(Integer, Variable) / empty store") {
        pfw.setStatementModifiesStore({});
        ModifiesTester{pfr, new Integer("1"), new Literal("x")}.testBoolean(false);
    }

    SECTION("Modifies(Integer, Wildcard)") {
        std::unordered_set<std::pair<int, std::string>> modifiesStoreEntries{std::pair<int, std::string>{1, "x"},
                                                                             std::pair<int, std::string>{2, "y"}};
        pfw.setStatementModifiesStore(modifiesStoreEntries);
        ModifiesTester{pfr, new Integer("1"), new Wildcard()}.testBoolean(true);
        ModifiesTester{pfr, new Integer("2"), new Wildcard()}.testBoolean(true);
        ModifiesTester{pfr, new Integer("3"), new Wildcard()}.testBoolean(false);
    }

    SECTION("Modifies(Integer, Wildcard) / empty store") {
        pfw.setStatementModifiesStore({});
        ModifiesTester{pfr, new Integer("1"), new Wildcard()}.testBoolean(false);
        ModifiesTester{pfr, new Wildcard(), new Wildcard()}.testBoolean(false);
    }

    SECTION("Modifies(Wildcard, Variable)") {
        std::unordered_set<std::pair<int, std::string>> modifiesStoreEntries{std::pair<int, std::string>{1, "x"},
                                                                             std::pair<int, std::string>{2, "y"}};
        pfw.setStatementModifiesStore(modifiesStoreEntries);
        ModifiesTester{pfr, new Wildcard(), new Literal("x")}.testBoolean(true);
        ModifiesTester{pfr, new Wildcard(), new Literal("y")}.testBoolean(true);
        ModifiesTester{pfr, new Wildcard(), new Literal("z")}.testBoolean(false);
    }

    SECTION("Modifies(Wildcard, Wildcard)") {
        std::unordered_set<std::pair<int, std::string>> modifiesStoreEntries{std::pair<int, std::string>{1, "x"},
                                                                             std::pair<int, std::string>{2, "y"}};
        pfw.setStatementModifiesStore(modifiesStoreEntries);
        ModifiesTester{pfr, new Wildcard(), new Wildcard()}.testBoolean(true);
    }

    SECTION("Modifies(Wildcard, Wildcard) / empty store") {
        pfw.setStatementModifiesStore({});
        ModifiesTester{pfr, new Wildcard(), new Wildcard()}.testBoolean(false);
    }
}

TEST_CASE("SuchThatClause evaluate for Modifies relationship with 1 synonym") {
    PKB pkb{};
    PKBFacadeWriter pfw{pkb};
    PKBFacadeReader pfr{pkb};

    SECTION("Modifies(Synonym, Integer)") {
        std::unordered_set<std::pair<int, std::string>> modifiesStoreEntries{std::pair<int, std::string>{1, "x"},
                                                                             std::pair<int, std::string>{2, "y"},
                                                                             std::pair<int, std::string>{3, "y"}};
        pfw.setStatementModifiesStore(modifiesStoreEntries);

        std::unordered_set<Stmt> stmts = {Stmt{StatementType::READ, 1}, Stmt{StatementType::ASSIGN, 2},
                                          Stmt{StatementType::READ, 3}, Stmt{StatementType::PRINT, 4}};
        pfw.setStmts(stmts);

        Synonym* stmtSyn = new Synonym(DesignEntityType::STMT, "s");
        // Select s such that Modifies(s, "x")
        ModifiesTester{pfr, stmtSyn, new Literal("x")}.testSynonyms({*stmtSyn}).testSynonymValues({{"1"}});
        // Select s such that Modifies(s, "y")
        ModifiesTester{pfr, stmtSyn, new Literal("y")}.testSynonyms({*stmtSyn}).testSynonymValues({{"2", "3"}});

        // Select s such that Modifies(s, "z"), non-existent
        ModifiesTester{pfr, stmtSyn, new Literal("z")}.testSynonyms({*stmtSyn}).testSynonymValues({{}});

        Synonym* readStmtSyn = new Synonym(DesignEntityType::READ, "s1");
        // Select s such that Modifies(s, "x")
        ModifiesTester{pfr, readStmtSyn, new Literal("x")}.testSynonyms({*readStmtSyn}).testSynonymValues({{"1"}});
        // Select s such that Modifies(s, "y") -- should not match modifier#2 since it is not correct type
        ModifiesTester{pfr, readStmtSyn, new Literal("y")}.testSynonyms({*readStmtSyn}).testSynonymValues({{"3"}});
    }

    SECTION("Modifies(Integer, Synonym)") {
        std::unordered_set<std::pair<int, std::string>> modifiesStoreEntries{std::pair<int, std::string>{1, "x"},
                                                                             std::pair<int, std::string>{1, "y"},
                                                                             std::pair<int, std::string>{2, "y"}};
        pfw.setStatementModifiesStore(modifiesStoreEntries);
        Synonym* stmtSyn = new Synonym(DesignEntityType::VARIABLE, "s");

        // Select s such that Modifies(1, s)
        ModifiesTester{pfr, new Integer("1"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{"x", "y"}});
        // Select s such that Modifies(2, s)
        ModifiesTester{pfr, new Integer("2"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{"y"}});
        // Select s such that Modifies(3, s)
        ModifiesTester{pfr, new Integer("3"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{}});

        // Select s such that Modifies(5, s), out of bounds
        ModifiesTester{pfr, new Integer("5"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{}});
    }

    SECTION("Modifies(Synonym, Wildcard)") {
        std::unordered_set<std::pair<int, std::string>> modifiesStoreEntries{std::pair<int, std::string>{1, "x"},
                                                                             std::pair<int, std::string>{2, "y"},
                                                                             std::pair<int, std::string>{3, "y"}};
        pfw.setStatementModifiesStore(modifiesStoreEntries);

        std::unordered_set<Stmt> stmts = {Stmt{StatementType::READ, 1}, Stmt{StatementType::ASSIGN, 2},
                                          Stmt{StatementType::READ, 3}, Stmt{StatementType::PRINT, 4}};
        pfw.setStmts(stmts);

        std::unordered_set<Variable> variables = {"x", "y", "z"};
        pfw.setVariables(variables);

        Synonym* stmtSyn = new Synonym(DesignEntityType::STMT, "s");
        // Select s such that Modifies(s, _)
        ModifiesTester{pfr, stmtSyn, new Wildcard()}.testSynonyms({*stmtSyn}).testSynonymValues({{"1", "2", "3"}});

        Synonym* readStmtSyn = new Synonym(DesignEntityType::ASSIGN, "s1");
        // Select s such that Modifies(s, _)
        ModifiesTester{pfr, readStmtSyn, new Wildcard()}.testSynonyms({*readStmtSyn}).testSynonymValues({{"2"}});
    }

    SECTION("Modifies(Wildcard, Synonym)") {
        std::unordered_set<Stmt> stmts = {Stmt{StatementType::ASSIGN, 1}, Stmt{StatementType::ASSIGN, 2},
                                          Stmt{StatementType::ASSIGN, 3}};

        std::unordered_set<std::pair<int, std::string>> modifiesStoreEntries{std::pair<int, std::string>{1, "x"},
                                                                             std::pair<int, std::string>{2, "y"}};

        std::unordered_set<Variable> variables = {"x", "y", "z"};
        pfw.setVariables(variables);

        pfw.setStmts(stmts);
        pfw.setStatementModifiesStore(modifiesStoreEntries);
        Synonym* stmtSyn = new Synonym(DesignEntityType::VARIABLE, "s");

        // Select s such that Modifies(_, s)
        ModifiesTester{pfr, new Wildcard(), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{"x", "y"}});
    }
}

TEST_CASE("SuchThatClause evaluate for Modifies relationship with 2 synonyms") {
    PKB pkb{};
    PKBFacadeWriter pfw{pkb};
    PKBFacadeReader pfr{pkb};

    SECTION("Modifies(Synonym, Synonym)") {
        std::unordered_set<std::pair<int, std::string>> modifiesStoreEntries{
            std::pair<int, std::string>{1, "x"}, std::pair<int, std::string>{1, "y"},
            std::pair<int, std::string>{2, "x"}, std::pair<int, std::string>{3, "y"}};

        std::unordered_set<Stmt> stmts = {Stmt{StatementType::ASSIGN, 1}, Stmt{StatementType::READ, 2},
                                          Stmt{StatementType::ASSIGN, 3}, Stmt{StatementType::ASSIGN, 4}};

        std::unordered_set<Variable> variables = {"x", "y", "z"};
        pfw.setVariables(variables);

        pfw.setStmts(stmts);
        pfw.setStatementModifiesStore(modifiesStoreEntries);
        Synonym* stmtSyn = new Synonym(DesignEntityType::STMT, "s1");
        Synonym* varSyn = new Synonym(DesignEntityType::VARIABLE, "v");

        // Select s1 such that Modifies(s1, v)
        ModifiesTester{pfr, stmtSyn, varSyn}
            .testSynonyms({*stmtSyn, *varSyn})
            .testSynonymValues({{"1", "1", "2", "3"}, {"x", "y", "x", "y"}});

        Synonym* readStmtSyn = new Synonym(DesignEntityType::READ, "s2");
        // Select s2 such that Modifies(s2, v) -- should only retrieve read statements.
        ModifiesTester{pfr, readStmtSyn, varSyn}
            .testSynonyms({*readStmtSyn, *varSyn})
            .testSynonymValues({{"2"}, {"x"}});
    }
}
