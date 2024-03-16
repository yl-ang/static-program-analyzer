#include "IfNode.h"

void IfNode::accept(AstVisitor* visitor) {
    visitor->visitIf(this);
}

std::shared_ptr<ExpressionNode> IfNode::getCond() {
    return this->ifCondition;
}

std::shared_ptr<StatementListNode> IfNode::getThenStmtLstNode() {
    return this->thenStmtList;
}

std::shared_ptr<StatementListNode> IfNode::getElseStmtLstNode() {
    return this->elseStmtList;
}
