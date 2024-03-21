#include <functional>

#include "PKB/Stores/IfPatternStore.h"
#include "catch.hpp"
#include "qps/clauseArguments/Wildcard.h"

bool alwaysTrueIfFunction(std::string s1, std::string s2) {
    return true;
}

bool alwaysFalseIfFunction(std::string s1, std::string s2) {
    return false;
}

TEST_CASE("IfPatternStore - All Tests") {
    IfPatternStore ifPatternStore;

    SECTION("Test always return true for hasExactPattern - (StmtNum, Expr)") {
        std::unordered_set<std::pair<StmtNum, std::string>> patterns = {{1, "x"}, {2, "y"}, {3, "z"}, {1, "q"}};
        std::function<bool(std::string, std::string)> funct_ptr1 = alwaysTrueIfFunction;
        std::function<bool(std::string, std::string)> funct_ptr2 = alwaysFalseIfFunction;

        ifPatternStore.initialiseStore(funct_ptr1, funct_ptr2, patterns);

        std::string arg = "z";
        REQUIRE(ifPatternStore.hasExactPattern(3, arg));
    }

    SECTION("Test always return false for hasExactPattern - (StmtNum, Expr)") {
        std::unordered_set<std::pair<StmtNum, std::string>> patterns = {{1, "x"}, {2, "y"}, {3, "z"}, {1, "q"}};
        std::function<bool(std::string, std::string)> funct_ptr1 = alwaysFalseIfFunction;
        std::function<bool(std::string, std::string)> funct_ptr2 = alwaysFalseIfFunction;

        ifPatternStore.initialiseStore(funct_ptr1, funct_ptr2, patterns);

        std::string arg = "z";
        REQUIRE_FALSE(ifPatternStore.hasExactPattern(3, arg));
    }

    SECTION("Test always return true for hasPartialPattern - (StmtNum, Expr)") {
        std::unordered_set<std::pair<StmtNum, std::string>> patterns = {{1, "x"}, {2, "y"}, {3, "z"}, {1, "q"}};
        std::function<bool(std::string, std::string)> funct_ptr1 = alwaysTrueIfFunction;
        std::function<bool(std::string, std::string)> funct_ptr2 = alwaysTrueIfFunction;

        ifPatternStore.initialiseStore(funct_ptr1, funct_ptr2, patterns);

        std::string arg = "z";
        REQUIRE(ifPatternStore.hasPartialPattern(1, arg));
    }

    SECTION("Test always return false for hasPartialPattern - (StmtNum, Expr)") {
        std::unordered_set<std::pair<StmtNum, std::string>> patterns = {{1, "x"}, {2, "y"}, {3, "z"}, {1, "q"}};
        std::function<bool(std::string, std::string)> funct_ptr1 = alwaysFalseIfFunction;
        std::function<bool(std::string, std::string)> funct_ptr2 = alwaysFalseIfFunction;

        ifPatternStore.initialiseStore(funct_ptr1, funct_ptr2, patterns);

        std::string arg = "z";
        REQUIRE_FALSE(ifPatternStore.hasPartialPattern(1, arg));
    }
}
