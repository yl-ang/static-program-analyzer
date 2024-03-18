

#include <memory>

#include "catch.hpp"
#include "sp/PatternTreeNode.h"
using namespace std;  // NOLINT

TEST_CASE("PatternTreeNode tests") {
    SECTION("Test building from string") {
        std::string input = "x+1";
        auto expected = std::make_shared<PatternTreeNode>("add");
        auto left = std::make_shared<PatternTreeNode>("x");
        auto right = std::make_shared<PatternTreeNode>("1");
        expected->left = left;
        expected->right = right;

        auto result = PatternTreeNode::buildTreeFromString(input);
        REQUIRE(PatternTreeNode::isEqual(expected, result));
    }
}
