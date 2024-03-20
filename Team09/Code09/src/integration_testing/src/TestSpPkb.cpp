#include <filesystem>

#include "PKB/PKB.h"
#include "PKB/Utils/DataTypes.h"
#include "Spa.h"
#include "catch.hpp"

TEST_CASE("SP-PKB: Design Entities stored correctly") {
    SPA spa;
    auto filePath = std::filesystem::current_path();
    for (int i = 0; i < 3; i++) {
        filePath = filePath.parent_path();
    }

    filePath /= "src/integration_testing/test_files/basic_source.txt";

    spa.parse(filePath);
    PKB& pkb = spa.getInternalPKB();
    PKBFacadeReader pkbFacadeReader(pkb);

    SECTION("Variables stored correctly") {
        std::unordered_set<Variable> variableSet = pkbFacadeReader.getVariables();
        std::unordered_set<Variable> expectedVariableSet = {
            "elephant", "totalAnimals", "additionalVar2", "monkeyCount", "iteration", "additionalVar1",
            "newVar",   "tiger",        "lionCount",      "x",           "k"};
        REQUIRE(variableSet == expectedVariableSet);
    }

    SECTION("Constants stored correctly") {
        std::unordered_set<Constant> constantsSet = pkbFacadeReader.getConstants();
        std::unordered_set<Constant> expectedConstantsSet = {
            "111", "5",   "1",    "2",    "42",   "9999", "641111", "222",      "17",      "100", "1234", "333",
            "0",   "332", "8888", "1111", "2024", "2020", "10101",  "22228888", "9092229", "50",  "3"};
        REQUIRE(constantsSet == expectedConstantsSet);
    }

    // SECTION("Statements stored correctly") {
    //     std::unordered_set<Stmt> stmtSet = pkbFacadeReader.getStmts();
    //     std::unordered_set<Stmt> expectedStmtSet = {{StatementType::READ, 1}, {StatementType::ASSIGN, 2}};
    //     REQUIRE(stmtSet == expectedStmtSet);
    // }

    SECTION("Procedures stored correctly") {
        std::unordered_set<Procedure> procSet = pkbFacadeReader.getProcedures();
        std::unordered_set<Procedure> expectedProcSet = {"Animals"};
        REQUIRE(procSet == expectedProcSet);
    }
}

TEST_CASE("SP-PKB: Design Abstractions stored correctly") {
    SPA spa;
    auto filePath = std::filesystem::current_path();
    for (int i = 0; i < 3; i++) {
        filePath = filePath.parent_path();
    }

    filePath /= "src/integration_testing/test_files/basic_source.txt";

    spa.parse(filePath);
    PKB& pkb = spa.getInternalPKB();
    PKBFacadeReader pkbFacadeReader(pkb);

    SECTION("Follows relationship stored correctly") {
        REQUIRE(pkbFacadeReader.hasFollowRelationship(1, 2));
        REQUIRE_FALSE(pkbFacadeReader.hasFollowRelationship(6, 7));
    }

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
