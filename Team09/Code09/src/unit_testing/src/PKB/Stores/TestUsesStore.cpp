#include "catch.hpp"
#include "pkb/Stores/UsesStore.h"
#include "qps/clauseArguments/Integer.h"
#include "qps/clauseArguments/Synonym.h"
#include "qps/clauseArguments/Wildcard.h"

TEST_CASE("UsesStore (Statements) - All Tests") {
    UsesStore usesStore;

    SECTION("Test hasStatementVariableUseRelationship") {
        usesStore.setStatementUsesStore({{1, "x"}, {2, "y"}, {3, "x"}});

        REQUIRE(usesStore.hasStatementVariableUseRelationship(1, "x"));
        REQUIRE(usesStore.hasStatementVariableUseRelationship(2, "y"));
        REQUIRE(usesStore.hasStatementVariableUseRelationship(3, "x"));

        REQUIRE_FALSE(usesStore.hasStatementVariableUseRelationship(1, "y"));
        REQUIRE_FALSE(usesStore.hasStatementVariableUseRelationship(2, "x"));
        REQUIRE_FALSE(usesStore.hasStatementVariableUseRelationship(3, "y"));
    }

    SECTION("Test getVariablesByStatement") {
        usesStore.setStatementUsesStore({{1, "x"}, {2, "y"}, {3, "x"}});

        REQUIRE(usesStore.getVariablesByStatement(1) == std::unordered_set<Variable>{"x"});
        REQUIRE(usesStore.getVariablesByStatement(2) == std::unordered_set<Variable>{"y"});
        REQUIRE(usesStore.getVariablesByStatement(3) == std::unordered_set<Variable>{"x"});
        REQUIRE(usesStore.getVariablesByStatement(4).empty());
    }

    SECTION("Test getStatementsByVariable") {
        usesStore.setStatementUsesStore({{1, "x"}, {2, "y"}, {3, "x"}});

        REQUIRE(usesStore.getStatementsByVariable("x") == std::unordered_set<StmtNum>{1, 3});
        REQUIRE(usesStore.getStatementsByVariable("y") == std::unordered_set<StmtNum>{2});
        REQUIRE(usesStore.getStatementsByVariable("z").empty());
    }

    SECTION(
        "Test hasStatementVariableUseRelationship with Wildcards arg1 and arg2, non-empty usesStore (Expecting True)") {
        usesStore.setStatementUsesStore({{1, "x"}, {2, "y"}, {3, "z"}});
        ClauseArgument* wildcardArg1 = new Wildcard();
        ClauseArgument* wildcardArg2 = new Wildcard();
        REQUIRE(usesStore.hasStatementVariableUseRelationship(*wildcardArg1, *wildcardArg2));

        delete wildcardArg1;
        delete wildcardArg2;
    }

    SECTION(
        "Test hasStatementVariableUseRelationship with Wildcards arg1 and arg2, empty usesStore (Expecting False)") {
        usesStore.setStatementUsesStore({});
        ClauseArgument* wildcardArg1 = new Wildcard();
        ClauseArgument* wildcardArg2 = new Wildcard();
        REQUIRE_FALSE(usesStore.hasStatementVariableUseRelationship(*wildcardArg1, *wildcardArg2));

        delete wildcardArg1;
        delete wildcardArg2;
    }

    SECTION("Test hasStatementVariableUseRelationship with Wildcard arg1 and Synonym arg2 (Expecting True)") {
        usesStore.setStatementUsesStore({{1, "x"}, {2, "y"}, {3, "z"}});
        // Argument 1 is wildcard
        // Argument 2 is synonym with variable use ("y")
        ClauseArgument* wildcardArg1 = new Wildcard();
        ClauseArgument* synonymArg2 = new Synonym(DesignEntityType::VARIABLE, "y");
        REQUIRE(usesStore.hasStatementVariableUseRelationship(*wildcardArg1, *synonymArg2));

        delete wildcardArg1;
        delete synonymArg2;
    }

    SECTION("Test hasStatementVariableUseRelationship with Wildcard arg1 and Synonym arg2 (Expecting False)") {
        usesStore.setStatementUsesStore({{1, "x"}, {2, "y"}, {3, "z"}});
        // Argument 1 is wildcard
        // Argument 2 is synonym with no variable use
        ClauseArgument* wildcardArg1 = new Wildcard();
        ClauseArgument* synonymArg2 = new Synonym(DesignEntityType::VARIABLE, "w");
        REQUIRE_FALSE(usesStore.hasStatementVariableUseRelationship(*wildcardArg1, *synonymArg2));

        delete wildcardArg1;
        delete synonymArg2;
    }

    SECTION("Test hasStatementVariableUseRelationship with Integer arg1 and Wildcard arg2 (Expecting True)") {
        usesStore.setStatementUsesStore({{1, "x"}, {2, "y"}, {3, "z"}});
        // Argument 1 is Integer ("1") with variable use "x"
        // Argument 2 is wildcard
        ClauseArgument* integerArg1 = new Integer("1");
        ClauseArgument* wildcardArg2 = new Wildcard();
        REQUIRE(usesStore.hasStatementVariableUseRelationship(*integerArg1, *wildcardArg2));

        delete integerArg1;
        delete wildcardArg2;
    }

    SECTION("Test hasStatementVariableUseRelationship with Integer arg1 and Wildcard arg2 (Expecting false)") {
        usesStore.setStatementUsesStore({{1, "x"}, {2, "y"}, {3, "z"}});
        // Test argument 1 is Integer("4") with no variable use
        // Test argument 2 is wildcard
        ClauseArgument* integerArg1 = new Integer("4");
        ClauseArgument* wildcardArg2 = new Wildcard();
        REQUIRE_FALSE(usesStore.hasStatementVariableUseRelationship(*integerArg1, *wildcardArg2));

        delete integerArg1;
        delete wildcardArg2;
    }

    SECTION("Test hasStatementVariableUseRelationship with Integer arg1 and Synonym arg2 (Expecting true)") {
        usesStore.setStatementUsesStore({{1, "x"}, {2, "y"}, {3, "z"}});
        // Test when both arguments have variable use
        ClauseArgument* integerArg1 = new Integer("3");
        ClauseArgument* synonymArg2 = new Synonym(DesignEntityType::VARIABLE, "z");
        REQUIRE(usesStore.hasStatementVariableUseRelationship(*integerArg1, *synonymArg2));

        delete integerArg1;
        delete synonymArg2;
    }

    SECTION("Test hasStatementVariableUseRelationship with Integer arg1 and Synonym arg2 (Expecting False)") {
        usesStore.setStatementUsesStore({{1, "x"}, {2, "y"}, {3, "z"}});
        // Test when both arguments have no variable use relationship
        ClauseArgument* integerArg1 = new Integer("3");
        ClauseArgument* synonymArg2 = new Synonym(DesignEntityType::VARIABLE, "w");
        REQUIRE_FALSE(usesStore.hasStatementVariableUseRelationship(*integerArg1, *synonymArg2));

        delete integerArg1;
        delete synonymArg2;
    }
}

