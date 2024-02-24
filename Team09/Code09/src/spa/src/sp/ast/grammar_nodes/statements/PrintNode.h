#pragma once
#include "sp/ast/AstNode.h"
#include "sp/ast/grammar_nodes/statements/StatementNode.h"
class PrintNode : public StatementNode {
public:
    PrintNode() : StatementNode("", "print") {}

    void accept(AstVisitor* visitor) override {
        visitor->visitPrint(this);
    }
};
