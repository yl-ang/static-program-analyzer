#include "StatementListNode.h"

void StatementListNode::accept(AstVisitor* visitor) {
    visitor->visitStmtLst(this);
}

std::vector<int> StatementListNode::getStmtsStmtNum() {
    std::vector<int> stmtNums;
    std::vector<std::unique_ptr<ASTNode>> _children = this->getChildren();
    for (const auto& child : _children) {
        stmtNums.push_back(child->getStmtNumber());
    }
    return stmtNums;
}
