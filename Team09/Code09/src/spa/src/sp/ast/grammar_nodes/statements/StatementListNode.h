#pragma once

#include "sp/ast/AstNode.h"

class StatementListNode : public ASTNode {
public:
    StatementListNode() : ASTNode("", "stmtList") {}
};
