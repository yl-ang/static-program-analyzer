#include "IfNode.h"

void IfNode::accept(AstVisitor* visitor) {
    visitor->visitIf(this);
}

std::shared_ptr<ExpressionNode> IfNode::getCond() {
    auto exprNodePtr = std::dynamic_pointer_cast<ExpressionNode>(this->getChildren().at(0));
    return exprNodePtr;
}

std::shared_ptr<StatementListNode> IfNode::getThenStmtLstNode() {
    auto stmtLstNodePtr = std::dynamic_pointer_cast<StatementListNode>(this->getChildren().at(1));
    return stmtLstNodePtr;
}

std::shared_ptr<StatementListNode> IfNode::getElseStmtLstNode() {
    auto stmtLstNodePtr = std::dynamic_pointer_cast<StatementListNode>(this->getChildren().at(2));
    return stmtLstNodePtr;
}
