#include "AssignmentNode.h"

void AssignmentNode::accept(AstVisitor* visitor) {
    visitor->visitAssign(this);
}

std::shared_ptr<ExpressionNode> AssignmentNode::getExpr() {
    const std::vector<std::shared_ptr<ASTNode>>& _children = this->getChildren();
    auto exprNodePtr = dynamic_cast<ExpressionNode*>(_children.at(1).get());
    std::shared_ptr<ExpressionNode> exprUniquePtr = std::shared_ptr<ExpressionNode>(exprNodePtr);
    return exprUniquePtr;
}

std::string AssignmentNode::getVar() {
    const std::vector<std::shared_ptr<ASTNode>>& _children = this->getChildren();
    return _children.at(0)->getValue();
}
