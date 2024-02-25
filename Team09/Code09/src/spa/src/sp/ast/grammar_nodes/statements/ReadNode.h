#pragma once
#include <string>
#include <vector>

#include "sp/ast/AstNode.h"
#include "sp/ast/grammar_nodes/statements/StatementNode.h"
class ReadNode : public StatementNode {
public:
    ReadNode() : StatementNode("", "read") {}

    void accept(AstVisitor* visitor) override;

    std::string ReadNode::getVar();
};
