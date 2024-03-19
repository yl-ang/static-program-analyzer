#include <functional>

#include "PKB/Stores/AssignPatternStore.h"
#include "catch.hpp"
#include "qps/clauseArguments/Synonym.h"
#include "qps/clauseArguments/Wildcard.h"

bool alwaysTrueFunction(std::string s1, std::string s2) {
    return s1 == s2;
}

bool alwaysFalseFunction(std::string s1, std::string s2) {
    return s1 != s2;
}

TEST_CASE("store - All Tests") {
    AssignPatternStore store;

    SECTION("Test always returns true") {
        std::unordered_set<std::pair<StmtNum, std::pair<std::string, std::string>>> patterns = {
            {1, {"x", "y + 1"}}, {2, {"a", "x * y * z"}}, {3, {"p", "q + 1 + x + y"}}};
        std::function<bool(std::string, std::string)> funct_ptr1 = alwaysTrueFunction;
        std::function<bool(std::string, std::string)> funct_ptr2 = alwaysFalseFunction;

        store.initialiseStore(funct_ptr1, funct_ptr2, patterns);

        std::string arg1 = "p";
        std::string arg2 = "q + 1 + x + y";

        REQUIRE(store.hasExactPattern(3, arg1, arg2));
    }

    SECTION("Test always returns true") {
        std::unordered_set<std::pair<StmtNum, std::pair<std::string, std::string>>> patterns = {
            {1, {"x", "y + 6"}}, {2, {"a", "b"}}, {3, {"p", "q"}}};
        std::function<bool(std::string, std::string)> funct_ptr1 = alwaysTrueFunction;
        std::function<bool(std::string, std::string)> funct_ptr2 = alwaysFalseFunction;

        store.initialiseStore(funct_ptr1, funct_ptr2, patterns);

        std::string arg1 = "x";
        std::string arg2 = "y";

        REQUIRE_FALSE(store.hasPartialPattern(1, arg1, arg2));
    }
}
