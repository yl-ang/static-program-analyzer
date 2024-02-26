#pragma once
#include <memory>
#include <utility>
#include <vector>

#include "../ExpressionNode.h"
#include "StatementListNode.h"
#include "sp/ast/AstNode.h"
#include "sp/ast/grammar_nodes/statements/StatementNode.h"

class WhileNode : public StatementNode {
public:
    explicit WhileNode(std::vector<std::unique_ptr<ASTNode>> children, int statementNumber)
        : StatementNode("", "while", statementNumber, std::move(children)) {}

    void accept(AstVisitor* visitor) override;

    std::unique_ptr<ExpressionNode> getCond();
    std::unique_ptr<StatementListNode> getStmtLstNode();
};