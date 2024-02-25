#include "PrintNode.h"

void PrintNode::accept(AstVisitor* visitor) {
    visitor->visitPrint(this);
}

std::string PrintNode::getVar() {
    std::vector<std::unique_ptr<ASTNode>> children = this->getChildren();
    return children.at(0)->getValue();
}
