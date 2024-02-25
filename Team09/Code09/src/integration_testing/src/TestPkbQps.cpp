#include "PKB/PKBClient/PKBFacadeReader.h"
#include "PKB/PKBClient/PKBFacadeWriter.h"
#include "catch.hpp"
#include "pkb/PKB.h"
#include "qps/clauseArguments/Wildcard.h"
#include "qps/clauses/SuchThatClause.h"

class SuchThatTester {
private:  // NOLINT
    PKBFacadeReader pkb;
    ClauseArgument* firstArg;
    ClauseArgument* secondArg;
    ClauseResult result;

public:  // NOLINT
    SuchThatTester(PKBFacadeReader pkb, ClauseArgument* firstArg, ClauseArgument* secondArg, RelationshipType rsType)
        : pkb{pkb},
          firstArg{firstArg},
          secondArg{secondArg},
          result{SuchThatClause(rsType, firstArg, secondArg).evaluate(pkb)} {}

    void testBoolean(bool expected) {
        REQUIRE(result.isBoolean());
        REQUIRE(result.getBoolean() == expected);
    }

    SuchThatTester testSynonyms(std::vector<Synonym> expectedSynonyms) {
        REQUIRE(!result.isBoolean());
        REQUIRE(result.getSynonyms() == expectedSynonyms);
        return *this;
    }

    SuchThatTester testSynonymValues(std::vector<SynonymValues> expectedValues) {
        REQUIRE(!result.isBoolean());

        auto allSynonymValues = result.getAllSynonymValues();

        REQUIRE(allSynonymValues.size() == expectedValues.size());

        if (allSynonymValues.size() == 0) {
            return *this;
        }

        std::unordered_set<std::string> entries{};
        for (int i = 0; i < allSynonymValues[0].size(); i++) {
            std::string entry = "";
            for (int j = 0; j < allSynonymValues.size(); j++) {
                entry += allSynonymValues[j][i] + ",";
            }
            auto it = entries.find(entry);
            if (it == entries.end()) {
                entries.insert(entry);
            } else {
                FAIL("Duplicate entry found in synonym values: " + entry);
            }
        }

        for (int i = 0; i < expectedValues[0].size(); i++) {
            std::string entry = "";
            for (int j = 0; j < expectedValues.size(); j++) {
                entry += expectedValues[j][i] + ",";
            }
            auto it = entries.find(entry);
            if (it == entries.end()) {
                FAIL("Missing entry in synonym values: " + entry);
            }
        }

        return *this;
    }
};

class FollowsTester : public SuchThatTester {
public:  // NOLINT
    FollowsTester(PKBFacadeReader pkb, ClauseArgument* firstArg, ClauseArgument* secondArg)
        : SuchThatTester(pkb, firstArg, secondArg, RelationshipType::FOLLOWS) {}
};

class FollowsStarTester : public SuchThatTester {
public:  // NOLINT
    FollowsStarTester(PKBFacadeReader pkb, ClauseArgument* firstArg, ClauseArgument* secondArg)
        : SuchThatTester(pkb, firstArg, secondArg, RelationshipType::FOLLOWS_STAR) {}
};

class ParentTester : public SuchThatTester {
public:  // NOLINT
    ParentTester(PKBFacadeReader pkb, ClauseArgument* firstArg, ClauseArgument* secondArg)
        : SuchThatTester(pkb, firstArg, secondArg, RelationshipType::PARENT) {}
};

class ParentStarTester : public SuchThatTester {
public:  // NOLINT
    ParentStarTester(PKBFacadeReader pkb, ClauseArgument* firstArg, ClauseArgument* secondArg)
        : SuchThatTester(pkb, firstArg, secondArg, RelationshipType::PARENT_STAR) {}
};

class UsesTester : public SuchThatTester {
public:  // NOLINT
    UsesTester(PKBFacadeReader pkb, ClauseArgument* firstArg, ClauseArgument* secondArg)
        : SuchThatTester(pkb, firstArg, secondArg, RelationshipType::USES) {}
};

