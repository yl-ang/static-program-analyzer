#include "WhileNode.h"

#include <utility>

void WhileNode::accept(AstVisitor* visitor) {
    visitor->visitWhile(this);
}

std::shared_ptr<ExpressionNode> WhileNode::getCond() {
    const std::vector<std::shared_ptr<ASTNode>>& _children = this->getChildren();
    auto exprNodePtr = dynamic_cast<ExpressionNode*>(_children.at(0).get());
    std::shared_ptr<ExpressionNode> exprUniquePtr = std::shared_ptr<ExpressionNode>(exprNodePtr);
    return exprUniquePtr;
}

std::shared_ptr<StatementListNode> WhileNode::getStmtLstNode() {
    const std::vector<std::shared_ptr<ASTNode>>& _children = this->getChildren();
    auto stmtLstNodePtr = dynamic_cast<StatementListNode*>(_children.at(1).get());
    std::shared_ptr<StatementListNode> stmtLstUniquePtr = std::shared_ptr<StatementListNode>(stmtLstNodePtr);
    return stmtLstUniquePtr;
}
