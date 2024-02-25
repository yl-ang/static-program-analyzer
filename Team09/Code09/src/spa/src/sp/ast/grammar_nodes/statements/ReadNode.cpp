#include "ReadNode.h"

void ReadNode::accept(AstVisitor* visitor) {
    visitor->visitRead(this);
}

std::string ReadNode::getVar() {
    std::vector<std::unique_ptr<ASTNode>> _children = this->getChildren();
    return _children.at(0)->getValue();
}
