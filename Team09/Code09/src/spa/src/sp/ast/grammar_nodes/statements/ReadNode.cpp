#include "ReadNode.h"

void ReadNode::accept(AstVisitor* visitor) override {
    visitor->visitRead(this);
}

std::string ReadNode::getVar() {
    std::vector<ASTNode*> children = this->getChildren();
    for (const auto& child : children) {
        if (typeid(child) == typeid(VariableNode)) {
            return child->getValue();
        }
    }
}
