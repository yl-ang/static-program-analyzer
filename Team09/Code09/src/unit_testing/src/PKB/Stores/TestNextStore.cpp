#include "PKB/Stores/NextStore.h"
#include "catch.hpp"
#include "qps/clauseArguments/Integer.h"
#include "qps/clauseArguments/Wildcard.h"

TEST_CASE("NextStore - Next Tests") {
    NextStore nextStore;

    SECTION("Test setNextStore and getNexter") {
        nextStore.setNextStore({{1, 2}, {1, 3}, {2, 4}, {3, 4}});
        REQUIRE(nextStore.getNexter(1) == std::unordered_set<StmtNum>{2, 3});
    }

    SECTION("Test setNextStore and getNextee") {
        nextStore.setNextStore({{1, 2}, {1, 3}, {2, 4}, {3, 4}});
        REQUIRE(nextStore.getNextee(4) == std::unordered_set<StmtNum>{2, 3});
    }

    SECTION("Test hasNextRelationship") {
        nextStore.setNextStore({{1, 2}, {1, 3}, {2, 4}, {3, 4}});
        REQUIRE(nextStore.hasNextRelationship(1, 2));
        REQUIRE(nextStore.hasNextRelationship(3, 4));
        REQUIRE(nextStore.hasNextRelationship(2, 4));
        REQUIRE_FALSE(nextStore.hasNextRelationship(1, 4));
    }

    SECTION("Test hasNextRelationship with Wildcards arg1 and arg2, non-empty nextStore (Expecting True)") {
        nextStore.setNextStore({{1, 2}, {1, 3}, {2, 4}, {3, 4}});
        ClauseArgument *wildcardArg1 = new Wildcard();
        ClauseArgument *wildcardArg2 = new Wildcard();
        REQUIRE(nextStore.hasNextRelationship(*wildcardArg1, *wildcardArg2));

        delete wildcardArg1;
        delete wildcardArg2;
    }

    SECTION("Test hasNextRelationship with Wildcards arg1 and arg2, empty nextStore (Expecting False)") {
        nextStore.setNextStore({});
        ClauseArgument *wildcardArg1 = new Wildcard();
        ClauseArgument *wildcardArg2 = new Wildcard();
        REQUIRE_FALSE(nextStore.hasNextRelationship(*wildcardArg1, *wildcardArg2));

        delete wildcardArg1;
        delete wildcardArg2;
    }

    SECTION("Test hasNextRelationship with Wildcard arg1 and Integer arg2 (Expecting True)") {
        nextStore.setNextStore({{1, 2}, {1, 3}, {2, 4}, {3, 4}});
        ClauseArgument *wildcardArg1 = new Wildcard();
        ClauseArgument *integerArg2 = new Integer("4");
        REQUIRE(nextStore.hasNextRelationship(*wildcardArg1, *integerArg2));

        delete wildcardArg1;
        delete integerArg2;
    }

    SECTION("Test hasNextRelationship with Wildcard arg1 and Integer arg2 (Expecting False)") {
        nextStore.setNextStore({{1, 2}, {1, 3}, {2, 4}, {3, 4}});
        ClauseArgument *wildcardArg1 = new Wildcard();
        ClauseArgument *integerArg2 = new Integer("1");
        REQUIRE_FALSE(nextStore.hasNextRelationship(*wildcardArg1, *integerArg2));

        delete wildcardArg1;
        delete integerArg2;
    }

    SECTION("Test hasNextRelationship with Integer arg1 and Wildcard arg2 (Expecting True)") {
        nextStore.setNextStore({{1, 2}, {1, 3}, {2, 4}, {3, 4}});
        ClauseArgument *integerArg1 = new Integer("1");
        ClauseArgument *wildcardArg2 = new Wildcard();
        REQUIRE(nextStore.hasNextRelationship(*integerArg1, *wildcardArg2));

        delete integerArg1;
        delete wildcardArg2;
    }

    SECTION("Test hasNextRelationship with Integer arg1 and Wildcard arg2 (Expecting False)") {
        nextStore.setNextStore({{1, 2}, {1, 3}, {2, 4}, {3, 4}});
        ClauseArgument *integerArg1 = new Integer("4");
        ClauseArgument *wildcardArg2 = new Wildcard();
        REQUIRE_FALSE(nextStore.hasNextRelationship(*integerArg1, *wildcardArg2));

        delete integerArg1;
        delete wildcardArg2;
    }

    SECTION("Test hasNextRelationship with Integer arg1 and Integer arg2 (Expecting True)") {
        nextStore.setNextStore({{1, 2}, {1, 3}, {2, 4}, {3, 4}});
        ClauseArgument *integerArg1 = new Integer("1");
        ClauseArgument *integerArg2 = new Integer("2");
        ClauseArgument *integerArg3 = new Integer("3");
        REQUIRE(nextStore.hasNextRelationship(*integerArg1, *integerArg2));
        REQUIRE(nextStore.hasNextRelationship(*integerArg1, *integerArg3));

        delete integerArg1;
        delete integerArg2;
        delete integerArg3;
    }

    SECTION("Test hasNextRelationship with Integer arg1 and Integer arg2 (Expecting False), no self reflective") {
        nextStore.setNextStore({{1, 2}, {1, 3}, {2, 4}, {3, 4}});
        ClauseArgument *integerArg1 = new Integer("1");
        ClauseArgument *integerArg2 = new Integer("1");
        REQUIRE_FALSE(nextStore.hasNextRelationship(*integerArg1, *integerArg2));

        delete integerArg1;
        delete integerArg2;
    }

    SECTION("Test hasNextRelationship with Integer arg1 and Integer arg2 (Expecting False)") {
        nextStore.setNextStore({{1, 2}, {1, 3}, {2, 4}, {3, 4}});
        ClauseArgument *integerArg1 = new Integer("1");
        ClauseArgument *integerArg2 = new Integer("4");
        REQUIRE_FALSE(nextStore.hasNextRelationship(*integerArg1, *integerArg2));

        delete integerArg1;
        delete integerArg2;
    }

    SECTION("Test hasNextRelationship with Integer arg1 and Integer arg2 (Expecting False)") {
        nextStore.setNextStore({{1, 2}, {3, 4}, {5, 6}});
        ClauseArgument *integerArg1 = new Integer("1");
        ClauseArgument *integerArg2 = new Integer("6");
        REQUIRE_FALSE(nextStore.hasNextRelationship(*integerArg1, *integerArg2));

        delete integerArg1;
        delete integerArg2;
    }
}

