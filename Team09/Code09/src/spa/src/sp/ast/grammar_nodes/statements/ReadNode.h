#pragma once
#include <memory>
#include <utility>
#include <vector>

#include "sp/ast/AstNode.h"
#include "sp/ast/grammar_nodes/statements/StatementNode.h"
class ReadNode : public StatementNode {
public:
    explicit ReadNode(std::vector<std::unique_ptr<ASTNode>> children, int statementNumber)
        : StatementNode("", "read", statementNumber, std::move(children)) {}
};
