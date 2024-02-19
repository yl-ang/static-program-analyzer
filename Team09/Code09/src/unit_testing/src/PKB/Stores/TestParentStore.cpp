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
        auto result = parentStore.getChildren(1);
        REQUIRE(std::vector<StmtNum>(result.begin(), result.end()) ==
                std::vector<StmtNum>{2, 3, 4});
    }

    SECTION("Test getChildrenStar") {
        parentStore.setParentStore({{1, 2}, {2, 3}, {3, 4}});

        auto result1 = parentStore.getChildrenStar(1);
        REQUIRE(std::vector<StmtNum>(result1.begin(), result1.end()) ==
                std::vector<StmtNum>{2, 3, 4});

        auto result2 = parentStore.getChildrenStar(2);
        REQUIRE(std::vector<StmtNum>(result2.begin(), result2.end()) ==
                std::vector<StmtNum>{3, 4});

        auto result3 = parentStore.getChildrenStar(3);
        REQUIRE(std::vector<StmtNum>(result3.begin(), result3.end()) ==
                std::vector<StmtNum>{4});

        auto result4 = parentStore.getChildrenStar(4);
        REQUIRE(result4.empty());
    }

    SECTION("Test getParents") {
        parentStore.setParentStore({{1, 2}, {2, 3}, {3, 4}});

        REQUIRE(parentStore.getParent(4) == 3);
        REQUIRE(parentStore.getParent(3) == 2);
        REQUIRE(parentStore.getParent(2) == 1);
    }

    SECTION("Test getParentsStar") {
        parentStore.setParentStore({{1, 2}, {2, 3}, {3, 4}});

        auto result4 = parentStore.getParentsStar(4);
        REQUIRE(std::vector<StmtNum>(result4.begin(), result4.end()) ==
                std::vector<StmtNum>{3, 2, 1});

        auto result3 = parentStore.getParentsStar(3);
        REQUIRE(std::vector<StmtNum>(result3.begin(), result3.end()) ==
                std::vector<StmtNum>{2, 1});

        auto result2 = parentStore.getParentsStar(2);
        REQUIRE(std::vector<StmtNum>(result2.begin(), result2.end()) ==
                std::vector<StmtNum>{1});

        auto result1 = parentStore.getParentsStar(1);
        REQUIRE(result1.empty());
    }

    SECTION("Test containsParentRelationship") {
        parentStore.setParentStore({{1, 2}, {3, 4}, {5, 6}});

        REQUIRE(parentStore.containsParentRelationship(1, 2));
        REQUIRE(parentStore.containsParentRelationship(3, 4));
        REQUIRE(parentStore.containsParentRelationship(5, 6));

        REQUIRE_FALSE(parentStore.containsParentRelationship(2, 1));
        REQUIRE_FALSE(parentStore.containsParentRelationship(4, 3));
        REQUIRE_FALSE(parentStore.containsParentRelationship(6, 5));
    }

    SECTION("Test containsParentStarRelationship") {
        parentStore.setParentStore({{1, 2}, {3, 4}, {4, 5}, {5, 6}});

        REQUIRE(parentStore.containsParentStarRelationship(1, 2));
        REQUIRE(parentStore.containsParentStarRelationship(3, 5));
        REQUIRE_FALSE(parentStore.containsParentStarRelationship(1, 4));
        REQUIRE_FALSE(parentStore.containsParentStarRelationship(2, 1));
    }
}
