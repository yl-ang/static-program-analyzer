#include "CallNode.h"

void CallNode::accept(AstVisitor* visitor) {}

std::string CallNode::getCalledProcedure() {
    return this->getValue();
}
