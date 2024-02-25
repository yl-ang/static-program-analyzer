#include "ReadNode.h"

void ReadNode::accept(AstVisitor* visitor) override {
    visitor->visitRead(this);
}

std::string ReadNode::getVar() {
    std::vector<std::unique_ptr<ASTNode>> children = this->getChildren();
    return children.at(0)->getValue();
}
