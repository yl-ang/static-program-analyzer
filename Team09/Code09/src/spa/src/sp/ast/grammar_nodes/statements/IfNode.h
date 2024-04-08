#pragma once
#include <memory>
#include <vector>

#include "StatementListNode.h"
#include "sp/ast/grammar_nodes/ExpressionNode.h"
#include "sp/ast/grammar_nodes/statements/StatementNode.h"

class IfNode : public StatementNode {
public:
    explicit IfNode(std::shared_ptr<ExpressionNode> ifCondition, std::shared_ptr<StatementListNode> thenStmtList,
                    std::shared_ptr<StatementListNode> elseStmtList, int lineNumber)
        : StatementNode("", "if", lineNumber, {ifCondition, thenStmtList, elseStmtList}),
          ifCondition(ifCondition),
          thenStmtList(thenStmtList),
          elseStmtList(elseStmtList),
          statementNumber(lineNumber) {}

    void accept(AstVisitor* visitor) override {
        visitor->visitIf(this);
    }
    int statementNumber;
    std::shared_ptr<ExpressionNode> ifCondition;
    std::shared_ptr<StatementListNode> thenStmtList;
    std::shared_ptr<StatementListNode> elseStmtList;
};
