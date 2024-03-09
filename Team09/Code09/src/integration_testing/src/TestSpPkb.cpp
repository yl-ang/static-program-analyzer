#include <filesystem>

#include "PKB/PKB.h"
#include "PKB/Utils/DataTypes.h"
#include "Spa.h"
#include "catch.hpp"

TEST_CASE("Test sp and pkb - basic source.txt") {
    SPA spa;
    // Team, this path is for vs studio, as the integration binary is running from the out folder... (Mac user take
    // note)
    spa.parse("../../../../../src/integration_testing/test_files/basic_source.txt");
    PKB& pkb = spa.getInternalPKB();
    PKBFacadeReader pkbFacadeReader(pkb);

    SECTION("SP-PKB Entity:Procedure") {
        std::unordered_set<Procedure> procSet = pkbFacadeReader.getProcedures();
        std::unordered_set<Procedure> expectedProcSet = {"Animals"};
        REQUIRE(procSet == expectedProcSet);
    }

    SECTION("SP PKB get variables") {
        std::unordered_set<Variable> variableSet = pkbFacadeReader.getVariables();
        std::unordered_set<Variable> expectedVariableSet = {
            "elephant", "iteration",   "additionalVar1", "totalAnimals", "additionalVar2",
            "tiger",    "monkeyCount", "lionCount",      "newVar",       "x",
            "k"};
        REQUIRE(variableSet == expectedVariableSet);
    }

    SECTION("SP PKB get constants") {
        std::unordered_set<Constant> constantSet = pkbFacadeReader.getConstants();
        std::unordered_set<Constant> expectedConstantSet = {
            "1",    "100",      "0",    "332",  "42",    "333", "5",    "1234",   "2",       "9999", "111", "222",
            "8888", "22228888", "2024", "2020", "10101", "17",  "1111", "641111", "9092229", "50",   "3"};
        REQUIRE(constantSet == expectedConstantSet);
    }

    // TODO(yl-ang): SP PKB Statements tests

    SECTION("SP-PKB Follows Relation") {
        REQUIRE(pkbFacadeReader.hasFollowRelationship(1, 2));
        REQUIRE(pkbFacadeReader.hasFollowRelationship(2, 3));
        REQUIRE(pkbFacadeReader.hasFollowRelationship(3, 4));
        REQUIRE(pkbFacadeReader.hasFollowRelationship(4, 5));
        REQUIRE(pkbFacadeReader.hasFollowRelationship(5, 6));
        REQUIRE_FALSE(pkbFacadeReader.hasFollowRelationship(6, 7));
        REQUIRE(pkbFacadeReader.hasFollowRelationship(7, 8));
        REQUIRE(pkbFacadeReader.hasFollowRelationship(8, 9));
        REQUIRE_FALSE(pkbFacadeReader.hasFollowRelationship(9, 10));
        REQUIRE(pkbFacadeReader.hasFollowRelationship(10, 11));
        REQUIRE(pkbFacadeReader.hasFollowRelationship(11, 12));
        REQUIRE(pkbFacadeReader.hasFollowRelationship(12, 13));
        REQUIRE_FALSE(pkbFacadeReader.hasFollowRelationship(13, 14));
        REQUIRE(pkbFacadeReader.hasFollowRelationship(14, 15));
        REQUIRE(pkbFacadeReader.hasFollowRelationship(15, 16));
        REQUIRE(pkbFacadeReader.hasFollowRelationship(16, 17));
        REQUIRE(pkbFacadeReader.hasFollowRelationship(17, 18));
        REQUIRE(pkbFacadeReader.hasFollowRelationship(18, 19));
        REQUIRE_FALSE(pkbFacadeReader.hasFollowRelationship(19, 20));
        REQUIRE_FALSE(pkbFacadeReader.hasFollowRelationship(20, 21));
        REQUIRE(pkbFacadeReader.hasFollowRelationship(21, 22));
        REQUIRE_FALSE(pkbFacadeReader.hasFollowRelationship(22, 23));
        REQUIRE_FALSE(pkbFacadeReader.hasFollowRelationship(6, 22));
        REQUIRE(pkbFacadeReader.hasFollowRelationship(6, 23));
        REQUIRE_FALSE(pkbFacadeReader.hasFollowRelationship(23, 24));
        REQUIRE_FALSE(pkbFacadeReader.hasFollowRelationship(23, 26));
        REQUIRE(pkbFacadeReader.hasFollowRelationship(23, 27));
        REQUIRE(pkbFacadeReader.hasFollowRelationship(24, 25));
        REQUIRE(pkbFacadeReader.hasFollowRelationship(25, 26));
        REQUIRE_FALSE(pkbFacadeReader.hasFollowRelationship(26, 27));
    }
}
