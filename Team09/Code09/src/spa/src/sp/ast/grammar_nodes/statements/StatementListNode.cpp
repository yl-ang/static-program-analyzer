#include "StatementListNode.h"

#include "StatementNode.h"
void StatementListNode::accept(AstVisitor* visitor) {
    visitor->visitStmtLst(this);
}

std::vector<int> StatementListNode::getStmtsStmtNum() {
    std::vector<int> stmtNums;
    const std::vector<std::shared_ptr<StatementNode>>& _children = this->children;
    for (const auto& child : _children) {
        stmtNums.push_back(child->statementNumber);
    }
    return stmtNums;
}

int StatementListNode::firstChildStatementNumber() {
    return this->children[0]->statementNumber;
}