class ModifiesTester : public SuchThatTester {
public:  // NOLINT
    ModifiesTester(PKBFacadeReader pkb, ClauseArgument* firstArg, ClauseArgument* secondArg)
        : SuchThatTester(pkb, firstArg, secondArg, RelationshipType::MODIFIES) {}
};

PKBFacadeReader buildPKB(PKB pkb) {
    PKBFacadeWriter pfw{pkb};

    //    procedure main {
    //  1   num1 = 1;
    //  2   read num2
    //  3   if (num1 == num2) then {
    //  4     print num1;
    //      } else {
    //  5     num2 = num1 + 4;
    //      }
    //  6   call next;
    //    }
    //
    //    procedure next {
    //  7   while (num1 < num2) {
    //  8     num2 = num2 - 1;
    //  9     if (num2 == 2) then {
    // 10       print num2;
    //        } else {
    // 11       print "0";
    //        }
    //      }
    // 12   print num2;
    //    }

    const std::unordered_set<Stmt> stmts = {
        {StatementType::ASSIGN, 1}, {StatementType::READ, 2},   {StatementType::IF, 3},     {StatementType::PRINT, 4},
        {StatementType::ASSIGN, 5}, {StatementType::CALL, 6},   {StatementType::WHILE, 7},  {StatementType::ASSIGN, 8},
        {StatementType::IF, 9},     {StatementType::PRINT, 10}, {StatementType::PRINT, 11}, {StatementType::PRINT, 12}};

    const std::unordered_set<Variable> vars = {"num1", "num2", "num3"};
    const std::unordered_set<Constant> consts = {"4", "1", "0"};
    const std::unordered_set<Procedure> procs = {"main", "next"};
    const std::unordered_set<std::pair<int, int>> followsStoreEntries = {{1, 2}, {2, 3}, {3, 6}, {7, 12}, {8, 9}};
    const std::unordered_set<std::pair<int, int>> parentStoreEntries = {{3, 4},  {3, 5},  {7, 8}, {7, 9},
                                                                        {7, 12}, {9, 10}, {9, 11}};

    const std::unordered_set<std::pair<StmtNum, Variable>> modifiesStoreEntries = {
        {1, "num1"}, {2, "num2"}, {3, "num2"}, {5, "num2"}, {8, "num2"}, {7, "num2"}};
    const std::unordered_set<std::pair<StmtNum, Variable>> usesStoreEntries = {
        {3, "num1"}, {3, "num2"}, {4, "num1"}, {5, "num1"},  {7, "num1"},
        {7, "num2"}, {8, "num2"}, {9, "num2"}, {10, "num2"}, {12, "num2"}};

    pfw.setStmts(stmts);
    pfw.setVariables(vars);
    pfw.setConstants(consts);
    pfw.setProcedures(procs);
    pfw.setFollowsStore(followsStoreEntries);
    pfw.setParentStore(parentStoreEntries);
    pfw.setModifiesStore(modifiesStoreEntries);
    pfw.setUsesStore(usesStoreEntries);

    return PKBFacadeReader{pkb};
}

