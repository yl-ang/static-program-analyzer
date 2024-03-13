#include "PrintNode.h"

void PrintNode::accept(AstVisitor* visitor) {
    visitor->visitPrint(this);
}

std::string PrintNode::getVar() {
    return this->variable->value;
}