TEST_CASE("UsesStore (Procedures) - All Tests") {
    UsesStore usesStore;

    usesStore.setProcedureUsesStore({{"main", "flag"},
                                     {"main", "x"},
                                     {"computeCentroid", "x"},
                                     {"computeCentroid", "y"},
                                     {"computeCentroid", "count"},
                                     {"computeCentroid", "cenX"},
                                     {"computeCentroid", "cenY"},
                                     {"computeCentroid", "flag"},
                                     {"computeCentroid", "normSq"},
                                     {"computeSquare", "x"},
                                     {"computeSquare", "tempX"}});

    SECTION("Test getVariablesByProcedure") {
        REQUIRE(usesStore.getVariablesByProcedure("main") == std::unordered_set<Variable>{"x", "flag"});
        REQUIRE(usesStore.getVariablesByProcedure("computeCentroid") ==
                std::unordered_set<Variable>{"x", "y", "count", "cenX", "cenY", "flag", "normSq"});
        REQUIRE(usesStore.getVariablesByProcedure("computeSquare") == std::unordered_set<Variable>{"x", "tempX"});
    }

    SECTION("Test getProceduresByVariable") {
        REQUIRE(usesStore.getProceduresByVariable("x") ==
                std::unordered_set<Procedure>{"main", "computeCentroid", "computeSquare"});
        REQUIRE(usesStore.getProceduresByVariable("flag") == std::unordered_set<Procedure>{"main", "computeCentroid"});
        REQUIRE(usesStore.getProceduresByVariable("tempX") == std::unordered_set<Procedure>{"computeSquare"});
    }

    SECTION("Test hasProcedureVariableUseRelationship") {
        REQUIRE(usesStore.hasProcedureVariableUseRelationship("main", "x"));
        REQUIRE(usesStore.hasProcedureVariableUseRelationship("computeCentroid", "y"));
        REQUIRE(usesStore.hasProcedureVariableUseRelationship("computeSquare", "tempX"));

        REQUIRE_FALSE(usesStore.hasProcedureVariableUseRelationship("main", "tempX"));
        REQUIRE_FALSE(usesStore.hasProcedureVariableUseRelationship("computeCentroid", "test"));
        REQUIRE_FALSE(usesStore.hasProcedureVariableUseRelationship("computeSquare", "y"));
    }

    SECTION("Test hasProcedureVariableUseRelationship with Wildcard arg1 and Synonym arg2 (Expecting True)") {
        // Argument 1 is wildcard
        // Argument 2 is synonym with variable uses ("y")
        ClauseArgument* wildcardArg1 = new Wildcard();
        ClauseArgument* synonymArg2 = new Synonym(DesignEntityType::VARIABLE, "y");
        REQUIRE(usesStore.hasProcedureVariableUseRelationship(*wildcardArg1, *synonymArg2));

        delete wildcardArg1;
        delete synonymArg2;
    }

    SECTION("Test hasProcedureVariableUseRelationship with Wildcard arg1 and Synonym arg2 (Expecting False)") {
        // Argument 1 is wildcard
        // Argument 2 is synonym with no variable uses
        ClauseArgument* wildcardArg1 = new Wildcard();
        ClauseArgument* synonymArg2 = new Synonym(DesignEntityType::VARIABLE, "w");
        REQUIRE_FALSE(usesStore.hasProcedureVariableUseRelationship(*wildcardArg1, *synonymArg2));

        delete wildcardArg1;
        delete synonymArg2;
    }

    SECTION("Test hasProcedureVariableUseRelationship with procedure Synonym arg1 and Wildcard arg2 (Expecting True)") {
        // Argument 1 is procedure ("main") that modified variables
        // Argument 2 is wildcard
        ClauseArgument* procedureArg1 = new Synonym(DesignEntityType::PROCEDURE, "main");
        ClauseArgument* wildcardArg2 = new Wildcard();
        REQUIRE(usesStore.hasProcedureVariableUseRelationship(*procedureArg1, *wildcardArg2));

        delete procedureArg1;
        delete wildcardArg2;
    }

    SECTION(
        "Test hasProcedureVariableUseRelationship with procedure Synonym arg1 and Wildcard arg2 (Expecting false)") {
        // Argument 1 is procedure ("home") without modifying any variables
        // Argument 2 is wildcard
        ClauseArgument* procedureArg1 = new Synonym(DesignEntityType::PROCEDURE, "home");
        ClauseArgument* wildcardArg2 = new Wildcard();
        REQUIRE_FALSE(usesStore.hasProcedureVariableUseRelationship(*procedureArg1, *wildcardArg2));

        delete procedureArg1;
        delete wildcardArg2;
    }

    SECTION(
        "Test hasProcedureVariableUseRelationship with procedure Synonym arg1 and variable Synonym arg2 "
        "(Expecting "
        "True)") {
        // Argument 1 is procedure ("main") that modified variables
        // Argument 2 is synonym with variable uses ("x")
        ClauseArgument* procedureArg1 = new Synonym(DesignEntityType::PROCEDURE, "main");
        ClauseArgument* variableArg2 = new Synonym(DesignEntityType::VARIABLE, "x");
        REQUIRE(usesStore.hasProcedureVariableUseRelationship(*procedureArg1, *variableArg2));

        delete procedureArg1;
        delete variableArg2;
    }

    SECTION(
        "Test hasProcedureVariableUseRelationship with procedure Synonym arg1 and variable Synonym arg2 (Expecting "
        "false)") {
        // Argument 1 is procedure ("main")
        // Argument 2 is synonym with variable uses ("tempX")
        ClauseArgument* procedureArg1 = new Synonym(DesignEntityType::PROCEDURE, "main");
        ClauseArgument* variableArg2 = new Synonym(DesignEntityType::VARIABLE, "tempX");
        REQUIRE_FALSE(usesStore.hasProcedureVariableUseRelationship(*procedureArg1, *variableArg2));

        delete procedureArg1;
        delete variableArg2;
    }
}