TEST_CASE("SuchThatClause evaluate for Uses relationship") {
    PKB pkb{};
    PKBFacadeReader pfr{buildPKB(pkb)};

    SECTION("No synonyms") {
        SECTION("Uses(Integer, Variable)") {
            UsesTester{pfr, new Integer("3"), new Literal("num1")}.testBoolean(true);
            UsesTester{pfr, new Integer("1"), new Literal("num1")}.testBoolean(false);
            UsesTester{pfr, new Integer("3"), new Literal("y")}.testBoolean(false);
        }

        SECTION("Uses(Integer, Wildcard)") {
            UsesTester{pfr, new Integer("3"), new Wildcard()}.testBoolean(true);
            UsesTester{pfr, new Integer("1"), new Wildcard()}.testBoolean(false);
            UsesTester{pfr, new Integer("50"), new Wildcard()}.testBoolean(false);
        }

        SECTION("Uses(Wildcard, Variable)") {
            UsesTester{pfr, new Wildcard(), new Literal("num1")}.testBoolean(true);
            UsesTester{pfr, new Wildcard(), new Literal("num2")}.testBoolean(true);
            UsesTester{pfr, new Wildcard(), new Literal("num3")}.testBoolean(false);
        }

        SECTION("Uses(Wildcard, Wildcard)") {
            UsesTester{pfr, new Wildcard(), new Wildcard()}.testBoolean(true);
        }
    }

    SECTION("1 synonym") {
        SECTION("Uses(Synonym, Integer)") {
            Synonym* stmtSyn = new Synonym(DesignEntityType::STMT, "s");
            UsesTester{pfr, stmtSyn, new Literal("num1")}
                .testSynonyms({*stmtSyn})
                .testSynonymValues({{"3", "4", "5", "7"}});
            UsesTester{pfr, stmtSyn, new Literal("z")}.testSynonyms({*stmtSyn}).testSynonymValues({{}});

            Synonym* readStmtSyn = new Synonym(DesignEntityType::PRINT, "s1");
            UsesTester{pfr, readStmtSyn, new Literal("num1")}.testSynonyms({*readStmtSyn}).testSynonymValues({{"4"}});
            UsesTester{pfr, readStmtSyn, new Literal("y")}.testSynonyms({*readStmtSyn}).testSynonymValues({{}});
        }

        SECTION("Uses(Integer, Synonym)") {
            Synonym* stmtSyn = new Synonym(DesignEntityType::VARIABLE, "s");

            UsesTester{pfr, new Integer("7"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{"num1", "num2"}});
            UsesTester{pfr, new Integer("2"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{}});
        }

        SECTION("Uses(Synonym, Wildcard)") {
            Synonym* stmtSyn = new Synonym(DesignEntityType::STMT, "s");
            UsesTester{pfr, stmtSyn, new Wildcard()}
                .testSynonyms({*stmtSyn})
                .testSynonymValues({{"3", "4", "5", "7", "8", "9", "10", "12"}});

            Synonym* readStmtSyn = new Synonym(DesignEntityType::ASSIGN, "s1");
            UsesTester{pfr, readStmtSyn, new Wildcard()}.testSynonyms({*readStmtSyn}).testSynonymValues({{"5", "8"}});
        }

        SECTION("Uses(Wildcard, Synonym)") {
            Synonym* stmtSyn = new Synonym(DesignEntityType::VARIABLE, "s");

            UsesTester{pfr, new Wildcard(), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{"num1", "num2"}});
        }
    }

    SECTION("2 synonyms") {
        Synonym* stmtSyn = new Synonym(DesignEntityType::STMT, "s1");
        Synonym* varSyn = new Synonym(DesignEntityType::VARIABLE, "v");

        UsesTester{pfr, stmtSyn, varSyn}
            .testSynonyms({*stmtSyn, *varSyn})
            .testSynonymValues({{"3", "3", "4", "5", "7", "7", "8", "9", "10", "12"},
                                {"num1", "num2", "num1", "num1", "num1", "num2", "num2", "num2", "num2", "num2"}});

        Synonym* readStmtSyn = new Synonym(DesignEntityType::PRINT, "s2");
        UsesTester{pfr, readStmtSyn, varSyn}
            .testSynonyms({*readStmtSyn, *varSyn})
            .testSynonymValues({{"4", "10", "12"}, {"num1", "num2", "num2"}});
    }
}

