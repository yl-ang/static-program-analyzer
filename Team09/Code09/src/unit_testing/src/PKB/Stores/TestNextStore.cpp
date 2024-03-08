#include "PKB/Stores/NextStore.h"
#include "catch.hpp"
#include "qps/clauseArguments/Integer.h"
#include "qps/clauseArguments/Wildcard.h"

TEST_CASE("NextStore - All Tests") {
    NextStore nextStore;

    SECTION("Test setNextStore and getNexter") {
        nextStore.setNextStore({{1, 2}, {1, 3}, {2, 4}, {3, 4}});
        REQUIRE(nextStore.getNexter(1) == std::unordered_set<StmtNum>{2, 3});
    }

    SECTION("Test setNextStore and getNextee") {
        nextStore.setNextStore({{1, 2}, {1, 3}, {2, 4}, {3, 4}});
        REQUIRE(nextStore.getNextee(4) == std::unordered_set<StmtNum>{2, 3});
    }

    //    SECTION("Test hasNextRelationship") {
    //        nextStore.setNextStore({{1, 2}, {2, 3}, {3, 4}});
    //        REQUIRE(nextStore.hasNextRelationship(1, 2));
    //        REQUIRE_FALSE(nextStore.hasNextRelationship(1, 5));
    //    }
    //
    //    SECTION("Test hasNextRelationship with Wildcards arg1 and arg2, non-empty nextStore (Expecting True)") {
    //        nextStore.setNextStore({{1, 2}, {3, 4}, {5, 6}});
    //        ClauseArgument* wildcardArg1 = new Wildcard();
    //        ClauseArgument* wildcardArg2 = new Wildcard();
    //        REQUIRE(nextStore.hasNextRelationship(*wildcardArg1, *wildcardArg2));
    //
    //        delete wildcardArg1;
    //        delete wildcardArg2;
    //    }
    //
    //    SECTION("Test hasNextRelationship with Wildcards arg1 and arg2, empty nextStore (Expecting False)") {
    //        nextStore.setNextStore({});
    //        ClauseArgument* wildcardArg1 = new Wildcard();
    //        ClauseArgument* wildcardArg2 = new Wildcard();
    //        REQUIRE_FALSE(nextStore.hasNextRelationship(*wildcardArg1, *wildcardArg2));
    //
    //        delete wildcardArg1;
    //        delete wildcardArg2;
    //    }
    //
    //    SECTION("Test hasNextRelationship with Wildcard arg1 and Integer arg2 (Expecting True)") {
    //        nextStore.setNextStore({{1, 2}, {3, 4}, {5, 6}});
    //        ClauseArgument* wildcardArg1 = new Wildcard();
    //        ClauseArgument* integerArg2 = new Integer("2");
    //        REQUIRE(nextStore.hasNextRelationship(*wildcardArg1, *integerArg2));
    //
    //        delete wildcardArg1;
    //        delete integerArg2;
    //    }
    //
    //    SECTION("Test hasNextRelationship with Wildcard arg1 and Integer arg2 (Expecting False)") {
    //        nextStore.setNextStore({{1, 2}, {3, 4}, {5, 6}});
    //        ClauseArgument* wildcardArg1 = new Wildcard();
    //        ClauseArgument* integerArg2 = new Integer("3");
    //        REQUIRE_FALSE(nextStore.hasNextRelationship(*wildcardArg1, *integerArg2));
    //
    //        delete wildcardArg1;
    //        delete integerArg2;
    //
    //
    //    SECTION("Test hasNextRelationship with Integer arg1 and Wildcard arg2 (Expecting True)") {
    //        nextStore.setNextStore({{1, 2}, {3, 4}, {5, 6}});
    //        ClauseArgument* integerArg1 = new Integer("1");
    //        ClauseArgument* wildcardArg2 = new Wildcard();
    //        REQUIRE(nextStore.hasNextRelationship(*integerArg1, *wildcardArg2));
    //
    //        delete integerArg1;
    //        delete wildcardArg2;
    //    }
    //
    //    SECTION("Test hasNextRelationship with Integer arg1 and Wildcard arg2 (Expecting False)") {
    //        nextStore.setNextStore({{1, 2}, {3, 4}, {5, 6}});
    //        ClauseArgument* integerArg1 = new Integer("6");
    //        ClauseArgument* wildcardArg2 = new Wildcard();
    //        REQUIRE(nextStore.hasNextRelationship(*integerArg1, *wildcardArg2));
    //
    //        delete integerArg1;
    //        delete wildcardArg2;
    //    }
    //
    //    SECTION("Test hasNextRelationship with Integer arg1 and Integer arg2 (Expecting True)") {
    //        nextStore.setNextStore({{1, 2}, {3, 4}, {5, 6}});
    //        ClauseArgument* integerArg1 = new Integer("1");
    //        ClauseArgument* integerArg2 = new Integer("2");
    //        REQUIRE(nextStore.hasNextRelationship(*integerArg1, *integerArg2));
    //
    //        delete integerArg1;
    //        delete integerArg2;
    //    }
    //
    //    SECTION("Test hasNextRelationship with Integer arg1 and Integer arg2 (Expecting False)") {
    //        nextStore.setNextStore({{1, 2}, {3, 4}, {5, 6}});
    //        ClauseArgument* integerArg1 = new Integer("1");
    //        ClauseArgument* integerArg2 = new Integer("6");
    //        REQUIRE_FALSE(nextStore.hasNextRelationship(*integerArg1, *integerArg2));
    //
    //        delete integerArg1;
    //        delete integerArg2;
    //    }
}
