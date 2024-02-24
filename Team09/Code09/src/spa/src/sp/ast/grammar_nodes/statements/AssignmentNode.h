#pragma once
#include "sp/ast/grammar_nodes/statements/StatementNode.h"
class AssignmentNode : public StatementNode {
public:
    AssignmentNode() : StatementNode("", "assign") {}

    void accept(AstVisitor* visitor) override {
        visitor->visitAssign(this);
    }
};
