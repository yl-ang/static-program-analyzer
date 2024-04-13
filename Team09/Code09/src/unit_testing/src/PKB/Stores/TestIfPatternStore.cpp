#include <functional>

#include "catch.hpp"
#include "pkb/Stores/IfPatternStore.h"

TEST_CASE("IfPatternStore - All Tests") {
    IfPatternStore ifPatternStore;
    std::unordered_set<std::pair<StmtNum, std::string>> patterns = {{1, "x"}, {2, "y"}, {3, "z"}, {1, "q"}};
    ifPatternStore.setIfPatternStore(patterns);

    SECTION("Test return true for hasIfPattern - (StmtNum, Var)") {
        REQUIRE(ifPatternStore.hasIfPattern(1, "x"));
        REQUIRE(ifPatternStore.hasIfPattern(1, "q"));
        REQUIRE(ifPatternStore.hasIfPattern(2, "y"));
        REQUIRE(ifPatternStore.hasIfPattern(3, "z"));
    }

    SECTION("Test always return false for hasIfPattern - (StmtNum, Var)") {
        REQUIRE_FALSE(ifPatternStore.hasIfPattern(1, "a"));
        REQUIRE_FALSE(ifPatternStore.hasIfPattern(2, "b"));
        REQUIRE_FALSE(ifPatternStore.hasIfPattern(3, "c"));
        REQUIRE_FALSE(ifPatternStore.hasIfPattern(4, "d"));
    }
}
