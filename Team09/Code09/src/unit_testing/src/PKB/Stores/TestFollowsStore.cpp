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
        REQUIRE(followsStore.getFolloweesStar(2) ==
                std::unordered_set<StmtNum>{1});
        REQUIRE(followsStore.getFolloweesStar(3) ==
                std::unordered_set<StmtNum>{1, 2});
    }

    SECTION("Test getFollowersStar") {
        followsStore.setFollowsStore({{1, 2}, {2, 3}, {3, 4}});

        REQUIRE(followsStore.getFollowersStar(1) ==
                std::unordered_set<StmtNum>{2, 3, 4});
        REQUIRE(followsStore.getFollowersStar(2) ==
                std::unordered_set<StmtNum>{3, 4});
        REQUIRE(followsStore.getFollowersStar(3) ==
                std::unordered_set<StmtNum>{4});
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
}
