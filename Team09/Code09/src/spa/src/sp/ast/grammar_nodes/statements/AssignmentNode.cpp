#include "AssignmentNode.h"

void AssignmentNode::accept(AstVisitor* visitor) {
    visitor->visitAssign(this);
}

std::shared_ptr<ExpressionNode> AssignmentNode::getExpr() {
    return this->expression;
}

std::string AssignmentNode::getVar() {
    return this->variable->value;
}
