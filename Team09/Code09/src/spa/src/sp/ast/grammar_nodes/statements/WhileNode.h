#pragma once
#include <memory>
#include <vector>

#include "StatementListNode.h"
#include "sp/ast/grammar_nodes/ExpressionNode.h"
#include "sp/ast/grammar_nodes/statements/StatementNode.h"

class WhileNode : public StatementNode {
public:
    explicit WhileNode(std::shared_ptr<ExpressionNode> whileCondition, std::shared_ptr<StatementListNode> whileStmtList,
                       int statementNumber)
        : StatementNode("", "while", statementNumber, {whileCondition, whileStmtList}),
          whileCondition(whileCondition),
          statementNumber(statementNumber),
          whileStmtList(whileStmtList) {}

    void accept(AstVisitor* visitor) override {
        visitor->visitWhile(this);
    }
    std::shared_ptr<ExpressionNode> whileCondition;
    std::shared_ptr<StatementListNode> whileStmtList;
    int statementNumber;
};