TEST_CASE("NextStore - NextStar Tests") {
    NextStore nextStore;
    nextStore.setNextStore(
        {{1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6}, {6, 3}, {3, 7}, {7, 8}, {7, 9}, {8, 10}, {9, 10}, {10, 11}, {11, 12}});

    SECTION("Test hasNextStarRelationship") {
        // Self Reflectivity Checks (3, 4, 5, 6 are in a loop should accept self reflectivity)
        REQUIRE_FALSE(nextStore.hasNextStarRelationship(1, 1));
        REQUIRE_FALSE(nextStore.hasNextStarRelationship(2, 2));
        REQUIRE(nextStore.hasNextStarRelationship(3, 3) == true);
        REQUIRE(nextStore.hasNextStarRelationship(4, 4) == true);
        REQUIRE(nextStore.hasNextStarRelationship(5, 5) == true);
        REQUIRE(nextStore.hasNextStarRelationship(6, 6) == true);
        REQUIRE_FALSE(nextStore.hasNextStarRelationship(7, 7));
        REQUIRE_FALSE(nextStore.hasNextStarRelationship(8, 8));
        REQUIRE_FALSE(nextStore.hasNextStarRelationship(9, 9));
        REQUIRE_FALSE(nextStore.hasNextStarRelationship(10, 10));
        REQUIRE_FALSE(nextStore.hasNextStarRelationship(11, 11));
        REQUIRE_FALSE(nextStore.hasNextStarRelationship(12, 12));

        REQUIRE(nextStore.hasNextStarRelationship(1, 3) == true);
        REQUIRE(nextStore.hasNextStarRelationship(1, 4) == true);
        REQUIRE(nextStore.hasNextStarRelationship(1, 5) == true);
        REQUIRE(nextStore.hasNextStarRelationship(1, 6) == true);
        REQUIRE(nextStore.hasNextStarRelationship(1, 7) == true);
        REQUIRE(nextStore.hasNextStarRelationship(1, 8) == true);
        REQUIRE(nextStore.hasNextStarRelationship(1, 9) == true);
        REQUIRE(nextStore.hasNextStarRelationship(1, 10) == true);
        REQUIRE(nextStore.hasNextStarRelationship(1, 11) == true);
        REQUIRE(nextStore.hasNextStarRelationship(1, 12) == true);

        REQUIRE_FALSE(nextStore.hasNextStarRelationship(2, 1));
        REQUIRE(nextStore.hasNextStarRelationship(2, 3) == true);
        REQUIRE(nextStore.hasNextStarRelationship(2, 4) == true);
        REQUIRE(nextStore.hasNextStarRelationship(2, 5) == true);
        REQUIRE(nextStore.hasNextStarRelationship(2, 6) == true);
        REQUIRE(nextStore.hasNextStarRelationship(2, 7) == true);
        REQUIRE(nextStore.hasNextStarRelationship(2, 8) == true);
        REQUIRE(nextStore.hasNextStarRelationship(2, 9) == true);
        REQUIRE(nextStore.hasNextStarRelationship(2, 10) == true);
        REQUIRE(nextStore.hasNextStarRelationship(2, 11) == true);
        REQUIRE(nextStore.hasNextStarRelationship(2, 12) == true);

        REQUIRE_FALSE(nextStore.hasNextStarRelationship(3, 1));
        REQUIRE_FALSE(nextStore.hasNextStarRelationship(3, 2));
        REQUIRE(nextStore.hasNextStarRelationship(3, 4) == true);
        REQUIRE(nextStore.hasNextStarRelationship(3, 5) == true);
        REQUIRE(nextStore.hasNextStarRelationship(3, 6) == true);
        REQUIRE(nextStore.hasNextStarRelationship(3, 7) == true);
        REQUIRE(nextStore.hasNextStarRelationship(3, 8) == true);
        REQUIRE(nextStore.hasNextStarRelationship(3, 9) == true);
        REQUIRE(nextStore.hasNextStarRelationship(3, 10) == true);
        REQUIRE(nextStore.hasNextStarRelationship(3, 11) == true);
        REQUIRE(nextStore.hasNextStarRelationship(3, 12) == true);

        REQUIRE_FALSE(nextStore.hasNextStarRelationship(4, 1));
        REQUIRE_FALSE(nextStore.hasNextStarRelationship(4, 2));
        REQUIRE(nextStore.hasNextStarRelationship(4, 3) == true);
        REQUIRE(nextStore.hasNextStarRelationship(4, 5) == true);
        REQUIRE(nextStore.hasNextStarRelationship(4, 6) == true);
        REQUIRE(nextStore.hasNextStarRelationship(4, 7) == true);
        REQUIRE(nextStore.hasNextStarRelationship(4, 8) == true);
        REQUIRE(nextStore.hasNextStarRelationship(4, 9) == true);
        REQUIRE(nextStore.hasNextStarRelationship(4, 10) == true);
        REQUIRE(nextStore.hasNextStarRelationship(4, 11) == true);
        REQUIRE(nextStore.hasNextStarRelationship(4, 12) == true);

        REQUIRE_FALSE(nextStore.hasNextStarRelationship(5, 1));
        REQUIRE_FALSE(nextStore.hasNextStarRelationship(5, 2));
        REQUIRE(nextStore.hasNextStarRelationship(5, 3) == true);
        REQUIRE(nextStore.hasNextStarRelationship(5, 4) == true);
        REQUIRE(nextStore.hasNextStarRelationship(5, 6) == true);
        REQUIRE(nextStore.hasNextStarRelationship(5, 7) == true);
        REQUIRE(nextStore.hasNextStarRelationship(5, 8) == true);
        REQUIRE(nextStore.hasNextStarRelationship(5, 9) == true);
        REQUIRE(nextStore.hasNextStarRelationship(5, 10) == true);
        REQUIRE(nextStore.hasNextStarRelationship(5, 11) == true);
        REQUIRE(nextStore.hasNextStarRelationship(5, 12) == true);

        REQUIRE_FALSE(nextStore.hasNextStarRelationship(6, 1));
        REQUIRE_FALSE(nextStore.hasNextStarRelationship(6, 2));
        REQUIRE(nextStore.hasNextStarRelationship(6, 3) == true);
        REQUIRE(nextStore.hasNextStarRelationship(6, 4) == true);
        REQUIRE(nextStore.hasNextStarRelationship(6, 5) == true);
        REQUIRE(nextStore.hasNextStarRelationship(6, 7) == true);
        REQUIRE(nextStore.hasNextStarRelationship(6, 8) == true);
        REQUIRE(nextStore.hasNextStarRelationship(6, 9) == true);
        REQUIRE(nextStore.hasNextStarRelationship(6, 10) == true);
        REQUIRE(nextStore.hasNextStarRelationship(6, 11) == true);
        REQUIRE(nextStore.hasNextStarRelationship(6, 12) == true);

        REQUIRE_FALSE(nextStore.hasNextStarRelationship(7, 1));
        REQUIRE_FALSE(nextStore.hasNextStarRelationship(7, 2));
        REQUIRE_FALSE(nextStore.hasNextStarRelationship(7, 3));
        REQUIRE_FALSE(nextStore.hasNextStarRelationship(7, 4));
        REQUIRE_FALSE(nextStore.hasNextStarRelationship(7, 5));
        REQUIRE_FALSE(nextStore.hasNextStarRelationship(7, 6));
        REQUIRE(nextStore.hasNextStarRelationship(7, 8) == true);
        REQUIRE(nextStore.hasNextStarRelationship(7, 9) == true);
        REQUIRE(nextStore.hasNextStarRelationship(7, 10) == true);
        REQUIRE(nextStore.hasNextStarRelationship(7, 11) == true);
        REQUIRE(nextStore.hasNextStarRelationship(7, 12) == true);

        REQUIRE_FALSE(nextStore.hasNextStarRelationship(8, 1));
        REQUIRE_FALSE(nextStore.hasNextStarRelationship(8, 2));
        REQUIRE_FALSE(nextStore.hasNextStarRelationship(8, 3));
        REQUIRE_FALSE(nextStore.hasNextStarRelationship(8, 4));
        REQUIRE_FALSE(nextStore.hasNextStarRelationship(8, 5));
        REQUIRE_FALSE(nextStore.hasNextStarRelationship(8, 6));
        REQUIRE_FALSE(nextStore.hasNextStarRelationship(8, 7));
        REQUIRE_FALSE(nextStore.hasNextStarRelationship(8, 9));
        REQUIRE(nextStore.hasNextStarRelationship(8, 10) == true);
        REQUIRE(nextStore.hasNextStarRelationship(8, 11) == true);
        REQUIRE(nextStore.hasNextStarRelationship(8, 12) == true);

        REQUIRE_FALSE(nextStore.hasNextStarRelationship(12, 1));
        REQUIRE_FALSE(nextStore.hasNextStarRelationship(12, 2));
        REQUIRE_FALSE(nextStore.hasNextStarRelationship(12, 3));
        REQUIRE_FALSE(nextStore.hasNextStarRelationship(12, 4));
        REQUIRE_FALSE(nextStore.hasNextStarRelationship(12, 5));
        REQUIRE_FALSE(nextStore.hasNextStarRelationship(12, 6));
        REQUIRE_FALSE(nextStore.hasNextStarRelationship(12, 7));
        REQUIRE_FALSE(nextStore.hasNextStarRelationship(12, 8));
        REQUIRE_FALSE(nextStore.hasNextStarRelationship(12, 9));
        REQUIRE_FALSE(nextStore.hasNextStarRelationship(12, 10));
        REQUIRE_FALSE(nextStore.hasNextStarRelationship(12, 11));
        REQUIRE_FALSE(nextStore.hasNextStarRelationship(12, 12));
    }
}
