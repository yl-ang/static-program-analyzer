#include <functional>

#include "PKB/Stores/WhilePatternStore.h"
#include "catch.hpp"
#include "qps/clauseArguments/Wildcard.h"

bool alwaysTrueWhileFunction(std::string s1, std::string s2) {
    return true;
}

bool alwaysFalseWhileFunction(std::string s1, std::string s2) {
    return false;
}

TEST_CASE("WhilePatternStore - All Tests") {
    WhilePatternStore whilePatternStore;

    SECTION("Test always return true for hasExactPattern - (StmtNum, Expr)") {
        std::unordered_set<std::pair<StmtNum, std::string>> patterns = {{1, "x"}, {2, "y"}, {3, "z"}, {1, "q"}};
        std::function<bool(std::string, std::string)> funct_ptr1 = alwaysTrueWhileFunction;
        std::function<bool(std::string, std::string)> funct_ptr2 = alwaysFalseWhileFunction;

        whilePatternStore.initialiseStore(funct_ptr1, funct_ptr2, patterns);

        std::string arg = "z";
        REQUIRE(whilePatternStore.hasExactPattern(3, arg));
    }

    SECTION("Test always return false for hasExactPattern - (StmtNum, Expr)") {
        std::unordered_set<std::pair<StmtNum, std::string>> patterns = {{1, "x"}, {2, "y"}, {3, "z"}, {1, "q"}};
        std::function<bool(std::string, std::string)> funct_ptr1 = alwaysFalseWhileFunction;
        std::function<bool(std::string, std::string)> funct_ptr2 = alwaysFalseWhileFunction;

        whilePatternStore.initialiseStore(funct_ptr1, funct_ptr2, patterns);

        std::string arg = "z";
        REQUIRE_FALSE(whilePatternStore.hasExactPattern(3, arg));
    }

    SECTION("Test always return true for hasPartialPattern - (StmtNum, Expr)") {
        std::unordered_set<std::pair<StmtNum, std::string>> patterns = {{1, "x"}, {2, "y"}, {3, "z"}, {1, "q"}};
        std::function<bool(std::string, std::string)> funct_ptr1 = alwaysTrueWhileFunction;
        std::function<bool(std::string, std::string)> funct_ptr2 = alwaysTrueWhileFunction;

        whilePatternStore.initialiseStore(funct_ptr1, funct_ptr2, patterns);

        std::string arg = "z";
        REQUIRE(whilePatternStore.hasPartialPattern(1, arg));
    }

    SECTION("Test always return false for hasPartialPattern - (StmtNum, Expr)") {
        std::unordered_set<std::pair<StmtNum, std::string>> patterns = {{1, "x"}, {2, "y"}, {3, "z"}, {1, "q"}};
        std::function<bool(std::string, std::string)> funct_ptr1 = alwaysFalseWhileFunction;
        std::function<bool(std::string, std::string)> funct_ptr2 = alwaysFalseWhileFunction;

        whilePatternStore.initialiseStore(funct_ptr1, funct_ptr2, patterns);

        std::string arg = "z";
        REQUIRE_FALSE(whilePatternStore.hasPartialPattern(1, arg));
    }
}