TEST_CASE("Parent* relationship with no synonyms") {
    PKB pkb{};
    PKBFacadeReader pfr{buildPKB(pkb)};

    SECTION("No synonyms") {
        SECTION("ParentT(Integer, Integer)") {
            ParentStarTester{pfr, new Integer("7"), new Integer("12")}.testBoolean(true);
            ParentStarTester{pfr, new Integer("2"), new Integer("1")}.testBoolean(false);
            ParentStarTester{pfr, new Integer("50"), new Integer("51")}.testBoolean(false);
        }

        SECTION("ParentT(Integer, Wildcard)") {
            ParentStarTester{pfr, new Integer("7"), new Wildcard()}.testBoolean(true);
            ParentStarTester{pfr, new Integer("2"), new Wildcard()}.testBoolean(false);
            ParentStarTester{pfr, new Integer("50"), new Wildcard()}.testBoolean(false);
        }

        SECTION("ParentT(Wildcard, Integer)") {
            ParentStarTester{pfr, new Wildcard(), new Integer("12")}.testBoolean(true);
            ParentStarTester{pfr, new Wildcard(), new Integer("3")}.testBoolean(false);
            ParentStarTester{pfr, new Wildcard(), new Integer("50")}.testBoolean(false);
        }

        SECTION("ParentT(Wildcard, Wildcard)") {
            ParentStarTester{pfr, new Wildcard(), new Wildcard()}.testBoolean(true);
        }
    }

    SECTION("1 synonym") {
        SECTION("ParentT(Synonym, Integer)") {
            Synonym* stmtSyn = new Synonym(DesignEntityType::STMT, "s");

            ParentStarTester{pfr, stmtSyn, new Integer("10")}.testSynonyms({*stmtSyn}).testSynonymValues({{"7"}});
            ParentStarTester{pfr, stmtSyn, new Integer("1")}.testSynonyms({*stmtSyn}).testSynonymValues({{}});
            ParentStarTester{pfr, stmtSyn, new Integer("50")}.testSynonyms({*stmtSyn}).testSynonymValues({{}});
        }

        SECTION("ParentT(Integer, Synonym)") {
            Synonym* stmtSyn = new Synonym(DesignEntityType::STMT, "s");

            ParentStarTester{pfr, new Integer("7"), stmtSyn}
                .testSynonyms({*stmtSyn})
                .testSynonymValues({{"8", "9", "10", "11", "12"}});
            ParentStarTester{pfr, new Integer("2"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{}});
            ParentStarTester{pfr, new Integer("50"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{}});
        }

        SECTION("ParentT(Synonym, Wildcard)") {
            Synonym* stmtSyn = new Synonym(DesignEntityType::STMT, "s");

            ParentStarTester{pfr, stmtSyn, new Wildcard()}
                .testSynonyms({*stmtSyn})
                .testSynonymValues({{"3", "7", "9"}});
        }

        SECTION("ParentT(Wildcard, Synonym)") {
            Synonym* stmtSyn = new Synonym(DesignEntityType::STMT, "s");

            ParentStarTester{pfr, new Wildcard(), stmtSyn}
                .testSynonyms({*stmtSyn})
                .testSynonymValues({{"4", "5", "8", "9", "12", "10", "11"}});
        }
    }

    SECTION("2 Synonyms") {
        Synonym* stmtSyn1 = new Synonym(DesignEntityType::STMT, "s1");
        Synonym* stmtSyn2 = new Synonym(DesignEntityType::STMT, "s2");

        ParentStarTester{pfr, stmtSyn1, stmtSyn2}
            .testSynonyms({*stmtSyn1, *stmtSyn2})
            .testSynonymValues({{"3", "3", "7", "7", "7", "9", "9"}, {"4", "5", "8", "9", "12", "10", "11"}});
    }
}

