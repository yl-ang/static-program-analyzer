#include "PrintNode.h"

void PrintNode::accept(AstVisitor* visitor) {
    visitor->visitPrint(this);
}

std::string PrintNode::getVar() {
    std::vector<ASTNode*> children = this->getChildren();
    for (const auto& child : children) {
        if (typeid(child) == typeid(VariableNode)) {
            return child->getValue();
        }
    }
}
