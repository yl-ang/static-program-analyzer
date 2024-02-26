#include "WhileNode.h"

#include <utility>

void WhileNode::accept(AstVisitor* visitor) {
    visitor->visitWhile(this);
}

std::unique_ptr<ExpressionNode> WhileNode::getCond() {
    const std::vector<std::unique_ptr<ASTNode>>& _children = this->getChildren();
    auto exprNodePtr = dynamic_cast<ExpressionNode*>(_children.at(0).get());
    std::unique_ptr<ExpressionNode> exprUniquePtr = std::unique_ptr<ExpressionNode>(exprNodePtr);
    return exprUniquePtr;
}

std::unique_ptr<StatementListNode> WhileNode::getStmtLstNode() {
    const std::vector<std::unique_ptr<ASTNode>>& _children = this->getChildren();
    auto stmtLstNodePtr = dynamic_cast<StatementListNode*>(_children.at(1).get());
    std::unique_ptr<StatementListNode> stmtLstUniquePtr = std::unique_ptr<StatementListNode>(stmtLstNodePtr);
    return stmtLstUniquePtr;
}
