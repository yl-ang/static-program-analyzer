#include <functional>

#include "catch.hpp"
#include "pkb/Stores/WhilePatternStore.h"

TEST_CASE("WhilePatternStore - All Tests") {
    WhilePatternStore whilePatternStore;
    std::unordered_set<std::pair<StmtNum, std::string>> patterns = {{1, "x"}, {2, "y"}, {3, "z"}, {1, "q"}};
    whilePatternStore.setWhilePatternStore(patterns);

    SECTION("Test return true for hasWhilePattern - (StmtNum, Var)") {
        REQUIRE(whilePatternStore.hasWhilePattern(1, "x"));
        REQUIRE(whilePatternStore.hasWhilePattern(1, "q"));
        REQUIRE(whilePatternStore.hasWhilePattern(2, "y"));
        REQUIRE(whilePatternStore.hasWhilePattern(3, "z"));
    }

    SECTION("Test always return false for hasWhilePattern - (StmtNum, Var)") {
        REQUIRE_FALSE(whilePatternStore.hasWhilePattern(1, "a"));
        REQUIRE_FALSE(whilePatternStore.hasWhilePattern(2, "b"));
        REQUIRE_FALSE(whilePatternStore.hasWhilePattern(3, "c"));
        REQUIRE_FALSE(whilePatternStore.hasWhilePattern(4, "d"));
    }
}
