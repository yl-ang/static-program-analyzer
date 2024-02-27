#include "WhileNode.h"

void WhileNode::accept(AstVisitor* visitor) {
    visitor->visitWhile(this);
}

std::shared_ptr<ExpressionNode> WhileNode::getCond() {
    auto exprNodePtr = std::dynamic_pointer_cast<ExpressionNode>(this->getChildren().at(0));
    return exprNodePtr;
}

std::shared_ptr<StatementListNode> WhileNode::getStmtLstNode() {
    auto stmtLstNodePtr = std::dynamic_pointer_cast<StatementListNode>(this->getChildren().at(1));
    return stmtLstNodePtr;
}
