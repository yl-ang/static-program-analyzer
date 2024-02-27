#include "IfNode.h"

void IfNode::accept(AstVisitor* visitor) {
    visitor->visitIf(this);
}

std::shared_ptr<ExpressionNode> IfNode::getCond() {
    const std::vector<std::shared_ptr<ASTNode>>& _children = this->getChildren();
    auto exprNodePtr = dynamic_cast<ExpressionNode*>(_children.at(0).get());
    std::shared_ptr<ExpressionNode> exprUniquePtr = std::shared_ptr<ExpressionNode>(exprNodePtr);
    return exprUniquePtr;
}

std::shared_ptr<StatementListNode> IfNode::getThenStmtLstNode() {
    const std::vector<std::shared_ptr<ASTNode>>& _children = this->getChildren();
    auto stmtLstNodePtr = dynamic_cast<StatementListNode*>(_children.at(1).get());
    std::shared_ptr<StatementListNode> stmtLstUniquePtr = std::shared_ptr<StatementListNode>(stmtLstNodePtr);
    return stmtLstUniquePtr;
}

std::shared_ptr<StatementListNode> IfNode::getElseStmtLstNode() {
    const std::vector<std::shared_ptr<ASTNode>>& _children = this->getChildren();
    auto stmtLstNodePtr = dynamic_cast<StatementListNode*>(_children.at(2).get());
    std::shared_ptr<StatementListNode> stmtLstUniquePtr = std::shared_ptr<StatementListNode>(stmtLstNodePtr);
    return stmtLstUniquePtr;
}
