#pragma once

#include <string>

#include "sp/ast/grammar_nodes/FactorNode.h"

class VariableNode : public FactorNode {
public:
    explicit VariableNode(std::string value) : FactorNode(value, "var") {}
    void accept(AstVisitor* visitor) override;
};
