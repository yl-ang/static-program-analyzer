#pragma once
#include "sp/ast/AstNode.h"
#include "sp/ast/grammar_nodes/statements/StatementNode.h"
class ReadNode : public StatementNode {
public:
    ReadNode() : StatementNode("", "read") {}
};
