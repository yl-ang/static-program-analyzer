#pragma once
#include <memory>
#include <utility>
#include <vector>

#include "sp/ast/AstNode.h"
#include "sp/ast/grammar_nodes/statements/StatementNode.h"
class PrintNode : public StatementNode {
public:
    explicit PrintNode(std::vector<std::unique_ptr<ASTNode>> children)
        : StatementNode("", "print", std::move(children)) {}
};
