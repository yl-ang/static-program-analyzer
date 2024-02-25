#include "AssignmentNode.h"

void AssignmentNode::accept(AstVisitor* visitor) {
    visitor->visitAssign(this);
}

std::unique_ptr<ExpressionNode> AssignmentNode::getExpr() {
    std::vector<std::unique_ptr<ASTNode>> children = this->getChildren();
    auto exprNodePtr = dynamic_cast<ExpressionNode*>(children.at(1).get());
    std::unique_ptr<ExpressionNode> exprUniquePtr = std::make_unique<ExpressionNode>(*exprNodePtr);
    return exprUniquePtr;
}

std::string AssignmentNode::getVar() {
    return children.at(0)->getValue();
}
