#pragma once
#include <memory>
#include <utility>
#include <vector>

#include "sp/ast/AstNode.h"
#include "sp/ast/grammar_nodes/statements/StatementNode.h"
class AssignmentNode : public StatementNode {
public:
    explicit AssignmentNode(std::vector<std::unique_ptr<ASTNode>> children)
        : StatementNode("", "assign", std::move(children)) {}
};
