#pragma once

#include <string>

#include "sp/ast/AstNode.h"
#include "sp/ast/grammar_nodes/FactorNode.h"

class NameNode : public FactorNode {
public:
    explicit NameNode(std::string value) : FactorNode(value, "var") {}
};
