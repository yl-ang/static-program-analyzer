#include "PKB/Stores/ModifiesStore.h"
#include "PKB/Utils/DataTypes.h"
#include "catch.hpp"
#include "qps/clauseArguments/Integer.h"
#include "qps/clauseArguments/Synonym.h"
#include "qps/clauseArguments/Wildcard.h"

TEST_CASE("ModifiesStore - All Tests") {
    ModifiesStore modifiesStore;

    SECTION("Test hasStatementVariableModifiesRelationship") {
        modifiesStore.setModifiesStore({{1, "x"}, {2, "y"}, {3, "x"}});

        REQUIRE(modifiesStore.hasStatementVariableModifiesRelationship(1, "x"));
        REQUIRE(modifiesStore.hasStatementVariableModifiesRelationship(2, "y"));
        REQUIRE(modifiesStore.hasStatementVariableModifiesRelationship(3, "x"));

        REQUIRE_FALSE(modifiesStore.hasStatementVariableModifiesRelationship(1, "y"));
        REQUIRE_FALSE(modifiesStore.hasStatementVariableModifiesRelationship(2, "x"));
        REQUIRE_FALSE(modifiesStore.hasStatementVariableModifiesRelationship(3, "y"));
    }

    SECTION("Test getVariablesByStatement") {
        modifiesStore.setModifiesStore({{1, "x"}, {2, "y"}, {3, "x"}});

        REQUIRE(modifiesStore.getVariablesByStatement(1) == std::unordered_set<Variable>{"x"});
        REQUIRE(modifiesStore.getVariablesByStatement(2) == std::unordered_set<Variable>{"y"});
        REQUIRE(modifiesStore.getVariablesByStatement(3) == std::unordered_set<Variable>{"x"});
        REQUIRE(modifiesStore.getVariablesByStatement(4).empty());
    }

    SECTION("Test getStatementsByVariable") {
        modifiesStore.setModifiesStore({{1, "x"}, {2, "y"}, {3, "x"}});

        REQUIRE(modifiesStore.getStatementsByVariable("x") == std::unordered_set<StmtNum>{1, 3});
        REQUIRE(modifiesStore.getStatementsByVariable("y") == std::unordered_set<StmtNum>{2});
        REQUIRE(modifiesStore.getStatementsByVariable("z").empty());
    }

    SECTION(
        "Test hasStatementVariableModifiesRelationship with Wildcards arg1 and arg2, non-empty modifiesStore "
        "(Expecting True)") {
        modifiesStore.setModifiesStore({{1, "x"}, {2, "y"}, {3, "z"}});
        ClauseArgument* wildcardArg1 = new Wildcard();
        ClauseArgument* wildcardArg2 = new Wildcard();
        REQUIRE(modifiesStore.hasStatementVariableModifiesRelationship(*wildcardArg1, *wildcardArg2));

        delete wildcardArg1;
        delete wildcardArg2;
    }

    SECTION(
        "Test hasStatementVariableModifiesRelationship with Wildcards arg1 and arg2, empty modifiesStore (Expecting "
        "False)") {
        modifiesStore.setModifiesStore({});
        ClauseArgument* wildcardArg1 = new Wildcard();
        ClauseArgument* wildcardArg2 = new Wildcard();
        REQUIRE_FALSE(modifiesStore.hasStatementVariableModifiesRelationship(*wildcardArg1, *wildcardArg2));

        delete wildcardArg1;
        delete wildcardArg2;
    }

    SECTION("Test hasStatementVariableModifiesRelationship with Wildcard arg1 and Synonym arg2 (Expecting True)") {
        modifiesStore.setModifiesStore({{1, "x"}, {2, "y"}, {3, "z"}});
        // Argument 1 is wildcard
        // Argument 2 is synonym with variable modifies ("y")
        ClauseArgument* wildcardArg1 = new Wildcard();
        ClauseArgument* synonymArg2 = new Synonym(DesignEntityType::VARIABLE, "y");
        REQUIRE(modifiesStore.hasStatementVariableModifiesRelationship(*wildcardArg1, *synonymArg2));

        delete wildcardArg1;
        delete synonymArg2;
    }

    SECTION("Test hasStatementVariableModifiesRelationship with Wildcard arg1 and Synonym arg2 (Expecting False)") {
        modifiesStore.setModifiesStore({{1, "x"}, {2, "y"}, {3, "z"}});
        // Argument 1 is wildcard
        // Argument 2 is synonym with no variable modifies
        ClauseArgument* wildcardArg1 = new Wildcard();
        ClauseArgument* synonymArg2 = new Synonym(DesignEntityType::VARIABLE, "w");
        REQUIRE_FALSE(modifiesStore.hasStatementVariableModifiesRelationship(*wildcardArg1, *synonymArg2));

        delete wildcardArg1;
        delete synonymArg2;
    }

    SECTION("Test hasStatementVariableModifiesRelationship with Integer arg1 and Wildcard arg2 (Expecting True)") {
        modifiesStore.setModifiesStore({{1, "x"}, {2, "y"}, {3, "z"}});
        // Argument 1 is Integer ("1") with variable modifies "x"
        // Argument 2 is wildcard
        ClauseArgument* integerArg1 = new Integer("1");
        ClauseArgument* wildcardArg2 = new Wildcard();
        REQUIRE(modifiesStore.hasStatementVariableModifiesRelationship(*integerArg1, *wildcardArg2));

        delete integerArg1;
        delete wildcardArg2;
    }

    SECTION("Test hasStatementVariableModifiesRelationship with Integer arg1 and Wildcard arg2 (Expecting false)") {
        modifiesStore.setModifiesStore({{1, "x"}, {2, "y"}, {3, "z"}});
        // Test argument 1 is Integer("4") with no variable modifies
        // Test argument 2 is wildcard
        ClauseArgument* integerArg1 = new Integer("4");
        ClauseArgument* wildcardArg2 = new Wildcard();
        REQUIRE_FALSE(modifiesStore.hasStatementVariableModifiesRelationship(*integerArg1, *wildcardArg2));

        delete integerArg1;
        delete wildcardArg2;
    }

    SECTION("Test hasStatementVariableModifiesRelationship with Integer arg1 and Synonym arg2 (Expecting true)") {
        modifiesStore.setModifiesStore({{1, "x"}, {2, "y"}, {3, "z"}});
        // Test when both arguments have variable modifies
        ClauseArgument* integerArg1 = new Integer("3");
        ClauseArgument* synonymArg2 = new Synonym(DesignEntityType::VARIABLE, "z");
        REQUIRE(modifiesStore.hasStatementVariableModifiesRelationship(*integerArg1, *synonymArg2));

        delete integerArg1;
        delete synonymArg2;
    }

    SECTION("Test hasStatementVariableModifiesRelationship with Integer arg1 and Synonym arg2 (Expecting False)") {
        modifiesStore.setModifiesStore({{1, "x"}, {2, "y"}, {3, "z"}});
        // Test when both arguments have no variable modifies relationship
        ClauseArgument* integerArg1 = new Integer("3");
        ClauseArgument* synonymArg2 = new Synonym(DesignEntityType::VARIABLE, "w");
        REQUIRE_FALSE(modifiesStore.hasStatementVariableModifiesRelationship(*integerArg1, *synonymArg2));

        delete integerArg1;
        delete synonymArg2;
    }
}
