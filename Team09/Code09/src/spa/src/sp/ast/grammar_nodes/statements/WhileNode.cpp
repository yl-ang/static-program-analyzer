#include "WhileNode.h"

void WhileNode::accept(AstVisitor* visitor) {
    visitor->visitWhile(this);
}

std::unique_ptr<ExpressionNode> WhileNode::getCond() {
    auto exprNodePtr = dynamic_cast<ExpressionNode*>(children.at(0).get());
    std::unique_ptr<ExpressionNode> exprUniquePtr = std::make_unique<ExpressionNode>(*exprNodePtr);
    return exprUniquePtr;
}

std::unique_ptr<StatementListNode> WhileNode::getStmtLstNode() {
    auto stmtLstNodePtr = dynamic_cast<StatementListNode*>(children.at(1).get());
    std::unique_ptr<StatementListNode> stmtLstUniquePtr = std::make_unique<StatementListNode>(*stmtLstNodePtr);
    return stmtLstUniquePtr;
}