TEST_CASE("SuchThatClause evaluate for parent relationship") {
    PKB pkb{};
    PKBFacadeReader pfr{buildPKB(pkb)};

    SECTION("No synonyms") {
        SECTION("Parent(Integer, Integer)") {
            ParentTester{pfr, new Integer("7"), new Integer("9")}.testBoolean(true);
            ParentTester{pfr, new Integer("7"), new Integer("10")}.testBoolean(false);
            ParentTester{pfr, new Integer("1"), new Integer("1")}.testBoolean(false);
            ParentTester{pfr, new Integer("10"), new Integer("11")}.testBoolean(false);
        }

        SECTION("Parent(Integer, Wildcard)") {
            ParentTester{pfr, new Integer("3"), new Wildcard()}.testBoolean(true);
            ParentTester{pfr, new Integer("1"), new Wildcard()}.testBoolean(false);
            ParentTester{pfr, new Integer("50"), new Wildcard()}.testBoolean(false);
        }

        SECTION("Parent(Wildcard, Integer)") {
            ParentTester{pfr, new Wildcard(), new Integer("4")}.testBoolean(true);
            ParentTester{pfr, new Wildcard(), new Integer("1")}.testBoolean(false);
            ParentTester{pfr, new Wildcard(), new Integer("50")}.testBoolean(false);
        }

        SECTION("Parent(Wildcard, Wildcard)") {
            ParentTester{pfr, new Wildcard(), new Wildcard()}.testBoolean(true);
        }
    }

    SECTION("1 Synonym") {
        SECTION("Parent(Synonym, Integer)") {
            Synonym* stmtSyn = new Synonym(DesignEntityType::STMT, "s");

            ParentTester{pfr, stmtSyn, new Integer("4")}.testSynonyms({*stmtSyn}).testSynonymValues({{"3"}});
            ParentTester{pfr, stmtSyn, new Integer("10")}.testSynonyms({*stmtSyn}).testSynonymValues({{"9"}});
            ParentTester{pfr, stmtSyn, new Integer("1")}.testSynonyms({*stmtSyn}).testSynonymValues({{}});
            ParentTester{pfr, stmtSyn, new Integer("50")}.testSynonyms({*stmtSyn}).testSynonymValues({{}});
        }

        SECTION("Parent(Integer, Synonym)") {
            Synonym* stmtSyn = new Synonym(DesignEntityType::STMT, "s");

            ParentTester{pfr, new Integer("7"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{"8", "9", "12"}});
            ParentTester{pfr, new Integer("4"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{}});
            ParentTester{pfr, new Integer("50"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{}});
        }

        SECTION("Parent(Synonym, Wildcard)") {
            Synonym* stmtSyn = new Synonym(DesignEntityType::STMT, "s");

            ParentTester{pfr, stmtSyn, new Wildcard()}.testSynonyms({*stmtSyn}).testSynonymValues({{"3", "7", "9"}});
        }

        SECTION("Parent(Wildcard, Synonym)") {
            Synonym* stmtSyn = new Synonym(DesignEntityType::STMT, "s");

            ParentTester{pfr, new Wildcard(), stmtSyn}
                .testSynonyms({*stmtSyn})
                .testSynonymValues({{"4", "5", "8", "9", "12", "10", "11"}});
        }
    }

    SECTION("2 Synonyms") {
        Synonym* stmtSyn1 = new Synonym(DesignEntityType::STMT, "s1");
        Synonym* stmtSyn2 = new Synonym(DesignEntityType::STMT, "s2");

        ParentTester{pfr, stmtSyn1, stmtSyn2}
            .testSynonyms({*stmtSyn1, *stmtSyn2})
            .testSynonymValues({{"3", "3", "7", "7", "7", "9", "9"}, {"4", "5", "8", "9", "12", "10", "11"}});
    }
}

