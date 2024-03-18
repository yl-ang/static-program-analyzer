#include "PKB/Stores/AssignPatternStore.h"
#include "catch.hpp"
#include "qps/clauseArguments/Synonym.h"
#include "qps/clauseArguments/Wildcard.h"

bool alwaysTrueFunction(std::string s1, std::string s2) {
    return s1 == s2;
}
TEST_CASE("store - All Tests") {
    AssignPatternStore store;

    SECTION("Test always returns true") {
        std::unordered_set<std::pair<StmtNum, std::pair<std::string, std::string>>> patterns = {
            {1, {"x", "y"}}, {2, {"a", "b"}}, {3, {"p", "q"}}};
        std::function<bool(std::string, std::string)> funct_ptr = alwaysTrueFunction;

        store.initialiseStore(funct_ptr, patterns);

        ClauseArgument* wildcardArg1 = new Wildcard();
        ClauseArgument* synonymsArg2 = new Synonym(DesignEntityType::VARIABLE, "y");

        REQUIRE(store.hasPattern(1, *wildcardArg1, *synonymsArg2));

        // Cleanup
        delete wildcardArg1;
        delete synonymsArg2;
    }
}
