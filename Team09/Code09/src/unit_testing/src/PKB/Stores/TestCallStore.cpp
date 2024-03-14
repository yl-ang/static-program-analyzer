#include "PKB/Stores/CallStore.h"
#include "catch.hpp"
#include "qps/clauseArguments/Synonym.h"
#include "qps/clauseArguments/Wildcard.h"

TEST_CASE("CallStore - Call Tests") {
    CallStore callStore;
    callStore.setCallStore({{"First", "Second"},
                            {"First", "Third"},
                            {"Second", "Third"},
                            {"Third", "Fouth"},
                            {"Second", "Fouth"},
                            {"Second", "First"}});

    SECTION("Test getCaller") {
        REQUIRE(callStore.getCaller("First") == std::unordered_set<Procedure>{"Second"});
        REQUIRE(callStore.getCaller("Second") == std::unordered_set<Procedure>{"First"});
        REQUIRE(callStore.getCaller("Third") == std::unordered_set<Procedure>{"First", "Second"});
        REQUIRE(callStore.getCaller("Fouth") == std::unordered_set<Procedure>{"Second", "Third"});
    }

    SECTION("Test getCallee") {
        REQUIRE(callStore.getCallee("First") == std::unordered_set<Procedure>{"Second", "Third"});
        REQUIRE(callStore.getCallee("Second") == std::unordered_set<Procedure>{"Third", "Fouth", "First"});
        REQUIRE(callStore.getCallee("Third") == std::unordered_set<Procedure>{"Fouth"});
        REQUIRE(callStore.getCallee("Fouth").empty());
    }
}
