#include "ReadNode.h"

void ReadNode::accept(AstVisitor* visitor) override {
    visitor->visitRead(this);
}

std::string ReadNode::getVar() {
    return children.at(0)->getValue();
}
