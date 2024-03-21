#include "PKB/PKBClient/PKBFacadeReader.h"
#include "PKB/PKBClient/PKBFacadeWriter.h"
#include "catch.hpp"
#include "pkb/PKB.h"
#include "qps/QPS.h"
#include "sp/Utils.h"

namespace {
// TYPE DEFS
using QPSResult = std::vector<std::string>;

// CUSTOM TESTING MACROS
#define REQUIRE_THROW_SEMANTIC_ERROR(expr) REQUIRE((expr)[0] == "SemanticError")
#define REQUIRE_THROW_SYNTAX_ERROR(expr) REQUIRE((expr)[0] == "SyntaxError")
#define REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected) equalVectorContents(result, expected)
#define REQUIRE_EMPTY(result) REQUIRE((result).empty())
#define REQUIRE_FALSE_RESULT(expr) REQUIRE(expr[0] == "FALSE")
#define REQUIRE_TRUE_RESULT(expr) REQUIRE(expr[0] == "TRUE")

// HELPER FUNCTIONS
void equalVectorContents(QPSResult result, QPSResult expected) {
    std::sort(result.begin(), result.end());
    std::sort(expected.begin(), expected.end());
    REQUIRE(result == expected);
}

// PKB Items
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
// 11       print num1;
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

const std::unordered_set<std::pair<StmtNum, Variable>> stmtModifiesStoreEntries = {
    {1, "num1"}, {2, "num2"}, {3, "num2"}, {5, "num2"}, {6, "num2"}, {8, "num2"}, {7, "num2"}};
const std::unordered_set<std::pair<Procedure, Variable>> procModifiesStoreEntries = {
    {"main", "num1"}, {"main", "num2"}, {"next", "num2"}};

const std::unordered_set<std::pair<StmtNum, Variable>> stmtUsesStoreEntries = {
    {3, "num1"}, {3, "num2"}, {4, "num1"}, {5, "num1"},  {6, "num1"},  {6, "num2"}, {7, "num1"},
    {7, "num2"}, {8, "num2"}, {9, "num2"}, {10, "num2"}, {11, "num1"}, {12, "num2"}};
const std::unordered_set<std::pair<Procedure, Variable>> procUsesStoreEntries = {
    {"main", "num1"}, {"main", "num2"}, {"next", "num1"}, {"next", "num2"}};

const std::unordered_set<std::pair<StmtNum, std::pair<std::string, std::string>>> assignPatternStoreEntries = {
    {1, {"num1 # # ", "1 # # "}}, {5, {"num2 # # ", "add num1 # # 4 # # "}}, {8, {"num2 # # ", "sub num2 # # 1 # # "}}};

const std::unordered_set<std::pair<Procedure, Procedure>> callsStoreEntries = {{"main", "next"}};

const std::unordered_set<std::pair<StmtNum, StmtNum>> nextStoreEntries = {
    {1, 2}, {2, 3}, {3, 4}, {3, 5}, {4, 6}, {5, 6}, {7, 8}, {8, 9}, {9, 10}, {9, 11}, {10, 12}, {11, 12}};

const std::unordered_set<std::pair<Procedure, StmtNum>> callStmtStoreEntries = {{"next", 6}};

// Common results
const QPSResult allStmts = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12"};
const QPSResult allVars = {"num1", "num2", "num3"};
const QPSResult allConsts = {"4", "1", "0"};
const QPSResult allProcs = {"main", "next"};
const QPSResult allFollowers = {"2", "3", "6", "12", "9"};
const QPSResult allFollowees = {"1", "2", "3", "7", "8"};
const QPSResult allParents = {"3", "7", "9"};
const QPSResult allChildren = {"4", "5", "8", "9", "12", "10", "11"};

PKBFacadeReader buildPKBNew(PKB pkb) {
    PKBFacadeWriter pfw{pkb};

    pfw.setStmts(stmts);
    pfw.setVariables(vars);
    pfw.setConstants(consts);
    pfw.setProcedures(procs);
    pfw.setFollowsStore(followsStoreEntries);
    pfw.setParentStore(parentStoreEntries);
    pfw.setStatementModifiesStore(stmtModifiesStoreEntries);
    pfw.setProcedureModifiesStore(procModifiesStoreEntries);
    pfw.setStatementUsesStore(stmtUsesStoreEntries);
    pfw.setProcedureUsesStore(procUsesStoreEntries);
    pfw.setAssignPatternStore(isExactMatch, isPartialMatch, assignPatternStoreEntries);
    pfw.setCallStore(callsStoreEntries);
    pfw.setNextStore(nextStoreEntries);
    pfw.setCallStmtStore(callStmtStoreEntries);

    return PKBFacadeReader{pkb};
}
}  // namespace

TEST_CASE("Only select") {
    PKB pkb{};
    PKBFacadeReader pfr{buildPKBNew(pkb)};
    QPS qps{pfr};

    SECTION("Select statement") {
        QPSResult result = qps.processQueries("stmt s; Select s");
        QPSResult expected = {allStmts};
        REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
    }

    SECTION("Select variable") {
        QPSResult result = qps.processQueries("variable v; Select v");
        QPSResult expected = {allVars};
        REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
    }

    SECTION("Select constants") {
        QPSResult result = qps.processQueries("constant c; Select c");
        QPSResult expected = {allConsts};
        REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
    }

    SECTION("Select procedures") {
        QPSResult result = qps.processQueries("procedure p; Select p");
        QPSResult expected = {allProcs};
        REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
    }

    SECTION("Semantic Errors") {
        SECTION("Missing declaration") {
            REQUIRE_THROW_SEMANTIC_ERROR(qps.processQueries("Select a"));
        }

        SECTION("Incorrect declaration") {
            REQUIRE_THROW_SEMANTIC_ERROR(qps.processQueries("stmt s1; Select a"));
        }
    }

    SECTION("Syntax Errors") {
        SECTION("Missing semicolon") {
            REQUIRE_THROW_SYNTAX_ERROR(qps.processQueries("stmt s Select s"));
        }

        SECTION("Missing semicolon") {
            REQUIRE_THROW_SYNTAX_ERROR(qps.processQueries("stmt s Select s; stmt s2"));
        }
    }
}

