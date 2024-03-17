#include <memory>

#include "sp/ast/grammar_nodes/ExpressionNode.h"
#include "string"

class PatternTreeNode {
    std::string value;
    std::shared_ptr<PatternTreeNode> left;
    std::shared_ptr<PatternTreeNode> right;
    PatternTreeNode(std::string value) : value(value), left(NULL), right(NULL) {}
    PatternTreeNode();
    static PatternTreeNode buildTreeFromString(std::string value);

private:
    static PatternTreeNode buildTreeFromAST(std::shared_ptr<ExpressionNode> expressoinNode);
};