TEST_CASE("SuchThatClause evaluate for Modifies relationship") {
    PKB pkb{};
    PKBFacadeReader pfr{buildPKB(pkb)};

    SECTION("No synonyms") {
        SECTION("Modifies(Integer, Variable)") {
            ModifiesTester{pfr, new Integer("1"), new Literal("num1")}.testBoolean(true);
            ModifiesTester{pfr, new Integer("2"), new Literal("num1")}.testBoolean(false);
            ModifiesTester{pfr, new Integer("1"), new Literal("y")}.testBoolean(false);
        }

        SECTION("Modifies(Integer, Wildcard)") {
            ModifiesTester{pfr, new Integer("1"), new Wildcard()}.testBoolean(true);
            ModifiesTester{pfr, new Integer("4"), new Wildcard()}.testBoolean(false);
            ModifiesTester{pfr, new Integer("50"), new Wildcard()}.testBoolean(false);
        }

        SECTION("Modifies(Wildcard, Variable)") {
            ModifiesTester{pfr, new Wildcard(), new Literal("num1")}.testBoolean(true);
            ModifiesTester{pfr, new Wildcard(), new Literal("num2")}.testBoolean(true);
            ModifiesTester{pfr, new Wildcard(), new Literal("num3")}.testBoolean(false);
        }

        SECTION("Modifies(Wildcard, Wildcard)") {
            ModifiesTester{pfr, new Wildcard(), new Wildcard()}.testBoolean(true);
        }
    }

    SECTION("1 synonym") {
        SECTION("Modifies(Synonym, Variable)") {
            Synonym* stmtSyn = new Synonym(DesignEntityType::STMT, "s");
            ModifiesTester{pfr, stmtSyn, new Literal("num1")}.testSynonyms({*stmtSyn}).testSynonymValues({{"1"}});
            ModifiesTester{pfr, stmtSyn, new Literal("num2")}
                .testSynonyms({*stmtSyn})
                .testSynonymValues({{"2", "3", "5", "8", "7"}});
            ModifiesTester{pfr, stmtSyn, new Literal("z")}.testSynonyms({*stmtSyn}).testSynonymValues({{}});

            Synonym* readStmtSyn = new Synonym(DesignEntityType::READ, "s1");
            ModifiesTester{pfr, readStmtSyn, new Literal("num1")}.testSynonyms({*readStmtSyn}).testSynonymValues({{}});
            ModifiesTester{pfr, readStmtSyn, new Literal("num2")}
                .testSynonyms({*readStmtSyn})
                .testSynonymValues({{"2"}});
        }

        SECTION("Modifies(Integer, Synonym)") {
            Synonym* stmtSyn = new Synonym(DesignEntityType::VARIABLE, "s");
            ModifiesTester{pfr, new Integer("1"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{"num1"}});
            ModifiesTester{pfr, new Integer("2"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{"num2"}});
            ModifiesTester{pfr, new Integer("4"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{}});
            ModifiesTester{pfr, new Integer("50"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{}});
        }

        SECTION("Modifies(Synonym, Wildcard)") {
            Synonym* stmtSyn = new Synonym(DesignEntityType::STMT, "s");
            ModifiesTester{pfr, stmtSyn, new Wildcard()}
                .testSynonyms({*stmtSyn})
                .testSynonymValues({{"1", "2", "3", "5", "7", "8"}});

            Synonym* readStmtSyn = new Synonym(DesignEntityType::ASSIGN, "s1");
            ModifiesTester{pfr, readStmtSyn, new Wildcard()}
                .testSynonyms({*readStmtSyn})
                .testSynonymValues({{"1", "5", "8"}});
        }

        SECTION("Modifies(Wildcard, Synonym)") {
            Synonym* stmtSyn = new Synonym(DesignEntityType::VARIABLE, "s");
            ModifiesTester{pfr, new Wildcard(), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{"num1", "num2"}});
        }
    }

    SECTION("SuchThatClause evaluate for Modifies relationship with 2 synonyms") {
        Synonym* stmtSyn = new Synonym(DesignEntityType::STMT, "s1");
        Synonym* varSyn = new Synonym(DesignEntityType::VARIABLE, "v");

        ModifiesTester{pfr, stmtSyn, varSyn}
            .testSynonyms({*stmtSyn, *varSyn})
            .testSynonymValues({{"1", "2", "3", "5", "7", "8"}, {"num1", "num2", "num2", "num2", "num2", "num2"}});

        Synonym* readStmtSyn = new Synonym(DesignEntityType::READ, "s2");
        ModifiesTester{pfr, readStmtSyn, varSyn}
            .testSynonyms({*readStmtSyn, *varSyn})
            .testSynonymValues({{"2"}, {"num2"}});
    }
}

