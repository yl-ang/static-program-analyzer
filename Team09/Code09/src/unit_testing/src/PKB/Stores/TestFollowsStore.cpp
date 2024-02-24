#include "PKB/Stores/FollowsStore.h"
#include "catch.hpp"
#include "qps/clauseArguments/Integer.h"
#include "qps/clauseArguments/Wildcard.h"

TEST_CASE("FollowsStore - All Tests") {
    FollowsStore followsStore;

    SECTION("Test getFollower") {
        followsStore.setFollowsStore({{1, 2}, {3, 4}, {5, 6}});

        REQUIRE(followsStore.getFollower(1) == 2);
        REQUIRE(followsStore.getFollower(3) == 4);
        REQUIRE(followsStore.getFollower(5) == 6);
    }

    SECTION("Test getFollowee") {
        followsStore.setFollowsStore({{1, 2}, {3, 4}, {5, 6}});

        REQUIRE(followsStore.getFollowee(2) == 1);
        REQUIRE(followsStore.getFollowee(4) == 3);
        REQUIRE(followsStore.getFollowee(6) == 5);
    }

    SECTION("Test getFolloweesStar") {
        followsStore.setFollowsStore({{1, 2}, {2, 3}, {3, 4}});

        REQUIRE(followsStore.getFolloweesStar(1).empty());
        REQUIRE(followsStore.getFolloweesStar(2) == std::unordered_set<StmtNum>{1});
        REQUIRE(followsStore.getFolloweesStar(3) == std::unordered_set<StmtNum>{1, 2});
    }

    SECTION("Test getFollowersStar") {
        followsStore.setFollowsStore({{1, 2}, {2, 3}, {3, 4}});

        REQUIRE(followsStore.getFollowersStar(1) == std::unordered_set<StmtNum>{2, 3, 4});
        REQUIRE(followsStore.getFollowersStar(2) == std::unordered_set<StmtNum>{3, 4});
        REQUIRE(followsStore.getFollowersStar(3) == std::unordered_set<StmtNum>{4});
        REQUIRE(followsStore.getFollowersStar(4).empty());
    }

    SECTION("Test hasFollowRelationship") {
        followsStore.setFollowsStore({{1, 2}, {3, 4}, {5, 6}});

        REQUIRE(followsStore.hasFollowRelationship(1, 2));
        REQUIRE(followsStore.hasFollowRelationship(3, 4));
        REQUIRE(followsStore.hasFollowRelationship(5, 6));

        REQUIRE_FALSE(followsStore.hasFollowRelationship(2, 1));
        REQUIRE_FALSE(followsStore.hasFollowRelationship(4, 3));
        REQUIRE_FALSE(followsStore.hasFollowRelationship(6, 5));
    }

    SECTION("Test hasFollowStarRelationship") {
        followsStore.setFollowsStore({{1, 2}, {3, 4}, {4, 5}, {5, 6}});

        REQUIRE(followsStore.hasFollowStarRelationship(1, 2));
        REQUIRE(followsStore.hasFollowStarRelationship(3, 5));
        REQUIRE_FALSE(followsStore.hasFollowStarRelationship(1, 4));
        REQUIRE_FALSE(followsStore.hasFollowStarRelationship(2, 1));
    }

    SECTION("Test hasFollowRelationship with Wildcards arg1 and arg2, non-empty followStore (Expecting True)") {
        followsStore.setFollowsStore({{1, 2}, {3, 4}, {5, 6}});
        ClauseArgument* wildcardArg1 = new Wildcard();
        ClauseArgument* wildcardArg2 = new Wildcard();
        REQUIRE(followsStore.hasFollowRelationship(*wildcardArg1, *wildcardArg2));

        delete wildcardArg1;
        delete wildcardArg2;
    }

    SECTION("Test hasFollowRelationship with Wildcards arg1 and arg2, empty followStore (Expecting False)") {
        followsStore.setFollowsStore({});
        ClauseArgument* wildcardArg1 = new Wildcard();
        ClauseArgument* wildcardArg2 = new Wildcard();
        REQUIRE_FALSE(followsStore.hasFollowRelationship(*wildcardArg1, *wildcardArg2));

        delete wildcardArg1;
        delete wildcardArg2;
    }
    SECTION("Test hasFollowRelationship with Wildcard arg1 and Integer arg2 (Expecting True)") {
        followsStore.setFollowsStore({{1, 2}, {3, 4}, {5, 6}});

        // Argument 1 is wildcard
        // Argument 2 is integer with followee (1)
        ClauseArgument* wildcardArg1 = new Wildcard();
        ClauseArgument* integerArg2 = new Integer("2");
        REQUIRE(followsStore.hasFollowRelationship(*wildcardArg1, *integerArg2));

        delete wildcardArg1;
        delete integerArg2;
    }

    SECTION("Test hasFollowRelationship with Wildcard arg1 and Integer arg2 (Expecting False)") {
        followsStore.setFollowsStore({{1, 2}, {3, 4}, {5, 6}});

        // Argument 1 is wildcard
        // Argument 2 is integer with no followee
        ClauseArgument* wildcardArg1 = new Wildcard();
        ClauseArgument* integerArg2 = new Integer("3");
        REQUIRE_FALSE(followsStore.hasFollowRelationship(*wildcardArg1, *integerArg2));

        delete wildcardArg1;
        delete integerArg2;
    }

    SECTION("Test hasFollowRelationship with Integer arg1 and Wildcard arg2 (Expecting True)") {
        followsStore.setFollowsStore({{1, 2}, {3, 4}, {5, 6}});

        // Argument 1 is integer and has follower (4)
        // Argument 2 is wildcard
        ClauseArgument* integerArg1 = new Integer("3");
        ClauseArgument* wildcardArg2 = new Wildcard();
        REQUIRE(followsStore.hasFollowRelationship(*integerArg1, *wildcardArg2));

        delete integerArg1;
        delete wildcardArg2;
    }

    SECTION("Test hasFollowRelationship with Integer arg1 and Wildcard arg2 (Expecting false)") {
        followsStore.setFollowsStore({{1, 2}, {3, 4}, {5, 6}});

        // Test argument 1 is integer, but has no followers
        // Test argument 2 is wildcard
        ClauseArgument* integerArg1 = new Integer("6");
        ClauseArgument* wildcardArg2 = new Wildcard();
        REQUIRE_FALSE(followsStore.hasFollowRelationship(*integerArg1, *wildcardArg2));

        delete integerArg1;
        delete wildcardArg2;
    }

    SECTION("Test hasFollowRelationship with Integer arg1 and Integer arg2 (Expecting true)") {
        followsStore.setFollowsStore({{1, 2}, {3, 4}, {5, 6}});

        // Test when both arguments are integers and following each other
        ClauseArgument* integerArg1 = new Integer("1");
        ClauseArgument* integerArg2 = new Integer("2");
        REQUIRE(followsStore.hasFollowRelationship(*integerArg1, *integerArg2));

        delete integerArg1;
        delete integerArg2;
    }

    SECTION("Test hasFollowRelationship with Integer arg1 and Integer arg2 (Expecting False)") {
        followsStore.setFollowsStore({{1, 2}, {3, 4}, {5, 6}});

        // Test when both arguments are integers but not following each other
        ClauseArgument* integerArg1 = new Integer("3");
        ClauseArgument* integerArg2 = new Integer("5");
        REQUIRE_FALSE(followsStore.hasFollowRelationship(*integerArg1, *integerArg2));

        delete integerArg1;
        delete integerArg2;
    }

    SECTION("Test hasFollowStarRelationship with Wildcards arg1 and arg2, non-empty followerStarMap (Expecting True)") {
        followsStore.setFollowsStore({{1, 2}, {3, 4}, {5, 6}});
        ClauseArgument* wildcardArg1 = new Wildcard();
        ClauseArgument* wildcardArg2 = new Wildcard();
        REQUIRE(followsStore.hasFollowStarRelationship(*wildcardArg1, *wildcardArg2));
        delete wildcardArg1;
        delete wildcardArg2;
    }

    SECTION("Test hasFollowStarRelationship with Wildcards arg1 and arg2, empty followerStarMap (Expecting False)") {
        followsStore.setFollowsStore({});
        ClauseArgument* wildcardArg1 = new Wildcard();
        ClauseArgument* wildcardArg2 = new Wildcard();
        REQUIRE_FALSE(followsStore.hasFollowStarRelationship(*wildcardArg1, *wildcardArg2));
        delete wildcardArg1;
        delete wildcardArg2;
    }

    SECTION("Test hasFollowStarRelationship with Wildcard arg1 and Integer arg2 (Expecting True)") {
        followsStore.setFollowsStore({{1, 2}, {2, 3}, {3, 4}, {5, 6}});
        // Argument 1 is wildcard
        // Argument 2 is integer with followeeStars (1, 2, 3)
        ClauseArgument* wildcardArg1 = new Wildcard();
        ClauseArgument* integerArg2 = new Integer("4");
        REQUIRE(followsStore.hasFollowStarRelationship(*wildcardArg1, *integerArg2));
        delete wildcardArg1;
        delete integerArg2;
    }

    SECTION("Test hasFollowStarRelationship with Wildcard arg1 and Integer arg2 (Expecting False)") {
        followsStore.setFollowsStore({{1, 2}, {2, 3}, {5, 6}});
        // Argument 1 is wildcard
        // Argument 2 is integer with no followeeStars
        ClauseArgument* wildcardArg1 = new Wildcard();
        ClauseArgument* integerArg2 = new Integer("1");
        REQUIRE_FALSE(followsStore.hasFollowStarRelationship(*wildcardArg1, *integerArg2));
        delete wildcardArg1;
        delete integerArg2;
    }

    SECTION("Test hasFollowStarRelationship with Integer arg1 and Wildcard arg2 (Expecting True)") {
        followsStore.setFollowsStore({{1, 2}, {3, 4}, {4, 5}, {5, 6}});
        // Argument 1 is integer (3) and has followerStars (4, 5, 6)
        // Argument 2 is wildcard
        ClauseArgument* integerArg1 = new Integer("3");
        ClauseArgument* wildcardArg2 = new Wildcard();
        REQUIRE(followsStore.hasFollowStarRelationship(*integerArg1, *wildcardArg2));
        delete integerArg1;
        delete wildcardArg2;
    }

    SECTION("Test hasFollowStarRelationship with Integer arg1 and Wildcard arg2 (Expecting False)") {
        followsStore.setFollowsStore({{1, 2}, {2, 3}, {5, 6}});
        // Test argument 1 is integer, but has no followersStar
        // Test argument 2 is wildcard
        ClauseArgument* integerArg1 = new Integer("6");
        ClauseArgument* wildcardArg2 = new Wildcard();
        REQUIRE_FALSE(followsStore.hasFollowStarRelationship(*integerArg1, *wildcardArg2));
        delete integerArg1;
        delete wildcardArg2;
    }

    SECTION("Test hasFollowStarRelationship with Integer arg1 and Integer arg2 (Expecting True)") {
        followsStore.setFollowsStore({{1, 2}, {2, 3}, {3, 4}, {5, 6}});
        // Test when both arguments are integers and following each other
        ClauseArgument* integerArg1 = new Integer("1");
        ClauseArgument* integerArg2 = new Integer("4");
        REQUIRE(followsStore.hasFollowStarRelationship(*integerArg1, *integerArg2));
        delete integerArg1;
        delete integerArg2;
    }

    SECTION("Test hasFollowStarRelationship with Integer arg1 and Integer arg2 (Expecting False)") {
        followsStore.setFollowsStore({{1, 2}, {3, 4}, {5, 6}});
        // Test when both arguments are integers but not following each other
        ClauseArgument* integerArg1 = new Integer("1");
        ClauseArgument* integerArg2 = new Integer("6");
        REQUIRE_FALSE(followsStore.hasFollowStarRelationship(*integerArg1, *integerArg2));
        delete integerArg1;
        delete integerArg2;
    }
}
