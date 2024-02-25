#include "StatementListNode.h"

void StatementListNode::accept(AstVisitor* visitor) {
    visitor->visitStmtLst(this);
}

std::vector<int> StatementListNode::getStmtsStmtNum() {
    std::vector<int> stmtNums;
    for (const auto& child : children) {
        stmtNums.push_back(child->getStmtNum());
    }
    return stmtNums;
}
