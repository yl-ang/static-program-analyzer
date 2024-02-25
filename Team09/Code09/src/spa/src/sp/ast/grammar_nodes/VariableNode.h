#pragma once

#include <string>

#include "sp/ast/grammar_nodes/FactorNode.h"

class VariableNode : public FactorNode {
public:
    explicit VariableNode(std::string value, int stmtNumber) : FactorNode(value, "var", stmtNumber) {}
    void accept(AstVisitor* visitor) override {
        visitor->visitVariable(this);
    }
};
