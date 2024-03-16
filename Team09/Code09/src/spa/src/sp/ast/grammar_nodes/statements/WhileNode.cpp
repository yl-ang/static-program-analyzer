#include "WhileNode.h"

void WhileNode::accept(AstVisitor* visitor) {
    visitor->visitWhile(this);
}

std::shared_ptr<ExpressionNode> WhileNode::getCond() {
    return this->whileCondition;
}

std::shared_ptr<StatementListNode> WhileNode::getStmtLstNode() {
    return this->whileStmtList;
}
