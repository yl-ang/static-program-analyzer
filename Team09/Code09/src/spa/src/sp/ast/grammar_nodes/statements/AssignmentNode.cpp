#include "AssignmentNode.h"

void AssignmentNode::accept(AstVisitor* visitor) {
    visitor->visitAssign(this);
}

ExpressionNode* AssignmentNode::getExpr() {
    std::vector<ASTNode*> children = this->getChildren();
    for (const auto& child : children) {
        if (typeid(child) == typeid(ExpressionNode)) {
            return child;
        }
    }
}

std::string AssignmentNode::getVar() {
    std::vector<ASTNode*> children = this->getChildren();
    for (const auto& child : children) {
        if (typeid(child) == typeid(VariableNode)) {
            return child->getValue();
        }
    }
}
