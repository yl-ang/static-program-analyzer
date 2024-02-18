#pragma once

#include <string>

#include "sp/ast/AstNode.h"
#include "sp/ast/grammar_nodes/TermNode.h"
/*
Covers var_name, parenthesis, and const_value
*/
class FactorNode : public TermNode {
public:
    explicit FactorNode(LEXICAL_TOKEN_TYPE type) : TermNode(type) {}
    FactorNode(std::string value, std::string type) : TermNode(value, type) {}
};
