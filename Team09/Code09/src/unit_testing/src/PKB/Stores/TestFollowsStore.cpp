#include "catch.hpp"
#include "pkb/Stores/FollowsStore.h"
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

TEST_CASE("FollowsStore - Test for issue #260") {
    FollowsStore followsStore;

    // Provided follows pairs based on issue 260
    std::unordered_set<std::pair<StmtNum, StmtNum>> followsPairs = {
        {41, 42}, {37, 38}, {40, 43}, {45, 46}, {13, 14}, {9, 10},  {19, 48}, {1, 2},   {5, 6},
        {34, 35}, {32, 33}, {14, 15}, {12, 13}, {10, 11}, {2, 3},   {4, 5},   {6, 7},   {7, 8},
        {35, 36}, {11, 12}, {3, 4},   {8, 18},  {26, 27}, {24, 25}, {20, 21}, {16, 17}, {18, 19},
        {31, 32}, {15, 16}, {29, 30}, {21, 22}, {22, 45}, {23, 24}, {27, 44}, {28, 40}};

    followsStore.setFollowsStore(followsPairs);

    SECTION("Test getFollowersStar") {
        REQUIRE(followsStore.getFollowersStar(1) == std::unordered_set<StmtNum>{2, 3, 4, 5, 6, 7, 8, 48, 18, 19});
        REQUIRE(followsStore.getFollowersStar(2) == std::unordered_set<StmtNum>{3, 4, 5, 6, 7, 8, 48, 18, 19});
        REQUIRE(followsStore.getFollowersStar(3) == std::unordered_set<StmtNum>{4, 5, 6, 7, 8, 48, 18, 19});
        REQUIRE(followsStore.getFollowersStar(4) == std::unordered_set<StmtNum>{5, 6, 7, 8, 48, 18, 19});
        REQUIRE(followsStore.getFollowersStar(5) == std::unordered_set<StmtNum>{6, 7, 8, 48, 18, 19});
        REQUIRE(followsStore.getFollowersStar(6) == std::unordered_set<StmtNum>{7, 8, 48, 18, 19});
        REQUIRE(followsStore.getFollowersStar(7) == std::unordered_set<StmtNum>{8, 48, 18, 19});
        REQUIRE(followsStore.getFollowersStar(8) == std::unordered_set<StmtNum>{48, 18, 19});
        REQUIRE(followsStore.getFollowersStar(9) == std::unordered_set<StmtNum>{10, 11, 12, 13, 14, 15, 16, 17});
        REQUIRE(followsStore.getFollowersStar(10) == std::unordered_set<StmtNum>{11, 12, 13, 14, 15, 16, 17});
        REQUIRE(followsStore.getFollowersStar(11) == std::unordered_set<StmtNum>{12, 13, 14, 15, 16, 17});
        REQUIRE(followsStore.getFollowersStar(12) == std::unordered_set<StmtNum>{13, 14, 15, 16, 17});
        REQUIRE(followsStore.getFollowersStar(13) == std::unordered_set<StmtNum>{14, 15, 16, 17});
        REQUIRE(followsStore.getFollowersStar(14) == std::unordered_set<StmtNum>{15, 16, 17});
        REQUIRE(followsStore.getFollowersStar(15) == std::unordered_set<StmtNum>{16, 17});
        REQUIRE(followsStore.getFollowersStar(16) == std::unordered_set<StmtNum>{17});
        REQUIRE(followsStore.getFollowersStar(17).empty());
        REQUIRE(followsStore.getFollowersStar(18) == std::unordered_set<StmtNum>{19, 48});
        REQUIRE(followsStore.getFollowersStar(19) == std::unordered_set<StmtNum>{48});
        REQUIRE(followsStore.getFollowersStar(20) == std::unordered_set<StmtNum>{21, 22, 45, 46});
        REQUIRE(followsStore.getFollowersStar(21) == std::unordered_set<StmtNum>{46, 22, 45});
        REQUIRE(followsStore.getFollowersStar(22) == std::unordered_set<StmtNum>{45, 46});
        REQUIRE(followsStore.getFollowersStar(23) == std::unordered_set<StmtNum>{24, 25});
        REQUIRE(followsStore.getFollowersStar(24) == std::unordered_set<StmtNum>{25});
        REQUIRE(followsStore.getFollowersStar(25).empty());
        REQUIRE(followsStore.getFollowersStar(26) == std::unordered_set<StmtNum>{27, 44});
        REQUIRE(followsStore.getFollowersStar(27) == std::unordered_set<StmtNum>{44});
        REQUIRE(followsStore.getFollowersStar(28) == std::unordered_set<StmtNum>{40, 43});
        REQUIRE(followsStore.getFollowersStar(29) == std::unordered_set<StmtNum>{30});
        REQUIRE(followsStore.getFollowersStar(30).empty());
        REQUIRE(followsStore.getFollowersStar(31) == std::unordered_set<StmtNum>{32, 33});
        REQUIRE(followsStore.getFollowersStar(32) == std::unordered_set<StmtNum>{33});
        REQUIRE(followsStore.getFollowersStar(33).empty());
        REQUIRE(followsStore.getFollowersStar(34) == std::unordered_set<StmtNum>{35, 36});
        REQUIRE(followsStore.getFollowersStar(35) == std::unordered_set<StmtNum>{36});
        REQUIRE(followsStore.getFollowersStar(36).empty());
        REQUIRE(followsStore.getFollowersStar(37) == std::unordered_set<StmtNum>{38});
        REQUIRE(followsStore.getFollowersStar(38).empty());
        REQUIRE(followsStore.getFollowersStar(39).empty());
        REQUIRE(followsStore.getFollowersStar(40) == std::unordered_set<StmtNum>{43});
        REQUIRE(followsStore.getFollowersStar(41) == std::unordered_set<StmtNum>{42});
        REQUIRE(followsStore.getFollowersStar(42).empty());
        REQUIRE(followsStore.getFollowersStar(43).empty());
        REQUIRE(followsStore.getFollowersStar(44).empty());
        REQUIRE(followsStore.getFollowersStar(45) == std::unordered_set<StmtNum>{46});
    }

    SECTION("Test getFolloweesStar") {
        REQUIRE(followsStore.getFolloweesStar(1).empty());
        REQUIRE(followsStore.getFolloweesStar(2) == std::unordered_set<StmtNum>{1});
        REQUIRE(followsStore.getFolloweesStar(3) == std::unordered_set<StmtNum>{2, 1});
        REQUIRE(followsStore.getFolloweesStar(4) == std::unordered_set<StmtNum>{3, 2, 1});
        REQUIRE(followsStore.getFolloweesStar(5) == std::unordered_set<StmtNum>{4, 3, 2, 1});
        REQUIRE(followsStore.getFolloweesStar(6) == std::unordered_set<StmtNum>{5, 4, 3, 2, 1});
        REQUIRE(followsStore.getFolloweesStar(7) == std::unordered_set<StmtNum>{6, 5, 4, 3, 2, 1});
        REQUIRE(followsStore.getFolloweesStar(8) == std::unordered_set<StmtNum>{7, 6, 5, 4, 3, 2, 1});
        REQUIRE(followsStore.getFolloweesStar(9).empty());
        REQUIRE(followsStore.getFolloweesStar(10) == std::unordered_set<StmtNum>{9});
        REQUIRE(followsStore.getFolloweesStar(11) == std::unordered_set<StmtNum>{10, 9});
        REQUIRE(followsStore.getFolloweesStar(12) == std::unordered_set<StmtNum>{11, 10, 9});
        REQUIRE(followsStore.getFolloweesStar(13) == std::unordered_set<StmtNum>{12, 11, 10, 9});
        REQUIRE(followsStore.getFolloweesStar(14) == std::unordered_set<StmtNum>{13, 12, 11, 10, 9});
        REQUIRE(followsStore.getFolloweesStar(15) == std::unordered_set<StmtNum>{14, 13, 12, 11, 10, 9});
        REQUIRE(followsStore.getFolloweesStar(16) == std::unordered_set<StmtNum>{15, 14, 13, 12, 11, 10, 9});
        REQUIRE(followsStore.getFolloweesStar(17) == std::unordered_set<StmtNum>{16, 15, 14, 13, 12, 11, 10, 9});
        REQUIRE(followsStore.getFolloweesStar(18) == std::unordered_set<StmtNum>{8, 7, 6, 5, 4, 3, 2, 1});
        REQUIRE(followsStore.getFolloweesStar(19) == std::unordered_set<StmtNum>{2, 18, 8, 7, 6, 5, 4, 3, 1});
        REQUIRE(followsStore.getFolloweesStar(20).empty());
        REQUIRE(followsStore.getFolloweesStar(21) == std::unordered_set<StmtNum>{20});
        REQUIRE(followsStore.getFolloweesStar(22) == std::unordered_set<StmtNum>{21, 20});
        REQUIRE(followsStore.getFolloweesStar(23).empty());
        REQUIRE(followsStore.getFolloweesStar(24) == std::unordered_set<StmtNum>{23});
        REQUIRE(followsStore.getFolloweesStar(25) == std::unordered_set<StmtNum>{24, 23});
        REQUIRE(followsStore.getFolloweesStar(26).empty());
        REQUIRE(followsStore.getFolloweesStar(27) == std::unordered_set<StmtNum>{26});
        REQUIRE(followsStore.getFolloweesStar(28).empty());
        REQUIRE(followsStore.getFolloweesStar(29).empty());
        REQUIRE(followsStore.getFolloweesStar(30) == std::unordered_set<StmtNum>{29});
        REQUIRE(followsStore.getFolloweesStar(31).empty());
        REQUIRE(followsStore.getFolloweesStar(32) == std::unordered_set<StmtNum>{31});
        REQUIRE(followsStore.getFolloweesStar(33) == std::unordered_set<StmtNum>{32, 31});
        REQUIRE(followsStore.getFolloweesStar(34).empty());
        REQUIRE(followsStore.getFolloweesStar(35) == std::unordered_set<StmtNum>{34});
        REQUIRE(followsStore.getFolloweesStar(36) == std::unordered_set<StmtNum>{35, 34});
        REQUIRE(followsStore.getFolloweesStar(37).empty());
        REQUIRE(followsStore.getFolloweesStar(38) == std::unordered_set<StmtNum>{37});
        REQUIRE(followsStore.getFolloweesStar(39).empty());
        REQUIRE(followsStore.getFolloweesStar(40) == std::unordered_set<StmtNum>{28});
        REQUIRE(followsStore.getFolloweesStar(41).empty());
        REQUIRE(followsStore.getFolloweesStar(42) == std::unordered_set<StmtNum>{41});
        REQUIRE(followsStore.getFolloweesStar(43) == std::unordered_set<StmtNum>{40, 28});
        REQUIRE(followsStore.getFolloweesStar(44) == std::unordered_set<StmtNum>{27, 26});
        REQUIRE(followsStore.getFolloweesStar(45) == std::unordered_set<StmtNum>{21, 22, 20});
        REQUIRE(followsStore.getFolloweesStar(46) == std::unordered_set<StmtNum>{21, 45, 22, 20});
        REQUIRE(followsStore.getFolloweesStar(47).empty());
        REQUIRE(followsStore.getFolloweesStar(48) == std::unordered_set<StmtNum>{3, 19, 18, 8, 7, 6, 5, 4, 2, 1});
    }
}
