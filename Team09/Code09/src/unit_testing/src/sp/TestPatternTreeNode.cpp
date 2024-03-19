

#include <memory>

#include "catch.hpp"
#include "sp/PatternTreeNode.h"
using namespace std;  // NOLINT

TEST_CASE("PatternTreeNode tests") {
    SECTION("Test build tree from string") {
        std::string input = "x+1";
        auto expected = std::make_shared<PatternTreeNode>("add");
        auto left = std::make_shared<PatternTreeNode>("x");
        auto right = std::make_shared<PatternTreeNode>("1");
        expected->left = left;
        expected->right = right;

        auto result = PatternTreeNode::buildTreeFromString(input);
        REQUIRE(PatternTreeNode::isEqual(expected, result));
    }

    SECTION("Test serialise Tree to string correctly") {
        auto root = std::make_shared<PatternTreeNode>("add");
        auto left = std::make_shared<PatternTreeNode>("x");
        auto right = std::make_shared<PatternTreeNode>("1");
        root->left = left;
        root->right = right;
        auto result = PatternTreeNode::serialiseToString(root);
        auto expected = "add x # # 1 # # ";
        REQUIRE(result == expected);
    }

    SECTION("Test deserialise Tree to string correctly") {
        auto root = std::make_shared<PatternTreeNode>("sub");
        auto left = std::make_shared<PatternTreeNode>("x");
        auto right = std::make_shared<PatternTreeNode>("1");
        root->left = left;
        root->right = right;
        auto result = PatternTreeNode::deserializeToNode("sub x # # 1 # # ");
        REQUIRE(PatternTreeNode::isEqual(result, root));
    }

    SECTION("Test serialise and deserialise gives back the same tree") {
        auto root = std::make_shared<PatternTreeNode>("sub");
        auto left = std::make_shared<PatternTreeNode>("x");
        auto right = std::make_shared<PatternTreeNode>("1");
        root->left = left;
        root->right = right;
        auto result = PatternTreeNode::deserializeToNode(PatternTreeNode::serialiseToString(root));
        REQUIRE(PatternTreeNode::isEqual(result, root));
    }
    SECTION("Test deserialise and serialise gives back the same string") {
        auto expected = "sub x # # 1 # # ";
        auto result = PatternTreeNode::serialiseToString(PatternTreeNode::deserializeToNode(expected));
        REQUIRE(expected == result);
    }

    SECTION("Test is exact match tree") {
        auto qpsInput = "x+1";
        auto spInput = "add x # # 1 # # ";
        REQUIRE(isExactMatch(spInput, qpsInput));
    }

    SECTION("Test is not exact match tree") {
        auto qpsInput = "x-1";
        auto spInput = "add x # # 1 # # ";
        REQUIRE(!isExactMatch(spInput, qpsInput));
    }

    SECTION("Test is exact match tree using function pter") {
        std::function<bool(std::string, std::string)> funct_ptr = isExactMatch;
        auto qpsInput = "x+1";
        auto spInput = "add x # # 1 # # ";
        REQUIRE(funct_ptr(spInput, qpsInput));
    }

    SECTION("Test isnt exact match tree using function pter") {
        std::function<bool(std::string, std::string)> funct_ptr = isExactMatch;
        auto qpsInput = "x-1";
        auto spInput = "add x # # 1 # # ";
        REQUIRE(!funct_ptr(spInput, qpsInput));
    }

    SECTION("Test partial equality using function pter") {
        std::function<bool(std::string, std::string)> funct_ptr = isPartialMatch;
        auto qpsInput = "x";
        auto spInput = "add x # # 1 # # ";  // x + 1
        REQUIRE(funct_ptr(spInput, qpsInput));
    }

    SECTION("Test partial equality with different AST") {
        std::function<bool(std::string, std::string)> funct_ptr = isPartialMatch;
        auto qpsInput = "y + z";
        auto spInput = "add add x # # y # # z # # ";  // x + y + z
        REQUIRE(funct_ptr(spInput, qpsInput) == false);
    }
}
