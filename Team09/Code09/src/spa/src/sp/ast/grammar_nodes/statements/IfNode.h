#pragma once
#include <memory>
#include <utility>
#include <vector>

#include "../ExpressionNode.h"
#include "StatementListNode.h"
#include "sp/ast/AstNode.h"
#include "sp/ast/grammar_nodes/statements/StatementNode.h"

class IfNode : public StatementNode {
public:
    explicit IfNode(std::vector<std::shared_ptr<ASTNode>> children, int lineNumber)
        : StatementNode("", "if", lineNumber, (children)) {
    }

    void accept(AstVisitor* visitor) override;

    std::shared_ptr<ExpressionNode> getCond();
    std::shared_ptr<StatementListNode> getThenStmtLstNode();
    std::shared_ptr<StatementListNode> getElseStmtLstNode();
};
