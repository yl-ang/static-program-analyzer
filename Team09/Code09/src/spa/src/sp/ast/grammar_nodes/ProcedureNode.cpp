#include "ProcedureNode.h"

void ProcedureNode::accept(AstVisitor* visitor) {
    visitor->visitProcedure(this);
}

std::shared_ptr<StatementListNode> ProcedureNode::getStmtLstNode() {
    return this->statements;
}
