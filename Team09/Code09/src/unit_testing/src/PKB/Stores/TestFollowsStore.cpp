#include "PKB/Stores/FollowsStore.h"
#include "catch.hpp"

TEST_CASE("FollowsStore - Base Test") {
    FollowsStore followsStore;

    SECTION("Test setFollowsStore and getFollower/getFollowee") {
        followsStore.setFollowsStore({{1, 2}, {3, 4}, {5, 6}});

        REQUIRE(followsStore.getFollower(1) == 2);
        REQUIRE(followsStore.getFollower(3) == 4);
        REQUIRE(followsStore.getFollower(5) == 6);

        REQUIRE(followsStore.getFollowee(2) == 1);
        REQUIRE(followsStore.getFollowee(4) == 3);
        REQUIRE(followsStore.getFollowee(6) == 5);
    }

    SECTION("Test getFolloweesStar and getFollowersStar") {
        followsStore.setFollowsStore({{1, 2}, {2, 3}, {3, 4}});

        REQUIRE(followsStore.getFolloweesStar(1) ==
                std::unordered_set<StmtNum>{2, 3, 4});
        REQUIRE(followsStore.getFolloweesStar(2) ==
                std::unordered_set<StmtNum>{3, 4});
        REQUIRE(followsStore.getFolloweesStar(3) ==
                std::unordered_set<StmtNum>{4});
        REQUIRE(followsStore.getFolloweesStar(4).empty());

        REQUIRE(followsStore.getFollowersStar(1).empty());
        REQUIRE(followsStore.getFollowersStar(2) ==
                std::unordered_set<StmtNum>{1});
        REQUIRE(followsStore.getFollowersStar(3) ==
                std::unordered_set<StmtNum>{1, 2});
        REQUIRE(followsStore.getFollowersStar(4) ==
                std::unordered_set<StmtNum>{1, 2, 3});
    }

    SECTION("Test setFollowsStore with single relationship") {
        followsStore.setFollowsStore({{1, 2}});
        REQUIRE(followsStore.getFollower(1) == 2);
        REQUIRE(followsStore.getFollowee(2) == 1);
        REQUIRE(followsStore.getFolloweesStar(1) ==
                std::unordered_set<StmtNum>{2});
        REQUIRE(followsStore.getFolloweesStar(2).empty());
        REQUIRE(followsStore.getFollowersStar(1).empty());
        REQUIRE(followsStore.getFollowersStar(2) ==
                std::unordered_set<StmtNum>{1});
        REQUIRE(followsStore.containsFollowRelationship(1, 2));
        REQUIRE_FALSE(followsStore.containsFollowRelationship(2, 1));
        REQUIRE(followsStore.containsFollowStarRelationship(1, 2));
        REQUIRE_FALSE(followsStore.containsFollowStarRelationship(2, 1));
    }

    SECTION(
        "Test containsFollowRelationship and containsFollowStarRelationship") {
        followsStore.setFollowsStore({{1, 2}, {3, 4}, {4, 5}, {5, 6}});

        REQUIRE(followsStore.containsFollowRelationship(1, 2));
        REQUIRE(followsStore.containsFollowRelationship(3, 4));
        REQUIRE(followsStore.containsFollowRelationship(5, 6));

        REQUIRE_FALSE(followsStore.containsFollowRelationship(2, 1));
        REQUIRE_FALSE(followsStore.containsFollowRelationship(4, 3));
        REQUIRE_FALSE(followsStore.containsFollowRelationship(6, 5));

        REQUIRE(followsStore.containsFollowStarRelationship(1, 2));
        REQUIRE(followsStore.containsFollowStarRelationship(3, 5));
        REQUIRE_FALSE(followsStore.containsFollowStarRelationship(1, 4));
        REQUIRE_FALSE(followsStore.containsFollowStarRelationship(2, 1));
    }
}
