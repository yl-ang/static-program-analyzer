#pragma once
#include <memory>
#include <utility>
#include <vector>

#include "sp/ast/AstNode.h"
#include "sp/ast/grammar_nodes/statements/StatementNode.h"
class IfNode : public StatementNode {
public:
    explicit IfNode(std::vector<std::unique_ptr<ASTNode>> children, int lineNumber)
        : StatementNode("", "if", lineNumber, std::move(children)) {}

    void accept(AstVisitor* visitor) override;

    std::unique_ptr<ExpressionNode> getCond();
    std::unique_ptr<StatementListNode> getThenStmtLstNode();
    std::unique_ptr<StatementListNode> getElseStmtLstNode();
};
