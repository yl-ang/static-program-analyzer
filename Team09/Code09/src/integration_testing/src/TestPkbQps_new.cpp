#include "PKB/PKBClient/PKBFacadeReader.h"
#include "PKB/PKBClient/PKBFacadeWriter.h"
#include "catch.hpp"
#include "pkb/PKB.h"
#include "qps/QPS.h"

namespace {
// TYPE DEFS
using QPSResult = std::vector<std::string>;

// CUSTOM TESTING MACROS
#define REQUIRE_THROW_SEMANTIC_ERROR(expr) REQUIRE((expr)[0] == "SemanticError")
#define REQUIRE_THROW_SYNTAX_ERROR(expr) REQUIRE((expr)[0] == "SyntaxError")
#define REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected) equalVectorContents(result, expected)
#define REQUIRE_EMPTY(result) REQUIRE((result).empty())

// HELPER FUNCTIONS
void equalVectorContents(QPSResult result, QPSResult expected) {
    std::sort(result.begin(), result.end());
    std::sort(expected.begin(), expected.end());
    REQUIRE(result == expected);
}

// PKB Items
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

const std::unordered_set<std::pair<StmtNum, Variable>> modifiesStoreEntries = {{1, "num1"}, {2, "num2"}, {3, "num2"},
                                                                               {5, "num2"}, {8, "num2"}, {7, "num2"}};
const std::unordered_set<std::pair<StmtNum, Variable>> usesStoreEntries = {
    {3, "num1"}, {3, "num2"}, {4, "num1"}, {5, "num1"},  {7, "num1"},
    {7, "num2"}, {8, "num2"}, {9, "num2"}, {10, "num2"}, {12, "num2"}};

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
                QPSResult result = qps.processQueries("stmt s; Select s such that Parent*(3, s)");
                QPSResult expected = {"4", "5"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
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

            SECTION("Modifies(Wildcard, Variable)") {
                REQUIRE_THROW_SEMANTIC_ERROR(qps.processQueries("stmt s; Select s such that Modifies(_, \"num2\")"));
            }

            /* TODO(Ezekiel): Enable this test when we support procedure for modifies
            SECTION("Modifies(Procedure, Variable)") {
                QPSResult result = qps.processQueries("procedure p; Select p such that Modifies(p, \"num2\")");
                QPSResult expected = {"main", "next"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }
            */
        }

        SECTION("1 synonym") {
            SECTION("Modifies(Integer, synonym)") {
                QPSResult result = qps.processQueries("variable v; Select v such that Modifies(1, v)");
                QPSResult expected = {"num1"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Modifies(Synonym, Variable)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Modifies(s, \"num2\")");
                QPSResult expected = {"2", "3", "5", "7", "8"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Modifies(Synonym, Wildcard)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Modifies(s, _)");
                QPSResult expected = {"1", "2", "3", "5", "7", "8"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            /* TODO(Ezekiel): Enable this test when we support procedure for modifies
            SECTION("Modifies(Procedure, synonym)") {
                QPSResult result = qps.processQueries("variable v; Select v such that Modifies(\"main\", v)");
                QPSResult expected = {"num1", "num2"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }
            */
        }

        SECTION("2 synonyms") {
            SECTION("Modifies(StmtSyn, VariableSyn)") {
                QPSResult result = qps.processQueries("stmt s; variable v; Select s such that Modifies(s, v)");
                QPSResult expected = {"1", "2", "3", "5", "7", "8"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Modifies(assignSyn, VariableSyn)") {
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
        SECTION("No synonyms") {
            SECTION("Uses(Integer, VariableName)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Uses(3, \"num1\")");
                QPSResult expected = {allStmts};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Uses(Wildcard, VariableName)") {
                REQUIRE_THROW_SEMANTIC_ERROR(qps.processQueries("stmt s; Select s such that Uses(_, \"num2\")"));
            }

            /* TODO(Ezekiel): Enable this test when we support procedure for uses
            SECTION("Uses(ProcedureName, VariableName)") {
                QPSResult result = qps.processQueries("procedure p; Select p such that Uses(\"main\", \"num1\")");
                QPSResult expected = {"main"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }
            */

            SECTION("Uses(Integer, Wildcard)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Uses(3, _)");
                QPSResult expected = {allStmts};  // 3 uses variables
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }
        }

        SECTION("1 synonym") {
            SECTION("Uses(Synonym, VariableName)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Uses(s, \"num1\")");
                QPSResult expected = {"3", "4", "5", "7"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Uses(Synonym, Wildcard)") {
                QPSResult result = qps.processQueries("stmt s; Select s such that Uses(s, _)");
                // Assuming all statements that either use num1 or num2
                QPSResult expected = {"3", "4", "5", "7", "8", "9", "10", "12"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            /* TODO(Ezekiel): Enable this test when we support procedure for uses
            SECTION("Uses(ProcedureName, Synonym)") {
                QPSResult result = qps.processQueries("variable v; Select v such that Uses(\"main\", v)");
                QPSResult expected = {"num1", "num2"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }
            */
        }

        SECTION("2 synonyms") {
            SECTION("Uses(Synonym, Synonym) - Stmt Variable pairing") {
                QPSResult result = qps.processQueries("stmt s; variable v; Select s such that Uses(s, v)");
                QPSResult expected = {"3", "4", "5", "7", "8", "9", "10", "12"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }

            SECTION("Uses(Assign, Synonym) - Assign Variable pairing") {
                QPSResult result = qps.processQueries("assign a; variable v; Select a such that Uses(a, v)");
                QPSResult expected = {"5", "8"};
                REQUIRE_EQUAL_VECTOR_CONTENTS(result, expected);
            }
        }
    }
    // ai-gen end
};