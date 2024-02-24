#include "PKB/Stores/UsesStore.h"
#include "catch.hpp"
#include "qps/clauseArguments/Integer.h"
#include "qps/clauseArguments/Synonym.h"
#include "qps/clauseArguments/Wildcard.h"

TEST_CASE("UsesStore - All Tests") {
    UsesStore usesStore;

    SECTION("Test hasStatementVariableUseRelationship") {
        usesStore.setUsesStore({{1, "x"}, {2, "y"}, {3, "x"}});

        REQUIRE(usesStore.hasStatementVariableUseRelationship(1, "x"));
        REQUIRE(usesStore.hasStatementVariableUseRelationship(2, "y"));
        REQUIRE(usesStore.hasStatementVariableUseRelationship(3, "x"));

        REQUIRE_FALSE(usesStore.hasStatementVariableUseRelationship(1, "y"));
        REQUIRE_FALSE(usesStore.hasStatementVariableUseRelationship(2, "x"));
        REQUIRE_FALSE(usesStore.hasStatementVariableUseRelationship(3, "y"));
    }

    SECTION("Test getVariablesByStatement") {
        usesStore.setUsesStore({{1, "x"}, {2, "y"}, {3, "x"}});

        REQUIRE(usesStore.getVariablesByStatement(1) == std::unordered_set<Variable>{"x"});
        REQUIRE(usesStore.getVariablesByStatement(2) == std::unordered_set<Variable>{"y"});
        REQUIRE(usesStore.getVariablesByStatement(3) == std::unordered_set<Variable>{"x"});
        REQUIRE(usesStore.getVariablesByStatement(4).empty());
    }

    SECTION("Test getStatementsByVariable") {
        usesStore.setUsesStore({{1, "x"}, {2, "y"}, {3, "x"}});

        REQUIRE(usesStore.getStatementsByVariable("x") == std::unordered_set<StmtNum>{1, 3});
        REQUIRE(usesStore.getStatementsByVariable("y") == std::unordered_set<StmtNum>{2});
        REQUIRE(usesStore.getStatementsByVariable("z").empty());
    }

    SECTION(
        "Test hasStatementVariableUseRelationship with Wildcards arg1 and arg2, non-empty usesStore (Expecting True)") {
        usesStore.setUsesStore({{1, "x"}, {2, "y"}, {3, "z"}});
        ClauseArgument* wildcardArg1 = new Wildcard();
        ClauseArgument* wildcardArg2 = new Wildcard();
        REQUIRE(usesStore.hasStatementVariableUseRelationship(*wildcardArg1, *wildcardArg2));

        delete wildcardArg1;
        delete wildcardArg2;
    }

    SECTION(
        "Test hasStatementVariableUseRelationship with Wildcards arg1 and arg2, empty usesStore (Expecting False)") {
        usesStore.setUsesStore({});
        ClauseArgument* wildcardArg1 = new Wildcard();
        ClauseArgument* wildcardArg2 = new Wildcard();
        REQUIRE_FALSE(usesStore.hasStatementVariableUseRelationship(*wildcardArg1, *wildcardArg2));

        delete wildcardArg1;
        delete wildcardArg2;
    }

    SECTION("Test hasStatementVariableUseRelationship with Wildcard arg1 and Synonym arg2 (Expecting True)") {
        usesStore.setUsesStore({{1, "x"}, {2, "y"}, {3, "z"}});
        // Argument 1 is wildcard
        // Argument 2 is synonym with variable use ("y")
        ClauseArgument* wildcardArg1 = new Wildcard();
        ClauseArgument* synonymArg2 = new Synonym(DesignEntityType::VARIABLE, "y");
        REQUIRE(usesStore.hasStatementVariableUseRelationship(*wildcardArg1, *synonymArg2));

        delete wildcardArg1;
        delete synonymArg2;
    }

    SECTION("Test hasStatementVariableUseRelationship with Wildcard arg1 and Synonym arg2 (Expecting False)") {
        usesStore.setUsesStore({{1, "x"}, {2, "y"}, {3, "z"}});
        // Argument 1 is wildcard
        // Argument 2 is synonym with no variable use
        ClauseArgument* wildcardArg1 = new Wildcard();
        ClauseArgument* synonymArg2 = new Synonym(DesignEntityType::VARIABLE, "w");
        REQUIRE_FALSE(usesStore.hasStatementVariableUseRelationship(*wildcardArg1, *synonymArg2));

        delete wildcardArg1;
        delete synonymArg2;
    }

    SECTION("Test hasStatementVariableUseRelationship with Integer arg1 and Wildcard arg2 (Expecting True)") {
        usesStore.setUsesStore({{1, "x"}, {2, "y"}, {3, "z"}});
        // Argument 1 is Integer ("1") with variable use "x"
        // Argument 2 is wildcard
        ClauseArgument* integerArg1 = new Integer("1");
        ClauseArgument* wildcardArg2 = new Wildcard();
        REQUIRE(usesStore.hasStatementVariableUseRelationship(*integerArg1, *wildcardArg2));

        delete integerArg1;
        delete wildcardArg2;
    }

    SECTION("Test hasStatementVariableUseRelationship with Integer arg1 and Wildcard arg2 (Expecting false)") {
        usesStore.setUsesStore({{1, "x"}, {2, "y"}, {3, "z"}});
        // Test argument 1 is Integer("4") with no variable use
        // Test argument 2 is wildcard
        ClauseArgument* integerArg1 = new Integer("4");
        ClauseArgument* wildcardArg2 = new Wildcard();
        REQUIRE_FALSE(usesStore.hasStatementVariableUseRelationship(*integerArg1, *wildcardArg2));

        delete integerArg1;
        delete wildcardArg2;
    }

    SECTION("Test hasStatementVariableUseRelationship with Integer arg1 and Synonym arg2 (Expecting true)") {
        usesStore.setUsesStore({{1, "x"}, {2, "y"}, {3, "z"}});
        // Test when both arguments are synonyms and have variable use
        ClauseArgument* integerArg1 = new Integer("3");
        ClauseArgument* synonymArg2 = new Synonym(DesignEntityType::VARIABLE, "z");
        REQUIRE(usesStore.hasStatementVariableUseRelationship(*integerArg1, *synonymArg2));

        delete integerArg1;
        delete synonymArg2;
    }

    SECTION("Test hasStatementVariableUseRelationship with Synonym arg1 and Synonym arg2 (Expecting False)") {
        usesStore.setUsesStore({{1, "x"}, {2, "y"}, {3, "z"}});
        // Test when both arguments are synonyms but no variable use relationship
        ClauseArgument* integerArg1 = new Integer("3");
        ClauseArgument* synonymArg2 = new Synonym(DesignEntityType::VARIABLE, "w");
        REQUIRE_FALSE(usesStore.hasStatementVariableUseRelationship(*integerArg1, *synonymArg2));

        delete integerArg1;
        delete synonymArg2;
    }
}
