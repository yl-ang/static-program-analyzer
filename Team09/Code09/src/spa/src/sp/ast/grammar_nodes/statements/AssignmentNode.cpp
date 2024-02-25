#include "AssignmentNode.h"

void AssignmentNode::accept(AstVisitor* visitor) {
    visitor->visitAssign(this);
}

std::unique_ptr<ExpressionNode> AssignmentNode::getExpr() {
    std::vector<std::unique_ptr<ASTNode>> _children = this->getChildren();
    auto exprNodePtr = dynamic_cast<ExpressionNode*>(_children.at(1).get());
    std::unique_ptr<ExpressionNode> exprUniquePtr = std::make_unique<ExpressionNode>(*exprNodePtr);
    return exprUniquePtr;
}

std::string AssignmentNode::getVar() {
    std::vector<std::unique_ptr<ASTNode>> _children = this->getChildren();
    return _children.at(0)->getValue();
}
