#include "PKB/Stores/ModifiesStore.h"
#include "catch.hpp"

TEST_CASE("ModifiesStore - All Tests") {
    ModifiesStore modifiesStore;

    SECTION("Test hasStatementVariableModifiesRelationship") {
        modifiesStore.setModifiesStore({{1, "x"}, {2, "y"}, {3, "x"}});

        REQUIRE(modifiesStore.hasStatementVariableModifiesRelationship(1, "x"));
        REQUIRE(modifiesStore.hasStatementVariableModifiesRelationship(2, "y"));
        REQUIRE(modifiesStore.hasStatementVariableModifiesRelationship(3, "x"));

        REQUIRE_FALSE(
            modifiesStore.hasStatementVariableModifiesRelationship(1, "y"));
        REQUIRE_FALSE(
            modifiesStore.hasStatementVariableModifiesRelationship(2, "x"));
        REQUIRE_FALSE(
            modifiesStore.hasStatementVariableModifiesRelationship(3, "y"));
    }

    SECTION("Test getVariablesByStatement") {
        modifiesStore.setModifiesStore({{1, "x"}, {2, "y"}, {3, "x"}});

        REQUIRE(modifiesStore.getVariablesByStatement(1) ==
                std::unordered_set<Variable>{"x"});
        REQUIRE(modifiesStore.getVariablesByStatement(2) ==
                std::unordered_set<Variable>{"y"});
        REQUIRE(modifiesStore.getVariablesByStatement(3) ==
                std::unordered_set<Variable>{"x"});
        REQUIRE(modifiesStore.getVariablesByStatement(4).empty());
    }

    SECTION("Test getStatementsByVariable") {
        modifiesStore.setModifiesStore({{1, "x"}, {2, "y"}, {3, "x"}});

        REQUIRE(modifiesStore.getStatementsByVariable("x") ==
                std::unordered_set<StmtNum>{1, 3});
        REQUIRE(modifiesStore.getStatementsByVariable("y") ==
                std::unordered_set<StmtNum>{2});
        REQUIRE(modifiesStore.getStatementsByVariable("z").empty());
    }
}
