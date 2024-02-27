#include "AssignmentNode.h"

void AssignmentNode::accept(AstVisitor* visitor) {
    visitor->visitAssign(this);
}

std::shared_ptr<ExpressionNode> AssignmentNode::getExpr() {
    auto exprNodePtr = std::dynamic_pointer_cast<ExpressionNode>(this->getChildren().at(1));
    return exprNodePtr;
}

std::string AssignmentNode::getVar() {
    return this->getChildren().at(0)->getValue();
}
