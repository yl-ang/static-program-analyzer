#include "PKB/Stores/ParentStore.h"
#include "catch.hpp"

TEST_CASE("ParentStore - All Tests") {
    ParentStore parentStore;

    SECTION("Test getParent") {
        parentStore.setParentStore({{1, 2}, {1, 3}, {1, 4}});
        REQUIRE(parentStore.getParent(2) == 1);
        REQUIRE(parentStore.getParent(3) == 1);
        REQUIRE(parentStore.getParent(4) == 1);
    }

    SECTION("Test getChildren") {
        parentStore.setParentStore({{1, 2}, {1, 3}, {1, 4}});
        REQUIRE(parentStore.getChildren(1) == std::unordered_set<StmtNum>{2, 3, 4});
    }

    SECTION("Test getChildrenStar") {
        parentStore.setParentStore({{1, 2}, {2, 3}, {3, 4}});

        REQUIRE(parentStore.getChildrenStar(1) == std::unordered_set<StmtNum>{2, 3, 4});
        REQUIRE(parentStore.getChildrenStar(2) == std::unordered_set<StmtNum>{3, 4});
        REQUIRE(parentStore.getChildrenStar(3) == std::unordered_set<StmtNum>{4});
        REQUIRE(parentStore.getChildrenStar(4).empty());
    }

    SECTION("Test getParents") {
        parentStore.setParentStore({{1, 2}, {2, 3}, {3, 4}});

        REQUIRE(parentStore.getParent(4) == 3);
        REQUIRE(parentStore.getParent(3) == 2);
        REQUIRE(parentStore.getParent(2) == 1);
    }

    SECTION("Test getParentsStar") {
        parentStore.setParentStore({{1, 2}, {2, 3}, {3, 4}});

        REQUIRE(parentStore.getParentsStar(3) == std::unordered_set<StmtNum>{2, 1});
        REQUIRE(parentStore.getParentsStar(2) == std::unordered_set<StmtNum>{1});
        REQUIRE(parentStore.getParentsStar(1).empty());
    }

    SECTION("Test containsParentRelationship") {
        parentStore.setParentStore({{1, 2}, {3, 4}, {5, 6}});

        REQUIRE(parentStore.hasParentRelationship(1, 2));
        REQUIRE(parentStore.hasParentRelationship(3, 4));
        REQUIRE(parentStore.hasParentRelationship(5, 6));

        REQUIRE_FALSE(parentStore.hasParentRelationship(2, 1));
        REQUIRE_FALSE(parentStore.hasParentRelationship(4, 3));
        REQUIRE_FALSE(parentStore.hasParentRelationship(6, 5));
    }
    SECTION("Test containsParentStarRelationship") {
        parentStore.setParentStore({{1, 2}, {3, 4}, {4, 5}, {5, 6}});

        REQUIRE(parentStore.hasParentStarRelationship(1, 2));
        REQUIRE(parentStore.hasParentStarRelationship(3, 5));
        REQUIRE_FALSE(parentStore.hasParentStarRelationship(1, 4));
        REQUIRE_FALSE(parentStore.hasParentStarRelationship(2, 1));
    }
}