TEST_CASE("SuchThatClause evaluate for follows* relationship") {
    PKB pkb{};
    PKBFacadeReader pfr{buildPKB(pkb)};

    SECTION("FollowsT with no synonyms") {
        SECTION("FollowsT(Integer, Integer)") {
            FollowsStarTester{pfr, new Integer("1"), new Integer("2")}.testBoolean(true);
            FollowsStarTester{pfr, new Integer("1"), new Integer("6")}.testBoolean(true);
            FollowsStarTester{pfr, new Integer("2"), new Integer("1")}.testBoolean(false);
            FollowsStarTester{pfr, new Integer("1"), new Integer("1")}.testBoolean(false);
            FollowsStarTester{pfr, new Integer("1"), new Integer("8")}.testBoolean(false);
        }

        SECTION("FollowsT(Integer, Wildcard)") {
            FollowsStarTester{pfr, new Integer("1"), new Wildcard()}.testBoolean(true);
            FollowsStarTester{pfr, new Integer("5"), new Wildcard()}.testBoolean(false);
            FollowsStarTester{pfr, new Integer("50"), new Wildcard()}.testBoolean(false);
        }

        SECTION("FollowsT(Wildcard, Integer)") {
            FollowsStarTester{pfr, new Wildcard(), new Integer("2")}.testBoolean(true);
            FollowsStarTester{pfr, new Wildcard(), new Integer("5")}.testBoolean(false);
            FollowsStarTester{pfr, new Wildcard(), new Integer("50")}.testBoolean(false);
        }

        SECTION("FollowsT(Wildcard, Wildcard)") {
            FollowsStarTester{pfr, new Wildcard(), new Wildcard()}.testBoolean(true);
        }
    }

    SECTION("FollowsT with 1 synonym") {
        SECTION("FollowsT(Synonym, Integer)") {
            Synonym* stmtSyn = new Synonym(DesignEntityType::STMT, "s");
            FollowsStarTester{pfr, stmtSyn, new Integer("3")}.testSynonyms({*stmtSyn}).testSynonymValues({{"1", "2"}});
            FollowsStarTester{pfr, stmtSyn, new Integer("12")}.testSynonyms({*stmtSyn}).testSynonymValues({{"7"}});
            FollowsStarTester{pfr, stmtSyn, new Integer("1")}.testSynonyms({*stmtSyn}).testSynonymValues({{}});
            FollowsStarTester{pfr, stmtSyn, new Integer("50")}.testSynonyms({*stmtSyn}).testSynonymValues({{}});
        }

        SECTION("FollowsT(Integer, Synonym)") {
            Synonym* stmtSyn = new Synonym(DesignEntityType::STMT, "s");
            FollowsStarTester{pfr, new Integer("1"), stmtSyn}
                .testSynonyms({*stmtSyn})
                .testSynonymValues({{"2", "3", "6"}});
            FollowsStarTester{pfr, new Integer("7"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{"12"}});
            FollowsStarTester{pfr, new Integer("12"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{}});
            FollowsStarTester{pfr, new Integer("50"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{}});
        }

        SECTION("FollowsT(Synonym, Wildcard)") {
            Synonym* stmtSyn = new Synonym(DesignEntityType::STMT, "s");
            FollowsStarTester{pfr, stmtSyn, new Wildcard()}
                .testSynonyms({*stmtSyn})
                .testSynonymValues({{"1", "2", "3", "7", "8"}});
        }

        SECTION("FollowsT(Wildcard, Synonym)") {
            Synonym* stmtSyn = new Synonym(DesignEntityType::STMT, "s");
            FollowsStarTester{pfr, new Wildcard(), stmtSyn}
                .testSynonyms({*stmtSyn})
                .testSynonymValues({{"2", "3", "6", "12", "9"}});
        }
    }

    SECTION("FollowsT with 2 synonyms") {
        Synonym* stmtSyn1 = new Synonym(DesignEntityType::STMT, "s1");
        Synonym* stmtSyn2 = new Synonym(DesignEntityType::STMT, "s2");
        FollowsStarTester{pfr, stmtSyn1, stmtSyn2}
            .testSynonyms({*stmtSyn1, *stmtSyn2})
            .testSynonymValues({{"1", "1", "1", "2", "2", "3", "7", "8"}, {"2", "3", "6", "3", "6", "6", "12", "9"}});
    }
}

