#include "ReadNode.h"

void ReadNode::accept(AstVisitor* visitor) {
    visitor->visitRead(this);
}

std::string ReadNode::getVar() {
    return this->variable->value;
}
