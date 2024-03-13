#pragma once
#include <memory>
#include <vector>

#include "../ExpressionNode.h"
#include "StatementListNode.h"
#include "sp/ast/grammar_nodes/statements/StatementNode.h"

class WhileNode : public StatementNode {
public:
    explicit WhileNode(std::shared_ptr<ExpressionNode> whileCondition, std::shared_ptr<StatementListNode> whileStmtList,
                       int statementNumber)
        : StatementNode("", "while", statementNumber, {whileCondition, whileStmtList}),
          whileCondition(whileCondition),
          whileStmtList(whileStmtList) {}

    void accept(AstVisitor* visitor) override;
    std::shared_ptr<ExpressionNode> whileCondition;
    std::shared_ptr<StatementListNode> whileStmtList;

    std::shared_ptr<ExpressionNode> getCond();
    std::shared_ptr<StatementListNode> getStmtLstNode();
};
