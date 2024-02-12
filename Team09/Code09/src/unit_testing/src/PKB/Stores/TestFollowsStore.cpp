#include "PKB/Stores/FollowsStore.h"
#include "catch.hpp"

TEST_CASE("FollowsStore - Base Test") {
    FollowsStore followsStore;

    SECTION("Test setFollowsStore and getFollower") {
        followsStore.setFollowsStore(1, 2);
        followsStore.setFollowsStore(2, 3);
        followsStore.setFollowsStore(3, 4);
        followsStore.setFollowsStore(4, 5);

        REQUIRE(followsStore.getFollower(1).value_or(-1) == 2);
        REQUIRE(followsStore.getFollower(2).value_or(-1) == 3);
        REQUIRE(followsStore.getFollower(3).value_or(-1) == 4);
        REQUIRE(followsStore.getFollower(4).value_or(-1) == 5);
        // Test for non-existent follower
        REQUIRE(followsStore.getFollower(5).value_or(-1) == -1);
    }

    SECTION("Test setFollowsStore and getFollowee") {
        followsStore.setFollowsStore(1, 2);
        followsStore.setFollowsStore(2, 3);
        followsStore.setFollowsStore(3, 4);
        followsStore.setFollowsStore(4, 5);

        REQUIRE(followsStore.getFollowee(2).value_or(-1) == 1);
        REQUIRE(followsStore.getFollowee(3).value_or(-1) == 2);
        REQUIRE(followsStore.getFollowee(4).value_or(-1) == 3);
        REQUIRE(followsStore.getFollowee(5).value_or(-1) == 4);
        // Test for non-existent followee
        REQUIRE(followsStore.getFollowee(1).value_or(-1) == -1);
    }
}
