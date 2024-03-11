#include "CallNode.h"

void CallNode::accept(AstVisitor* visitor) {
    visitor->visitCall(this);
}

std::string CallNode::getCalledProcedure() {
    return this->getValue();
}
