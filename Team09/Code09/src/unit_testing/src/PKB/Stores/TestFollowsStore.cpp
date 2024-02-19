#include <unordered_set>

#include "PKB/Stores/FollowsStore.h"
#include "catch.hpp"

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
        auto result2 = followsStore.getFolloweesStar(2);
        std::unordered_set<StmtNum> expected2 = {1};
        REQUIRE(result2 == expected2);

        auto result3 = followsStore.getFolloweesStar(3);
        std::unordered_set<StmtNum> expected3 = {1, 2};
        REQUIRE(result3 == expected3);

        auto result4 = followsStore.getFolloweesStar(4);
        std::unordered_set<StmtNum> expected4 = {1, 2, 3};
        REQUIRE(result4 == expected4);
    }

    SECTION("Test getFollowersStar") {
        followsStore.setFollowsStore({{1, 2}, {2, 3}, {3, 4}});

        auto result1 = followsStore.getFollowersStar(1);
        std::unordered_set<StmtNum> expected1 = {2, 3, 4};
        REQUIRE(result1 == expected1);

        auto result2 = followsStore.getFollowersStar(2);
        std::unordered_set<StmtNum> expected2 = {3, 4};
        REQUIRE(result2 == expected2);

        auto result3 = followsStore.getFollowersStar(3);
        std::unordered_set<StmtNum> expected3 = {4};
        REQUIRE(result3 == expected3);

        auto result4 = followsStore.getFollowersStar(4);
        std::unordered_set<StmtNum> expected4;
        REQUIRE(result4 == expected4);
    }

    SECTION("Test containsFollowRelationship") {
        followsStore.setFollowsStore({{1, 2}, {3, 4}, {5, 6}});

        REQUIRE(followsStore.containsFollowRelationship(1, 2));
        REQUIRE(followsStore.containsFollowRelationship(3, 4));
        REQUIRE(followsStore.containsFollowRelationship(5, 6));

        REQUIRE_FALSE(followsStore.containsFollowRelationship(2, 1));
        REQUIRE_FALSE(followsStore.containsFollowRelationship(4, 3));
        REQUIRE_FALSE(followsStore.containsFollowRelationship(6, 5));
    }

    SECTION("Test containsFollowStarRelationship") {
        followsStore.setFollowsStore({{1, 2}, {3, 4}, {4, 5}, {5, 6}});

        REQUIRE(followsStore.containsFollowStarRelationship(1, 2));
        REQUIRE(followsStore.containsFollowStarRelationship(3, 5));
        REQUIRE_FALSE(followsStore.containsFollowStarRelationship(1, 4));
        REQUIRE_FALSE(followsStore.containsFollowStarRelationship(2, 1));
    }
}
