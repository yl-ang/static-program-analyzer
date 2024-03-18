#include <memory>

#include "sp/ast/grammar_nodes/ExpressionNode.h"
#include "string"
// Tree serialising and deserialising adapted from the following solutions:
// https://leetcode.com/problems/serialize-and-deserialize-binary-tree/solutions/74259/recursive-preorder-python-and-c-o-n/
class PatternTreeNode {
public:
    std::string value;
    std::shared_ptr<PatternTreeNode> left;
    std::shared_ptr<PatternTreeNode> right;
    PatternTreeNode(std::string value) : value(value), left(NULL), right(NULL) {}
    PatternTreeNode();
    static PatternTreeNode buildTreeFromString(std::string value);
    std::string serialiseToString(std::shared_ptr<PatternTreeNode> node);
    static std::shared_ptr<PatternTreeNode> deserializeToNode(std::string);

    friend bool operator==(const PatternTreeNode& lhs, const PatternTreeNode& rhs) {
        // TODO(ben): verify this equality works
        return lhs.value == rhs.value && lhs.left == rhs.left && lhs.left == rhs.left;
    }

private:
    static PatternTreeNode buildTreeFromAST(std::shared_ptr<ExpressionNode> expressoinNode);
    void serializeHelper(std::shared_ptr<PatternTreeNode> node, std::ostringstream& out);
    static std::shared_ptr<PatternTreeNode> deserializeHelper(std::istringstream& in);
};