TEST_CASE("SuchThatClause evaluate for follows relationship") {
    PKB pkb{};
    PKBFacadeReader pfr{buildPKB(pkb)};

    SECTION("Follows with no synonyms") {
        SECTION("Follows(Integer, Integer)") {
            FollowsTester{pfr, new Integer("1"), new Integer("2")}.testBoolean(true);
            FollowsTester{pfr, new Integer("3"), new Integer("4")}.testBoolean(false);
            FollowsTester{pfr, new Integer("2"), new Integer("1")}.testBoolean(false);
            FollowsTester{pfr, new Integer("50"), new Integer("61")}.testBoolean(false);
        }

        SECTION("Follows(Integer, Wildcard)") {
            FollowsTester{pfr, new Integer("1"), new Wildcard()}.testBoolean(true);
            FollowsTester{pfr, new Integer("5"), new Wildcard()}.testBoolean(false);
            FollowsTester{pfr, new Integer("50"), new Wildcard()}.testBoolean(false);
        }

        SECTION("Follows(Wildcard, Integer)") {
            FollowsTester{pfr, new Wildcard(), new Integer("2")}.testBoolean(true);
            FollowsTester{pfr, new Wildcard(), new Integer("7")}.testBoolean(false);
            FollowsTester{pfr, new Wildcard(), new Integer("55")}.testBoolean(false);
        }

        SECTION("Follows(Wildcard, Wildcard)") {
            FollowsTester{pfr, new Wildcard(), new Wildcard()}.testBoolean(true);
        }
    }

    SECTION("Follows with 1 synonym") {
        SECTION("Follows(Synonym, Integer)") {
            Synonym* stmtSyn = new Synonym(DesignEntityType::STMT, "s");
            FollowsTester{pfr, stmtSyn, new Integer("2")}.testSynonyms({*stmtSyn}).testSynonymValues({{"1"}});
            FollowsTester{pfr, stmtSyn, new Integer("1")}.testSynonyms({*stmtSyn}).testSynonymValues({{}});
            FollowsTester{pfr, stmtSyn, new Integer("50")}.testSynonyms({*stmtSyn}).testSynonymValues({{}});
        }

        SECTION("Follows(Integer, Synonym)") {
            Synonym* stmtSyn = new Synonym(DesignEntityType::STMT, "s");
            FollowsTester{pfr, new Integer("1"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{"2"}});
            FollowsTester{pfr, new Integer("11"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{}});
            FollowsTester{pfr, new Integer("50"), stmtSyn}.testSynonyms({*stmtSyn}).testSynonymValues({{}});
        }

        SECTION("Follows(Synonym, Wildcard)") {
            Synonym* stmtSyn = new Synonym(DesignEntityType::STMT, "s");
            FollowsTester{pfr, stmtSyn, new Wildcard()}
                .testSynonyms({*stmtSyn})
                .testSynonymValues({{"1", "2", "3", "7", "8"}});
        }

        SECTION("Follows(Wildcard, Synonym)") {
            Synonym* stmtSyn = new Synonym(DesignEntityType::STMT, "s");
            FollowsTester{pfr, new Wildcard(), stmtSyn}
                .testSynonyms({*stmtSyn})
                .testSynonymValues({{"2", "3", "6", "9", "12"}});
        }
    }

    SECTION("Follows with 2 synonyms") {
        Synonym* stmtSyn1 = new Synonym(DesignEntityType::STMT, "s1");
        Synonym* stmtSyn2 = new Synonym(DesignEntityType::STMT, "s2");

        // Select s1 such that Follows(s1, s2)
        FollowsTester{pfr, stmtSyn1, stmtSyn2}
            .testSynonyms({*stmtSyn1, *stmtSyn2})
            .testSynonymValues({{"1", "2", "3", "7", "8"}, {"2", "3", "6", "12", "9"}});
    }
}
