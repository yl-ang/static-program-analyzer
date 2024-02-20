#include "PKB/Stores/ModifiesStore.h"
#include "catch.hpp"

TEST_CASE("ModifiesStore - All Tests") {
    ModifiesStore modifiesStore;

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
}
