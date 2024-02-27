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
    explicit WhileNode(std::vector<std::shared_ptr<ASTNode>> children, int statementNumber)
        : StatementNode("", "while", statementNumber, (children)) {
    }

    void accept(AstVisitor* visitor) override;

    std::shared_ptr<ExpressionNode> getCond();
    std::shared_ptr<StatementListNode> getStmtLstNode();
};