/*
TEST_CASE("Boolean Select") {
    PKB pkb{};
    PKBFacadeReader pfr{buildPKBNew(pkb)};
    QPS qps{pfr};

    SECTION("BOOLEAN, Return TRUE, such that") {
        QPSResult result = qps.processQueries("stmt s; Select BOOLEAN such that Follows(1, 2)");
        REQUIRE_TRUE_RESULT(result);
    }

    SECTION("BOOLEAN, Return FALSE, such that") {
        QPSResult result = qps.processQueries("stmt s; Select BOOLEAN such that Follows(12, _)");
        REQUIRE_FALSE_RESULT(result);
    }

    SECTION("BOOLEAN, Return TRUE, pattern") {
        QPSResult result = qps.processQueries("assign a; Select BOOLEAN pattern a(_,_\"1\"_)");
        REQUIRE_TRUE_RESULT(result);
    }

    SECTION("BOOLEAN, Return FALSE, pattern") {
        QPSResult result = qps.processQueries("assign a; variable v; Select BOOLEAN pattern a(v,_\"num3\"_)");
        REQUIRE_FALSE_RESULT(result);
    }
}
*/

TEST_CASE("Select with 1 such-that clause") {
    PKB pkb{};
    PKBFacadeReader pfr{buildPKBNew(pkb)};
    QPS qps{pfr};

    SECTION("Follows") {
        SECTION("No synonyms") {
            SECTION("Follows(Integer, Integer)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Follows(1, 2)");
                QPSResult expected = {allStmts};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Follows(Integer, Wildcard)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Follows(1, _)");
                QPSResult expected = {allStmts};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Follows(Integer, Wildcard) / False") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Follows(12, _)");
                REQUIRE_EMPTY(result);
            }

            SECTION("Follows(Wildcard, Integer)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Follows(_, 2)");
                QPSResult expected = {allStmts};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Follows(Wildcard, Integer) / False") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Follows(_, 1)");
                REQUIRE_EMPTY(result);
            }
        }

        SECTION("1 synonym") {
            SECTION("Follows(Integer, synonym") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Follows(1, s)");
                QPSResult expected = {"2"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Follows(Integer, Synonym: READ)") {
                QPSResult result = qps.processQueries("read r; Select r such that Follows(1, r)");
                QPSResult expected = {"2"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Follows(Integer, Synonym: other types)") {
                QPSResult result = qps.processQueries("assign a; Select a such that Follows(1, a)");
                REQUIRE_EMPTY(result);
            }

            SECTION("Follows(Synonym, Integer)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Follows(s, 12)");
                QPSResult expected = {"7"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Follows(Synonym: WHILE, Integer)") {
                QPSResult result = qps.processQueries("while w; Select w such that Follows(w, 12)");
                QPSResult expected = {"7"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Follows(Synonym: OTHERS, Integer)") {
                QPSResult result = qps.processQueries("assign a; Select a such that Follows(a, 12)");
                REQUIRE_EMPTY(result);
            }

            SECTION("Follows(Synonym, Wildcard)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Follows(s, _)");
                QPSResult expected = {allFollowees};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Follows(Wildcard, Synonym)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Follows(_, s)");
                QPSResult expected = {allFollowers};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }
        }

        SECTION("2 Synonyms") {
            SECTION("Follows(StmtSyn, StmtSyn)") {
                QPSResult result = qps.processQueries("stmt s1, s2; Select s1 such that Follows(s1, s2)");
                QPSResult expected = {allFollowees};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Follows(WhileSyn, StmtSyn)") {
                QPSResult result = qps.processQueries("while w; stmt s; Select w such that Follows(w, s)");
                QPSResult expected = {"7"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Follows(StmtSyn, IfSyn") {
                QPSResult result = qps.processQueries("stmt s; if ifs; Select ifs such that Follows(s, ifs)");
                QPSResult expected = {"3", "9"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }
        }
    }

    SECTION("FollowsStar") {
        SECTION("No synonyms") {
            SECTION("FollowsStar(Integer, Integer)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Follows*(1, 2)");
                QPSResult expected = {allStmts};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("FollowsStar(Integer, Wildcard)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Follows*(1, _)");
                QPSResult expected = {allStmts};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("FollowsStar(Integer, Wildcard) / False") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Follows*(12, _)");
                REQUIRE_EMPTY(result);
            }

            SECTION("FollowsStar(Wildcard, Integer)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Follows*(_, 2)");
                QPSResult expected = {allStmts};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("FollowsStar(Wildcard, Integer) / False") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Follows*(_, 1)");
                REQUIRE_EMPTY(result);
            }
        }

        SECTION("1 synonym") {
            SECTION("FollowsStar(Integer, synonym") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Follows*(1, s)");
                QPSResult expected = {"2", "3", "6"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("FollowsStar(Integer, Synonym: READ)") {
                QPSResult result = qps.processQueries("read r; Select r such that Follows*(1, r)");
                QPSResult expected = {"2"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("FollowsStar(Integer, Synonym: other types)") {
                QPSResult result = qps.processQueries("assign a; Select a such that Follows*(1, a)");
                REQUIRE_EMPTY(result);
            }

            SECTION("FollowsStar(Synonym, Integer)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Follows*(s, 12)");
                QPSResult expected = {"7"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("FollowsStar(Synonym: WHILE, Integer)") {
                QPSResult result = qps.processQueries("while w; Select w such that Follows*(w, 12)");
                QPSResult expected = {"7"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("FollowsStar(Synonym: OTHERS, Integer)") {
                QPSResult result = qps.processQueries("assign a; Select a such that Follows*(a, 12)");
                REQUIRE_EMPTY(result);
            }

            SECTION("FollowsStar(Synonym, Wildcard)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Follows*(s, _)");
                QPSResult expected = {allFollowees};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("FollowsStar(Wildcard, Synonym)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Follows*(_, s)");
                QPSResult expected = {allFollowers};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }
        }

        SECTION("2 synonyms") {
            SECTION("FollowsStar(StmtSyn, StmtSyn)") {
                QPSResult result = qps.processQueries("stmt s1, s2; Select s1 such that Follows*(s1, s2)");
                QPSResult expected = {allFollowees};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("FollowsStar(WhileSyn, StmtSyn)") {
                QPSResult result = qps.processQueries("while w; stmt s; Select w such that Follows*(w, s)");
                QPSResult expected = {"7"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("FollowsStar(StmtSyn, IfSyn") {
                QPSResult result = qps.processQueries("stmt s; if ifs; Select ifs such that Follows*(s, ifs)");
                QPSResult expected = {"3", "9"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }
        }
    }

    // ai-gen start(gpt, 2, e)
    // prompt: https://platform.openai.com/playground/p/F3Qq2w6nZHvWBlW3a5TuHUlH?model=gpt-3.5-turbo&mode=chat
    SECTION("Parent") {
        SECTION("No synonyms") {
            SECTION("Parent(Integer, Integer)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Parent(3, 4)");
                QPSResult expected = {allStmts};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Parent(Integer, Wildcard)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Parent(3, _)");
                QPSResult expected = {allStmts};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Parent(Integer, Wildcard) / False") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Parent(8, _)");
                REQUIRE_EMPTY(result);
            }

            SECTION("Parent(Wildcard, Integer)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Parent(_, 10)");
                QPSResult expected = {allStmts};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Parent(Wildcard, Integer) / False") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Parent(_, 1)");
                REQUIRE_EMPTY(result);
            }
        }

        SECTION("1 synonym") {
            SECTION("Parent(Integer, synonym") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Parent(3, s)");
                QPSResult expected = {"4", "5"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Parent(Integer, Synonym: PRINT") {
                QPSResult result = qps.processQueries("print p; Select p such that Parent(3, p)");
                QPSResult expected = {"4"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Parent(Integer, Synonym: other types)") {
                QPSResult result = qps.processQueries("if ifs; Select ifs such that Parent(3, ifs)");
                REQUIRE_EMPTY(result);
            }

            SECTION("Parent(Synonym, Integer)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Parent(s, 10)");
                QPSResult expected = {"9"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Parent(Synonym: IF, Integer)") {
                QPSResult result = qps.processQueries("if i; Select i such that Parent(i, 10)");
                QPSResult expected = {"9"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Parent(Synonym: OTHERS, Integer)") {
                QPSResult result = qps.processQueries("assign a; Select a such that Parent(a, 10)");
                REQUIRE_EMPTY(result);
            }

            SECTION("Parent(Synonym, Wildcard)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Parent(s, _)");
                QPSResult expected = {allParents};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Parent(Wildcard, Synonym)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Parent(_, s)");
                QPSResult expected = {allChildren};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }
        }

        SECTION("2 synonyms") {
            SECTION("Parent(StmtSyn, StmtSyn)") {
                QPSResult result = qps.processQueries("stmt s1, s2; Select s1 such that Parent(s1, s2)");
                QPSResult expected = {allParents};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Parent(IfSyn, StmtSyn)") {
                QPSResult result = qps.processQueries("if i; stmt s; Select i such that Parent(i, s)");
                QPSResult expected = {"3", "9"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Parent(AssignSyn, StmtSyn)") {
                QPSResult result = qps.processQueries("assign a; stmt s; Select a such that Parent(a, s)");
                REQUIRE_EMPTY(result);
            }
        }
    }
    // ai-gen end

    // ai-gen start(gpt, 2, e)
    // prompt: https://platform.openai.com/playground/p/F3Qq2w6nZHvWBlW3a5TuHUlH?mode=chat&model=gpt-4
    SECTION("ParentStar") {
        SECTION("No synonyms") {
            SECTION("ParentStar(Integer, Integer)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Parent*(3, 5)");
                QPSResult expected = {allStmts};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("ParentStar(Integer, Wildcard)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Parent*(3, _)");
                QPSResult expected = {allStmts};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("ParentStar(Integer, Wildcard) / False") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Parent*(1, _)");
                REQUIRE_EMPTY(result);
            }

            SECTION("ParentStar(Wildcard, Integer)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Parent*(_, 5)");
                QPSResult expected = {allStmts};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("ParentStar(Wildcard, Integer) / False") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Parent*(_, 1)");
                REQUIRE_EMPTY(result);
            }
        }

        SECTION("1 synonym") {
            SECTION("ParentStar(Integer, synonym)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Parent*(3, s)");
                QPSResult expected = {"4", "5"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("ParentStar(Integer, Synonym: ASSIGN)") {
                QPSResult result = qps.processQueries("assign a; Select a such that Parent*(3, a)");
                QPSResult expected = {"5"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("ParentStar(Integer, Synonym: other types)") {
                QPSResult result = qps.processQueries("if ifs; Select ifs such that Parent*(3, ifs)");
                REQUIRE_EMPTY(result);
            }

            SECTION("ParentStar(Synonym, Integer)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Parent*(s, 5)");
                QPSResult expected = {"3"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("ParentStar(Synonym: IF, Integer)") {
                QPSResult result = qps.processQueries("if ifs; Select ifs such that Parent*(ifs, 5)");
                QPSResult expected = {"3"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("ParentStar(Synonym: OTHERS, Integer)") {
                QPSResult result = qps.processQueries("assign a; Select a such that Parent*(a, 5)");
                REQUIRE_EMPTY(result);
            }

            SECTION("ParentStar(Synonym, Wildcard)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Parent*(s, _)");
                QPSResult expected = {allParents};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("ParentStar(Wildcard, Synonym)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Parent*(_, s)");
                QPSResult expected = {allChildren};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }
        }

        SECTION("2 synonyms") {
            SECTION("ParentStar(StmtSyn, StmtSyn)") {
                QPSResult result = qps.processQueries("stmt s1, s2; Select s1 such that Parent*(s1, s2)");
                QPSResult expected = {"3", "7", "9"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("ParentStar(IfSyn, StmtSyn)") {
                QPSResult result = qps.processQueries("if ifs; stmt s; Select ifs such that Parent*(ifs, s)");
                QPSResult expected = {"3", "9"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("ParentStar(StmtSyn, IfSyn)") {
                QPSResult result = qps.processQueries("stmt s; if ifs; Select ifs such that Parent*(s, ifs)");
                QPSResult expected = {"9"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }
        }
    }

    // ai-gen start(gpt-4, 2, e)
    // prompt: https://platform.openai.com/playground/p/HbpIECxH0yuJoveu3X8TOOpU
    SECTION("Modifies") {
        SECTION("No synonyms") {
            SECTION("Modifies(Integer, Variable)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Modifies(1, \"num1\")");
                QPSResult expected = {allStmts};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Modifies(Integer, Wildcard)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Modifies(1, _)");
                QPSResult expected = {allStmts};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Modifies(Integer: [call stmt], Variable)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Modifies(6, \"num2\")");
                QPSResult expected = {allStmts};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Modifies(Literal: [procedure name], Variable)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Modifies(\"main\", \"num1\")");
                QPSResult expected = {allStmts};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Modifies(Wildcard, Variable)") {
                REQUIRE_THROW_SEMANTIC_ERROR(qps.processQueries("stmt s; Select s such that Modifies(_, \"num2\")"));
            }
        }

        QPSResult allModifyingStatements = {"1", "2", "3", "5", "6", "7", "8"};
        SECTION("1 synonym") {
            SECTION("Modifies(Integer, synonym)") {
                QPSResult result = qps.processQueries("variable v; Select v such that Modifies(1, v)");
                QPSResult expected = {"num1"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Modifies(Synonym: Stmt, Variable)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Modifies(s, \"num2\")");
                QPSResult expected = {"2", "3", "5", "6", "7", "8"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Modifies(Synonym, Wildcard)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Modifies(s, _)");
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, allModifyingStatements);
            }

            SECTION("Modifies(Procedure, synonym)") {
                QPSResult result = qps.processQueries("variable v; Select v such that Modifies(\"main\", v)");
                QPSResult expected = {"num1", "num2"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Modifies(Procedure, Variable)") {
                QPSResult result = qps.processQueries("procedure p; Select p such that Modifies(p, \"num2\")");
                QPSResult expected = {"main", "next"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Modifies(Synonym, Wildcard)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Modifies(s, _)");
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, allModifyingStatements);
            }
        }

        SECTION("2 synonyms") {
            SECTION("Modifies(StmtSyn, VariableSyn)") {
                QPSResult result = qps.processQueries("stmt s; variable v; Select s such that Modifies(s, v)");
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, allModifyingStatements);
            }

            SECTION("Modifies(ProcSyn, VariableSyn)") {
                QPSResult result = qps.processQueries("procedure p; variable v; Select p such that Modifies(p, v)");
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, allProcs);
            }

            SECTION("Modifies(AssignSyn, VariableSyn)") {
                QPSResult result = qps.processQueries("assign a; variable v; Select a such that Modifies(a, v)");
                QPSResult expected = {"1", "5", "8"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }
        }
    }
    // ai-gen end

    // ai-gen start(gpt-4, 2, e)
    // prompt: https://platform.openai.com/playground/p/WnxaYJxypfRzOt9jqsY1qk0d?mode=chat
    SECTION("Uses") {
        QPSResult allUsingStatements = {"3", "4", "5", "6", "7", "8", "9", "10", "11", "12"};
        SECTION("No synonyms") {
            SECTION("Uses(Integer, VariableName)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Uses(3, \"num1\")");
                QPSResult expected = {allStmts};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Uses(Wildcard, VariableName)") {
                REQUIRE_THROW_SEMANTIC_ERROR(qps.processQueries("stmt s; Select s such that Uses(_, \"num2\")"));
            }

            SECTION("Uses(Literal: [procedure name], VariableName)") {
                QPSResult result = qps.processQueries("procedure p; Select p such that Uses(\"main\", \"num1\")");
                QPSResult expected = {"main", "next"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Uses(Integer, Wildcard)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Uses(3, _)");
                QPSResult expected = {allStmts};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Uses(Literal: [procedure name], Wildcard)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Uses(\"main\", _)");
                QPSResult expected = {allStmts};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }
        }

        SECTION("1 synonym") {
            SECTION("Uses(Synonym, VariableName)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Uses(s, \"num1\")");
                QPSResult expected = {"3", "4", "5", "6", "7", "11"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Uses(Synonym, Wildcard)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Uses(s, _)");
                // Assuming all statements that either use num1 or num2
                QPSResult expected = {allUsingStatements};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Uses(ProcedureName, Synonym)") {
                QPSResult result = qps.processQueries("variable v; Select v such that Uses(\"main\", v)");
                QPSResult expected = {"num1", "num2"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Uses(Stmt, Synonym)") {
                QPSResult result = qps.processQueries("variable v; Select v such that Uses(3, v)");
                QPSResult expected = {"num1", "num2"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }
        }

        SECTION("2 synonyms") {
            SECTION("Uses(Synonym, Synonym)") {
                QPSResult result = qps.processQueries("stmt s; variable v; Select s such that Uses(s, v)");
                QPSResult expected = {allUsingStatements};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Uses(Synonym: Assign, Synonym)") {
                QPSResult result = qps.processQueries("assign a; variable v; Select a such that Uses(a, v)");
                QPSResult expected = {"5", "8"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Uses(Synonym: Procedure, Synonym)") {
                QPSResult result = qps.processQueries("procedure p; variable v; Select p such that Uses(p, v)");
                QPSResult expected = {allProcs};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }
        }
    }
    // ai-gen end

    SECTION("Calls") {
        SECTION("No synonyms") {
            SECTION("Calls(Procedure, Procedure)") {
                QPSResult result = qps.processQueries("procedure p; Select p such that Calls(\"main\", \"next\")");
                QPSResult expected = {allProcs};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Calls(Procedure, Wildcard)") {
                QPSResult result = qps.processQueries("procedure p; Select p such that Calls(\"main\", _)");
                QPSResult expected = {allProcs};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Calls(Wildcard, Procedure)") {
                QPSResult result = qps.processQueries("procedure p; Select p such that Calls(_, \"next\")");
                QPSResult expected = {allProcs};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Calls(Wildcard, Wildcard)") {
                QPSResult result = qps.processQueries("procedure p; Select p such that Calls(_, _)");
                QPSResult expected = {allProcs};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Calls(Wildcard, Wildcard)") {
                QPSResult result = qps.processQueries("procedure p; Select p such that Calls(\"next\", _)");
                REQUIRE_EMPTY(result);
            }
        }

        SECTION("1 Synonym") {
            SECTION("Calls(Procedure, Synonym)") {
                QPSResult result = qps.processQueries("procedure p; Select p such that Calls(\"main\", p)");
                QPSResult expected = {"next"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Calls(Synonym, Procedure)") {
                QPSResult result = qps.processQueries("procedure p; Select p such that Calls(p, \"next\")");
                QPSResult expected = {"main"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Calls(Synonym, Wildcard)") {
                QPSResult result = qps.processQueries("procedure p; Select p such that Calls(p, _)");
                QPSResult expected = {"main"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Calls(Wildcard, Synonym)") {
                QPSResult result = qps.processQueries("procedure p; Select p such that Calls(_, p)");
                QPSResult expected = {"next"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Calls(Procedure, Synonym: Stmt") {
                REQUIRE_THROW_SEMANTIC_ERROR(qps.processQueries("assign a; Select a such that Calls(\"main\", a)"));
            }

            SECTION("Calls(Synonym: Stmt, Procedure") {
                REQUIRE_THROW_SEMANTIC_ERROR(qps.processQueries("assign a; Select a such that Calls(a, \"next\")"));
            }
        }

        SECTION("2 Synonyms") {
            SECTION("Calls(ProcSyn, ProcSyn)") {
                QPSResult result = qps.processQueries("procedure p1, p2; Select p1 such that Calls(p1, p2)");
                QPSResult expected = {"main"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }
        }
    }

    SECTION("CallsStar") {
        SECTION("No synonyms") {
            SECTION("CallsStar(Procedure, Procedure)") {
                QPSResult result = qps.processQueries("procedure p; Select p such that Calls*(\"main\", \"next\")");
                QPSResult expected = {allProcs};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("CallsStar(Procedure, Wildcard)") {
                QPSResult result = qps.processQueries("procedure p; Select p such that Calls*(\"main\", _)");
                QPSResult expected = {allProcs};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("CallsStar(Wildcard, Procedure)") {
                QPSResult result = qps.processQueries("procedure p; Select p such that Calls*(_, \"next\")");
                QPSResult expected = {allProcs};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("CallsStar(Wildcard, Wildcard)") {
                QPSResult result = qps.processQueries("procedure p; Select p such that Calls*(_, _)");
                QPSResult expected = {allProcs};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("CallsStar(Wildcard, Wildcard)") {
                QPSResult result = qps.processQueries("procedure p; Select p such that Calls*(\"next\", _)");
                REQUIRE_EMPTY(result);
            }
        }

        SECTION("1 Synonym") {
            SECTION("CallsStar(Procedure, Synonym)") {
                QPSResult result = qps.processQueries("procedure p; Select p such that Calls*(\"main\", p)");
                QPSResult expected = {"next"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("CallsStar(Synonym, Procedure)") {
                QPSResult result = qps.processQueries("procedure p; Select p such that Calls*(p, \"next\")");
                QPSResult expected = {"main"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("CallsStar(Synonym, Wildcard)") {
                QPSResult result = qps.processQueries("procedure p; Select p such that Calls*(p, _)");
                QPSResult expected = {"main"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("CallsStar(Wildcard, Synonym)") {
                QPSResult result = qps.processQueries("procedure p; Select p such that Calls*(_, p)");
                QPSResult expected = {"next"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("CallsStar(Procedure, Synonym: Stmt") {
                REQUIRE_THROW_SEMANTIC_ERROR(qps.processQueries("assign a; Select a such that Calls*(\"main\", a)"));
            }

            SECTION("CallsStar(Synonym: Stmt, Procedure") {
                REQUIRE_THROW_SEMANTIC_ERROR(qps.processQueries("assign a; Select a such that Calls*(a, \"next\")"));
            }
        }

        SECTION("2 Synonyms") {
            SECTION("Calls(ProcSyn, ProcSyn)") {
                QPSResult result = qps.processQueries("procedure p1, p2; Select p1 such that Calls(p1, p2)");
                QPSResult expected = {"main"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }
        }
    }

    SECTION("Next") {
        QPSResult allNext = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12"};
        SECTION("No synonyms") {
            SECTION("Next(Stmt, stmt)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Next(1, 2)");
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, allNext);
            }

            SECTION("Next(Stmt, Wildcard)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Next(1, _)");
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, allNext);
            }

            SECTION("Next(Wildcard, Statement)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Next(_, 2)");
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, allNext);
            }

            SECTION("Next(Wildcard, Wildcard") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Next(_, _)");
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, allNext);
            }
        }

        SECTION("1 synonym") {
            SECTION("Next(Stmt, Synonym)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Next(3, s)");
                QPSResult expected = {"4", "5"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Next(Stmt, Synonym: assign)") {
                QPSResult result = qps.processQueries("assign a; Select a such that Next(3, a)");
                QPSResult expected = {"5"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Next(Stmt, Synonym: others)") {
                QPSResult result = qps.processQueries("if ifs; Select ifs such that Next(3, ifs)");
                REQUIRE_EMPTY(result);
            }

            SECTION("Next(Synonym, Stmt)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Next(s, 4)");
                QPSResult expected = {"3"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Next(Synonym: if, Stmt)") {
                QPSResult result = qps.processQueries("if ifs; Select ifs such that Next(ifs, 4)");
                QPSResult expected = {"3"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Next(Synonym: others, Stmt)") {
                QPSResult result = qps.processQueries("assign a; Select a such that Next(a, 4)");
                REQUIRE_EMPTY(result);
            }
        }

        SECTION("2 synonyms") {
            SECTION("Next(StmtSyn, StmtSyn)") {
                QPSResult result = qps.processQueries("stmt s1, s2; Select s1 such that Next(s1, s2)");
                QPSResult expected = {"1", "2", "3", "4", "5", "7", "8", "9", "10", "11"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Next(IfSyn, StmtSyn)") {
                QPSResult result = qps.processQueries("if ifs; stmt s; Select ifs such that Next(ifs, s)");
                QPSResult expected = {"3", "9"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Next(StmtSyn, AssignSyn)") {
                QPSResult result = qps.processQueries("assign a; stmt s; Select a such that Next(s, a)");
                QPSResult expected = {"5", "8"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }
        }
    }

    SECTION("NextStar") {
        QPSResult allNext = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12"};
        SECTION("No synonyms") {
            SECTION("NextStar(Stmt, stmt)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Next*(1, 2)");
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, allNext);
            }

            SECTION("NextStar(Stmt, Wildcard)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Next*(1, _)");
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, allNext);
            }

            SECTION("NextStar(Wildcard, Statement)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Next*(_, 2)");
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, allNext);
            }

            SECTION("NextStar(Wildcard, Wildcard") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Next*(_, _)");
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, allNext);
            }
        }

        SECTION("1 synonym") {
            SECTION("NextStar(Stmt, Synonym)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Next*(3, s)");
                QPSResult expected = {"4", "5", "6"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("NextStar(Stmt, Synonym: assign)") {
                QPSResult result = qps.processQueries("assign a; Select a such that Next*(3, a)");
                QPSResult expected = {"5"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("NextStar(Stmt, Synonym: others)") {
                QPSResult result = qps.processQueries("if ifs; Select ifs such that Next*(3, ifs)");
                REQUIRE_EMPTY(result);
            }

            SECTION("NextStar(Synonym, Stmt)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Next*(s, 4)");
                QPSResult expected = {"1", "2", "3"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("NextStar(Synonym: if, Stmt)") {
                QPSResult result = qps.processQueries("if ifs; Select ifs such that Next*(ifs, 4)");
                QPSResult expected = {"3"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("NextStar(Synonym: others, Stmt)") {
                QPSResult result = qps.processQueries("while w; Select w such that Next*(w, 4)");
                REQUIRE_EMPTY(result);
            }
        }

        SECTION("2 synonyms") {
            SECTION("NextStar(StmtSyn, StmtSyn)") {
                QPSResult result = qps.processQueries("stmt s1, s2; Select s1 such that Next*(s1, s2)");
                QPSResult expected = {"1", "2", "3", "4", "5", "7", "8", "9", "10", "11"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("NextStar(IfSyn, StmtSyn)") {
                QPSResult result = qps.processQueries("if ifs; stmt s; Select ifs such that Next*(ifs, s)");
                QPSResult expected = {"3", "9"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("NextStar(StmtSyn, AssignSyn)") {
                QPSResult result = qps.processQueries("assign a; stmt s; Select a such that Next*(s, a)");
                QPSResult expected = {"5", "8"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }
        }
    }

    SECTION("Pattern") {
        SECTION("Assign(Wildcard, Wildcard) Select a") {
            QPSResult result = qps.processQueries("assign a; Select a pattern a(_,_)");
            QPSResult expected = {"1", "5", "8"};
            REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
        }

        SECTION("Assign(Wildcard, Wildcard) Select v") {
            QPSResult result = qps.processQueries("assign a; variable v; Select v pattern a(_,_)");
            QPSResult expected = {allVars};
            REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
        }

        SECTION("Assign(Variable, Wildcard) Select a") {
            QPSResult result = qps.processQueries("assign a; variable v; Select a pattern a(v,_)");
            QPSResult expected = {"1", "5", "8"};
            REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
        }

        SECTION("Assign(Variable, Wildcard) Select v") {
            QPSResult result = qps.processQueries("assign a; variable v; Select v pattern a(v,_)");
            QPSResult expected = {"num1", "num2"};
            REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
        }

        SECTION("Assign(VarName, Wildcard) Select a") {
            QPSResult result = qps.processQueries("assign a; Select a pattern a(\"num2\",_)");
            QPSResult expected = {"5", "8"};
            REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
        }

        SECTION("Assign(VarName, Wildcard) Select v") {
            QPSResult result = qps.processQueries("assign a; variable v; Select v pattern a(\"num2\",_)");
            QPSResult expected = {allVars};
            REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
        }

        SECTION("Assign(VarName, Wildcard) Select v") {
            QPSResult result = qps.processQueries("assign a; variable v; Select v pattern a(\"num2\",_)");
            QPSResult expected = {allVars};
            REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
        }

        SECTION("Assign(_, Partial Matching) Select a") {
            QPSResult result = qps.processQueries("assign a; Select a pattern a(_,_\"1\"_)");
            QPSResult expected = {"1", "8"};
            REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
        }

        SECTION("Assign(_, Partial Matching) Select v") {
            QPSResult result = qps.processQueries("assign a; variable v; Select v pattern a(_,_\"1\"_)");
            QPSResult expected = {allVars};
            REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
        }

        SECTION("Assign(Variable, Partial Matching) Select a") {
            QPSResult result = qps.processQueries("assign a; variable v; Select a pattern a(v,_\"1\"_)");
            QPSResult expected = {"1", "8"};
            REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
        }

        SECTION("Assign(Variable, Partial Matching) Select a, Empty Result") {
            QPSResult result = qps.processQueries("assign a; variable v; Select a pattern a(v,_\"num3\"_)");
            REQUIRE_EMPTY(result);
        }

        SECTION("Assign(Variable, Partial Matching) Select v") {
            QPSResult result = qps.processQueries("assign a; variable v; Select v pattern a(v,_\"1\"_)");
            QPSResult expected = {"num1", "num2"};
            REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
        }

        SECTION("Assign(VarName, Partial Matching) Select a") {
            QPSResult result = qps.processQueries("assign a; Select a pattern a(\"num1\", _\"1\"_)");
            QPSResult expected = {"1"};
            REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
        }

        SECTION("Assign(VarName, Partial Matching) Select v") {
            QPSResult result = qps.processQueries("assign a; variable v; Select v pattern a(\"num1\", _\"1\"_)");
            QPSResult expected = {allVars};
            REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
        }

        SECTION("Assign(_, Exact Matching) Select a") {
            QPSResult result = qps.processQueries("assign a; Select a pattern a(_, \"1\")");
            QPSResult expected = {"1"};
            REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
        }

        SECTION("Assign(_, Exact Matching) Select v") {
            QPSResult result = qps.processQueries("assign a; variable v; Select v pattern a(_, \"1\")");
            QPSResult expected = {allVars};
            REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
        }

        SECTION("Assign(Variable, Exact Matching) Select a") {
            QPSResult result = qps.processQueries("assign a; variable v; Select a pattern a(v, \"num1+ 4 \")");
            QPSResult expected = {"5"};
            REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
        }

        SECTION("Assign(Variable, Exact Matching) Select v") {
            QPSResult result = qps.processQueries("assign a; variable v; Select v pattern a(v, \"num1+ 4 \")");
            QPSResult expected = {"num2"};
            REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
        }

        SECTION("Assign(VarName, Exact Matching) Select a") {
            QPSResult result = qps.processQueries("assign a; variable v; Select a pattern a(\"num2\", \"num2-1\")");
            QPSResult expected = {"8"};
            REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
        }

        SECTION("Assign(VarName, Exact Matching) Select v") {
            QPSResult result = qps.processQueries("assign a; variable v; Select v pattern a(\"num2\", \"num2-1\")");
            QPSResult expected = {allVars};
            REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
        }

        SECTION("Assign(Variable, Exact Matching not in store) Select a") {
            QPSResult result = qps.processQueries("assign a; variable v; Select a pattern a(\"num2\", \"num2+1\")");
            REQUIRE_EMPTY(result);
        }

        SECTION("Assign(Variable, Exact Matching not in store) Select v") {
            QPSResult result = qps.processQueries("assign a; variable v; Select v pattern a(\"num2\", \"num2+1\")");
            REQUIRE_EMPTY(result);
        }

        SECTION("Assign(Variable, Partial Matching) Missing variable, Semantic Error") {
            QPSResult result = qps.processQueries("assign a; Select a pattern a(v,_\"1\"_)");
            REQUIRE_THROW_SEMANTIC_ERROR(result);
        }
    }

    SECTION("Test attrRef") {
        SECTION("Select stmt#") {
            SECTION("Stmts") {
                QPSResult result = qps.processQueries("stmt s; Select s.stmt#");
                QPSResult expected = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Assign statements") {
                QPSResult result = qps.processQueries("assign a; Select a.stmt#");
                QPSResult expected = {"1", "5", "8"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("If statements") {
                QPSResult result = qps.processQueries("if ifs; Select ifs.stmt#");
                QPSResult expected = {"3", "9"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("While statements") {
                QPSResult result = qps.processQueries("while w; Select w.stmt#");
                QPSResult expected = {"7"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Call statements") {
                QPSResult result = qps.processQueries("call c; Select c.stmt#");
                QPSResult expected = {"6"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Read statements") {
                QPSResult result = qps.processQueries("read r; Select r.stmt#");
                QPSResult expected = {"2"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Print statements") {
                QPSResult result = qps.processQueries("print pr; Select pr.stmt#");
                QPSResult expected = {"10", "11", "12", "4"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Procedure names") {
                QPSResult result = qps.processQueries("procedure p; Select p.stmt#");
                REQUIRE_THROW_SEMANTIC_ERROR(result);
            }

            SECTION("Variables") {
                QPSResult result = qps.processQueries("variable v; Select v.stmt#");
                REQUIRE_THROW_SEMANTIC_ERROR(result);
            }

            SECTION("Constants") {
                QPSResult result = qps.processQueries("constant c; Select c.stmt#");
                REQUIRE_THROW_SEMANTIC_ERROR(result);
            }
        }

        SECTION("Select procName") {
            SECTION("Call statements") {
                QPSResult result = qps.processQueries("call c; Select c.procName");
                QPSResult expected = {"next"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Procedure names") {
                QPSResult result = qps.processQueries("procedure p; Select p.procName");
                QPSResult expected = {"main", "next"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Stmts") {
                QPSResult result = qps.processQueries("stmt s; Select s.procName");
                REQUIRE_THROW_SEMANTIC_ERROR(result);
            }

            SECTION("Assign statements") {
                QPSResult result = qps.processQueries("assign a; Select a.procName");
                REQUIRE_THROW_SEMANTIC_ERROR(result);
            }

            SECTION("If statements") {
                QPSResult result = qps.processQueries("if ifs; Select ifs.procName");
                REQUIRE_THROW_SEMANTIC_ERROR(result);
            }

            SECTION("While statements") {
                QPSResult result = qps.processQueries("while w; Select w.procName");
                REQUIRE_THROW_SEMANTIC_ERROR(result);
            }

            SECTION("Read statements") {
                QPSResult result = qps.processQueries("read r; Select r.procName");
                REQUIRE_THROW_SEMANTIC_ERROR(result);
            }

            SECTION("Print statements") {
                QPSResult result = qps.processQueries("print p; Select p.procName");
                REQUIRE_THROW_SEMANTIC_ERROR(result);
            }

            SECTION("Variables") {
                QPSResult result = qps.processQueries("variable v; Select v.procName");
                REQUIRE_THROW_SEMANTIC_ERROR(result);
            }

            SECTION("Constants") {
                QPSResult result = qps.processQueries("constant c; Select c.procName");
                REQUIRE_THROW_SEMANTIC_ERROR(result);
            }
        }

        SECTION("Select varName") {
            SECTION("Variables") {
                QPSResult result = qps.processQueries("variable v; Select v.varName");
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, allVars);
            }

            SECTION("Read statements") {
                QPSResult result = qps.processQueries("read r; Select r.varName");
                QPSResult expected = {"num2"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Print statements") {
                QPSResult result = qps.processQueries("print p; Select p.varName");
                QPSResult expected = {"num1", "num2"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Call statements") {
                QPSResult result = qps.processQueries("call c; Select c.varName");
                REQUIRE_THROW_SEMANTIC_ERROR(result);
            }

            SECTION("Procedure names") {
                QPSResult result = qps.processQueries("procedure p; Select p.varName");
                REQUIRE_THROW_SEMANTIC_ERROR(result);
            }

            SECTION("Stmts") {
                QPSResult result = qps.processQueries("stmt s; Select s.varName");
                REQUIRE_THROW_SEMANTIC_ERROR(result);
            }

            SECTION("Assign statements") {
                QPSResult result = qps.processQueries("assign a; Select a.varName");
                REQUIRE_THROW_SEMANTIC_ERROR(result);
            }

            SECTION("If statements") {
                QPSResult result = qps.processQueries("if ifs; Select ifs.varName");
                REQUIRE_THROW_SEMANTIC_ERROR(result);
            }

            SECTION("While statements") {
                QPSResult result = qps.processQueries("while w; Select w.varName");
                REQUIRE_THROW_SEMANTIC_ERROR(result);
            }

            SECTION("Constants") {
                QPSResult result = qps.processQueries("constant c; Select c.varName");
                REQUIRE_THROW_SEMANTIC_ERROR(result);
            }
        }

        SECTION("Select value") {
            SECTION("Constants") {
                QPSResult result = qps.processQueries("constant c; Select c.value");
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, allConsts);
            }

            SECTION("Variables") {
                QPSResult result = qps.processQueries("variable v; Select v.value");
                REQUIRE_THROW_SEMANTIC_ERROR(result);
            }

            SECTION("Read statements") {
                QPSResult result = qps.processQueries("read r; Select r.value");
                REQUIRE_THROW_SEMANTIC_ERROR(result);
            }

            SECTION("Print statements") {
                QPSResult result = qps.processQueries("print p; Select p.value");
                REQUIRE_THROW_SEMANTIC_ERROR(result);
            }

            SECTION("Call statements") {
                QPSResult result = qps.processQueries("call c; Select c.value");
                REQUIRE_THROW_SEMANTIC_ERROR(result);
            }

            SECTION("Procedure names") {
                QPSResult result = qps.processQueries("procedure p; Select p.value");
                REQUIRE_THROW_SEMANTIC_ERROR(result);
            }

            SECTION("Stmts") {
                QPSResult result = qps.processQueries("stmt s; Select s.value");
                REQUIRE_THROW_SEMANTIC_ERROR(result);
            }

            SECTION("Assign statements") {
                QPSResult result = qps.processQueries("assign a; Select a.value");
                REQUIRE_THROW_SEMANTIC_ERROR(result);
            }

            SECTION("If statements") {
                QPSResult result = qps.processQueries("if ifs; Select ifs.value");
                REQUIRE_THROW_SEMANTIC_ERROR(result);
            }

            SECTION("While statements") {
                QPSResult result = qps.processQueries("while w; Select w.value");
                REQUIRE_THROW_SEMANTIC_ERROR(result);
            }
        }

        SECTION("Follows(Synonym, Wildcard)") {
            QPSResult result = qps.processQueries("stmt s; Select s.stmt# such that Follows(s, _)");
            QPSResult expected = {allFollowees};
            REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
        }
    }
};

/*
TEST_CASE("Multi") {
    PKB pkb{};
    PKBFacadeReader pfr{buildPKBNew(pkb)};
    QPS qps{pfr};

    SECTION("Tuple Return Values") {
        SECTION("Tuple, Return attrRef and Synonym") {
            QPSResult result = qps.processQueries(
                "assign a; call c; print pr; read re; Select <a.stmt#, c.procName, pr.varName, re.varName>");
            QPSResult expected = {"1 next num1 num2", "1 next num2 num2", "5 next num1 num2",
                                  "5 next num2 num2", "8 next num1 num2", "8 next num2 num2"};
            REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
        }

        SECTION("Tuple, Return two values, such that") {
            QPSResult result = qps.processQueries("stmt s1, s2; Select <s1,s2> such that Follows(s1, s2)");
            QPSResult expected = {"1 2", "2 3", "3 6", "7 12", "8 9"};
            REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
        }

        SECTION("Tuple, Return two same values, such that") {
            QPSResult result = qps.processQueries("stmt s1, s2; Select <s1,s1> such that Follows(s1, s2)");
            QPSResult expected = {"1 1", "2 2", "3 3", "7 7", "8 8"};
            REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
        }

        SECTION("Tuple, Return three values, such that") {
            QPSResult result =
                qps.processQueries("stmt s1, s2; variable v; Select <s1,s2,v> such that Follows(s1, s2)");
            QPSResult expected = {"1 2 num1", "2 3 num1", "3 6 num1", "7 12 num1", "8 9 num1",
                                  "1 2 num2", "2 3 num2", "3 6 num2", "7 12 num2", "8 9 num2",
                                  "1 2 num3", "2 3 num3", "3 6 num3", "7 12 num3", "8 9 num3"};
            REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
        }

        SECTION("Tuple, Return two values, pattern") {
            QPSResult result = qps.processQueries("assign a; variable v; Select <a, v> pattern a(v,_\"1\"_)");
            QPSResult expected = {"1 num1", "8 num2"};
            REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
        }

        SECTION("Tuple, Return two values(such that), such that and pattern") {
            QPSResult result = qps.processQueries(
                "assign a; stmt s1,s2; variable v; Select <s1, s2> such that Follows(s1,s2) pattern a(v,_\"1\"_)");
            QPSResult expected = {"1 2", "2 3", "3 6", "7 12", "8 9"};
            REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
        }

        SECTION("Tuple, Return two values(pattern), such that and pattern") {
            QPSResult result = qps.processQueries(
                "assign a; stmt s1,s2; variable v; Select <a, v> such that Follows(s1,s2) pattern a(v,_\"1\"_)");
            QPSResult expected = {"1 num1", "8 num2"};
            REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
        }

        SECTION("Tuple, Return two values(with different synonyms), such that and pattern") {
            QPSResult result = qps.processQueries(
                "assign a; stmt s1,s2; variable v; Select <s1, v> such that Follows(s1,s2) pattern a(v,_\"1\"_)");
            QPSResult expected = {"1 num1", "1 num2", "2 num1", "2 num2", "3 num1",
                                  "3 num2", "7 num1", "7 num2", "8 num1", "8 num2"};
            REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
        }

        SECTION("Tuple, Return two values(with shared synonyms), such that and pattern") {
            QPSResult result = qps.processQueries(
                "assign a; stmt s1,s2; variable v; Select <a, s1> such that Follows(a,s2) pattern a(v,_\"1\"_)");
            QPSResult expected = {"1 1", "1 10", "1 11", "1 12", "1 2", "1 3",  "1 4",  "1 5",
                                  "1 6", "1 7",  "1 8",  "1 9",  "8 1", "8 10", "8 11", "8 12",
                                  "8 2", "8 3",  "8 4",  "8 5",  "8 6", "8 7",  "8 8",  "8 9"};
            REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
        }
    }

    SECTION("Multiple Clauses") {
        SECTION("Chained such that") {
            QPSResult result = qps.processQueries(
                "stmt s1, s2; Select s1 such that Follows(s1, s2) such that Parent(s1, _) such that Parent(_, s2)");
            QPSResult expected = {"7"};
            REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
        }

        SECTION("Chained pattern") {
            QPSResult result = qps.processQueries("assign a; variable v; Select a pattern a(_,_) pattern a(v,_\"1\"_)");
            QPSResult expected = {"1", "8"};
            REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
        }

        SECTION("Chained pattern and such that") {
            QPSResult result = qps.processQueries(
                "assign a; variable v; stmt s; Select s such that Parent*(3, s) pattern a(_,_) such that Modifies(s, "
                "\"num2\") pattern a(v,_\"1\"_)");
            QPSResult expected = {"5"};
            REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
        }
    }

    SECTION("Clauses with 'and'") {
        SECTION("Chained such that") {
            QPSResult result = qps.processQueries(
                "stmt s1, s2; Select s1 such that Follows(s1, s2) and Parent(s1, _) and Parent(_, s2)");
            QPSResult expected = {"7"};
            REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
        }

        SECTION("Chained pattern") {
            QPSResult result = qps.processQueries("assign a; variable v; Select a pattern a(_,_) and a(v,_\"1\"_)");
            QPSResult expected = {"1", "8"};
            REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
        }

        SECTION("Chained pattern and such that") {
            QPSResult result = qps.processQueries(
                "assign a; variable v; stmt s; Select s such that Parent*(3, s) and Modifies(s, \"num2\") pattern "
                "a(_,_) and a(v,_\"1\"_)");
            QPSResult expected = {"5"};
            REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
        }
    }
}
*/
