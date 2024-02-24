#include "PKB/Stores/ParentStore.h"
#include "catch.hpp"
#include "qps/clauseArguments/Integer.h"
#include "qps/clauseArguments/Wildcard.h"

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

    SECTION("Test hasParentStarRelationship") {
        parentStore.setParentStore({{1, 2}, {3, 4}, {5, 6}});

        REQUIRE(parentStore.hasParentRelationship(1, 2));
        REQUIRE(parentStore.hasParentRelationship(3, 4));
        REQUIRE(parentStore.hasParentRelationship(5, 6));

        REQUIRE_FALSE(parentStore.hasParentRelationship(2, 1));
        REQUIRE_FALSE(parentStore.hasParentRelationship(4, 3));
        REQUIRE_FALSE(parentStore.hasParentRelationship(6, 5));
    }
    SECTION("Test hasParentStarRelationship") {
        parentStore.setParentStore({{1, 2}, {3, 4}, {4, 5}, {5, 6}});

        REQUIRE(parentStore.hasParentStarRelationship(1, 2));
        REQUIRE(parentStore.hasParentStarRelationship(3, 5));
        REQUIRE_FALSE(parentStore.hasParentStarRelationship(1, 4));
        REQUIRE_FALSE(parentStore.hasParentStarRelationship(2, 1));
    }

    SECTION("Test hasParentRelationship with Wildcards arg1 and arg2, non-empty parentStore (Expecting True)") {
        parentStore.setParentStore({{1, 2}, {3, 4}, {5, 6}});
        ClauseArgument* wildcardArg1 = new Wildcard();
        ClauseArgument* wildcardArg2 = new Wildcard();
        REQUIRE(parentStore.hasParentRelationship(*wildcardArg1, *wildcardArg2));

        delete wildcardArg1;
        delete wildcardArg2;
    }

    SECTION("Test hasParentRelationship with Wildcards arg1 and arg2, empty parentStore (Expecting False)") {
        parentStore.setParentStore({});
        ClauseArgument* wildcardArg1 = new Wildcard();
        ClauseArgument* wildcardArg2 = new Wildcard();
        REQUIRE_FALSE(parentStore.hasParentRelationship(*wildcardArg1, *wildcardArg2));

        delete wildcardArg1;
        delete wildcardArg2;
    }

    SECTION("Test hasParentRelationship with Wildcard arg1 and Integer arg2 (Expecting True)") {
        parentStore.setParentStore({{1, 2}, {3, 4}, {5, 6}});

        // Argument 1 is wildcard
        // Argument 2 is integer with parent (1)
        ClauseArgument* wildcardArg1 = new Wildcard();
        ClauseArgument* integerArg2 = new Integer("2");
        REQUIRE(parentStore.hasParentRelationship(*wildcardArg1, *integerArg2));

        delete wildcardArg1;
        delete integerArg2;
    }

    SECTION("Test hasParentRelationship with Wildcard arg1 and Integer arg2 (Expecting False)") {
        parentStore.setParentStore({{1, 2}, {3, 4}, {5, 6}});

        // Argument 1 is wildcard
        // Argument 2 is integer with no parent
        ClauseArgument* wildcardArg1 = new Wildcard();
        ClauseArgument* integerArg2 = new Integer("3");
        REQUIRE_FALSE(parentStore.hasParentRelationship(*wildcardArg1, *integerArg2));

        delete wildcardArg1;
        delete integerArg2;
    }

    SECTION("Test hasParentRelationship with Integer arg1 and Wildcard arg2 (Expecting True)") {
        parentStore.setParentStore({{1, 2}, {3, 4}, {5, 6}});

        // Argument 1 is integer and has child (2)
        // Argument 2 is wildcard
        ClauseArgument* integerArg1 = new Integer("1");
        ClauseArgument* wildcardArg2 = new Wildcard();
        REQUIRE(parentStore.hasParentRelationship(*integerArg1, *wildcardArg2));

        delete integerArg1;
        delete wildcardArg2;
    }

    SECTION("Test hasParentRelationship with Integer arg1 and Wildcard arg2 (Expecting false)") {
        parentStore.setParentStore({{1, 2}, {3, 4}, {5, 6}});

        // Test argument 1 is integer, but has no children
        // Test argument 2 is wildcard
        ClauseArgument* integerArg1 = new Integer("6");
        ClauseArgument* wildcardArg2 = new Wildcard();
        REQUIRE_FALSE(parentStore.hasParentRelationship(*integerArg1, *wildcardArg2));

        delete integerArg1;
        delete wildcardArg2;
    }

    SECTION("Test hasParentRelationship with Integer arg1 and Integer arg2 (Expecting true)") {
        parentStore.setParentStore({{1, 2}, {3, 4}, {5, 6}});

        // Test when both arguments are integers and have parent-child relationship
        ClauseArgument* integerArg1 = new Integer("1");
        ClauseArgument* integerArg2 = new Integer("2");
        REQUIRE(parentStore.hasParentRelationship(*integerArg1, *integerArg2));

        delete integerArg1;
        delete integerArg2;
    }

    SECTION("Test hasParentRelationship with Integer arg1 and Integer arg2 (Expecting False)") {
        parentStore.setParentStore({{1, 2}, {3, 4}, {5, 6}});

        // Test when both arguments are integers and have no parent-child relationship
        ClauseArgument* integerArg1 = new Integer("1");
        ClauseArgument* integerArg2 = new Integer("6");
        REQUIRE_FALSE(parentStore.hasParentRelationship(*integerArg1, *integerArg2));

        delete integerArg1;
        delete integerArg2;
    }
}
