#include "StatementListNode.h"

void StatementListNode::accept(AstVisitor* visitor) {
    visitor->visitStmtLst(this);
}

std::vector<int> StatementListNode::getStmtsStmtNum() {
    std::vector<int> stmtNums;
    std::vector<ASTNode*> stmts = this->getChildren();
    for (const auto& stmt : stmts) {
        stmtNums.push_back(stmt->getStmtNum());
    }
    return stmtNums;
}
