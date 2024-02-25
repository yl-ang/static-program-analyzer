#include "AssignmentNode.h"

void AssignmentNode::accept(AstVisitor* visitor) {
    visitor->visitAssign(this);
}

ASTNode* AssignmentNode::getExpr() {
    std::vector<ASTNode*> children = this->getChildren();
    for (const auto& child : children) {
        if (typeid(child) == typeid(ExpressionNode)) {
            return child;
        }
    }
}

ASTNode* AssignmentNode::getVar() {
    std::vector<ASTNode*> children = this->getChildren();
    for (const auto& child : children) {
        if (typeid(child) == typeid(VariableNode)) {
            return child;
        }
    }
}
