#include <filesystem>
#include <set>
#include <unordered_set>

#include "PKB/PKB.h"
#include "PKB/Utils/DataTypes.h"
#include "Spa.h"
#include "catch.hpp"

const char CODE_DIRECTORY_NAME[] = "Code09";

TEST_CASE("SP-PKB: Design Entities stored correctly") {
    SPA spa;
    auto filePath = std::filesystem::current_path();
    while (filePath.filename() != CODE_DIRECTORY_NAME) {
        filePath = filePath.parent_path();
    }

    filePath /= "src/integration_testing/test_files/basic_source.txt";

    spa.parse(filePath.string());
    PKB& pkb = spa.getInternalPKB();
    PKBFacadeReader pkbFacadeReader(pkb);

    SECTION("Variables stored correctly") {
        std::unordered_set<Variable> variableSet = pkbFacadeReader.getVariables();
        std::unordered_set<Variable> expectedVariableSet = {
            "a",   "c",   "d",   "e",   "f",   "g",   "h",    "i",     "abc",  "xyz", "q",     "w",  "y",   "z",
            "r",   "t",   "a0",  "a1",  "a2",  "a3",  "a4",   "a5",    "a6",   "a7",  "a8",    "a9", "a10", "a14",
            "a15", "a16", "a17", "a20", "a21", "a22", "a23",  "a25",   "a33",  "a34", "while", "if", "gg",  "hh",
            "ii",  "o",   "m",   "x",   "b",   "b1",  "then", "print", "else", "a34", "read"};
        REQUIRE(std::set<Variable>(variableSet.begin(), variableSet.end()) ==
                std::set<Variable>(expectedVariableSet.begin(), expectedVariableSet.end()));
    }

    SECTION("Constants stored correctly") {
        std::unordered_set<Constant> constantsSet = pkbFacadeReader.getConstants();
        std::unordered_set<Constant> expectedConstantsSet = {"2",  "4",   "999", "1111", "9999", "50", "110", "40",
                                                             "10", "170", "29",  "1",    "11",   "90", "22",  "55",
                                                             "18", "3",   "34",  "27",   "15",   "6",  "68",  "0"};
        REQUIRE(std::set<Variable>(constantsSet.begin(), constantsSet.end()) ==
                std::set<Variable>(expectedConstantsSet.begin(), expectedConstantsSet.end()));
    }

    SECTION("Statements stored correctly") {
        std::unordered_set<Stmt> stmtSet = pkbFacadeReader.getStmts();
        std::unordered_set<Stmt> expectedStmtSet = {};
        std::unordered_set<int> assign = {1, 2, 3, 4, 5, 8, 9, 12, 13, 14, 15, 16, 17, 27, 28, 31, 32, 37, 39, 42};
        std::unordered_set<int> print = {6, 7, 10, 11, 20, 24, 35, 43, 47};
        std::unordered_set<int> read = {18, 25, 44, 48};
        std::unordered_set<int> ifs = {19, 22, 23, 26, 30, 34};
        std::unordered_set<int> call = {21, 29, 33, 36};
        std::unordered_set<int> whiles = {38, 40, 41, 45, 46};
        for (int i = 1; i < 51; i++) {
            if (assign.find(i) != assign.end()) {
                expectedStmtSet.insert({StatementType::ASSIGN, i});
            } else if (print.find(i) != print.end()) {
                expectedStmtSet.insert({StatementType::PRINT, i});
            } else if (read.find(i) != read.end()) {
                expectedStmtSet.insert({StatementType::READ, i});
            } else if (ifs.find(i) != ifs.end()) {
                expectedStmtSet.insert({StatementType::IF, i});
            } else if (call.find(i) != call.end()) {
                expectedStmtSet.insert({StatementType::CALL, i});
            } else if (whiles.find(i) != whiles.end()) {
                expectedStmtSet.insert({StatementType::WHILE, i});
            }
        }
        REQUIRE(std::set<Stmt>(stmtSet.begin(), stmtSet.end()) ==
                std::set<Stmt>(expectedStmtSet.begin(), expectedStmtSet.end()));
    }

    SECTION("Procedures stored correctly") {
        std::unordered_set<Procedure> procSet = pkbFacadeReader.getProcedures();
        std::unordered_set<Procedure> expectedProcSet = {"main", "if", "while"};
        REQUIRE(procSet == expectedProcSet);
    }
}

TEST_CASE("SP-PKB: Design Abstractions stored correctly") {
    SPA spa;
    auto filePath = std::filesystem::current_path();
    while (filePath.filename() != CODE_DIRECTORY_NAME) {
        filePath = filePath.parent_path();
    }

    filePath /= "src/integration_testing/test_files/basic_source.txt";

    spa.parse(filePath.string());
    PKB& pkb = spa.getInternalPKB();
    PKBFacadeReader pkbFacadeReader(pkb);

    // SECTION("Follows relationship stored correctly") {
    //     REQUIRE(pkbFacadeReader.hasFollowRelationship(1, 2));
    //     REQUIRE_FALSE(pkbFacadeReader.hasFollowRelationship(6, 7));
    // }

    // SECTION("Parent relationship stored correctly") {
    //     REQUIRE(pkbFacadeReader.hasParentRelationship(1, 2));
    //     REQUIRE_FALSE(pkbFacadeReader.hasParentRelationship(6, 7));
    // }

    SECTION("Uses relationship stored correctly - statements") {}

    SECTION("Uses relationship stored correctly - procedures") {}

    SECTION("Modifies relationship stored correctly - statements") {}

    SECTION("Modifies relationship stored correctly - procedures") {}

    SECTION("Calls relationship stored correctly") {}

    SECTION("Follows relationship stored correctly") {}

    SECTION("Next relationship stored correctly") {}

    SECTION("Pattern stored correctly") {}
}
