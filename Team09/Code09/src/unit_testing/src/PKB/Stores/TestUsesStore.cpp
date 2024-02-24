#include "PKB/Stores/UsesStore.h"
#include "catch.hpp"

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

        REQUIRE(usesStore.getVariablesByStatement(1) ==
                std::unordered_set<Variable>{"x"});
        REQUIRE(usesStore.getVariablesByStatement(2) ==
                std::unordered_set<Variable>{"y"});
        REQUIRE(usesStore.getVariablesByStatement(3) ==
                std::unordered_set<Variable>{"x"});
        REQUIRE(usesStore.getVariablesByStatement(4).empty());
    }

    SECTION("Test getStatementsByVariable") {
        usesStore.setUsesStore({{1, "x"}, {2, "y"}, {3, "x"}});

        REQUIRE(usesStore.getStatementsByVariable("x") ==
                std::unordered_set<StmtNum>{1, 3});
        REQUIRE(usesStore.getStatementsByVariable("y") ==
                std::unordered_set<StmtNum>{2});
        REQUIRE(usesStore.getStatementsByVariable("z").empty());
    }
}
