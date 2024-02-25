#pragma once
#include <vector>

#include "sp/ast/AstNode.h"

class StatementListNode : public ASTNode {
public:
    StatementListNode() : ASTNode("", "stmtList") {}

    void accept(AstVisitor* visitor) override;

    std::vector<int> getStmtsStmtNum();
};
