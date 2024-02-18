#pragma once
#include "sp/ast/AstNode.h"
#include "sp/ast/grammar_nodes/statements/StatementNode.h"
class AssignmentNode : public StatementNode {
public:
    AssignmentNode() : StatementNode("", "assign") {}
};
