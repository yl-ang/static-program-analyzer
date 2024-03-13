#pragma once
#include <memory>
#include <vector>

#include "../ExpressionNode.h"
#include "StatementListNode.h"
#include "sp/ast/grammar_nodes/statements/StatementNode.h"

class IfNode : public StatementNode {
public:
    explicit IfNode(std::shared_ptr<ExpressionNode> ifCondition, std::shared_ptr<StatementListNode> thenStmtList,
                    std::shared_ptr<StatementListNode> elseStmtList, int lineNumber)
        : StatementNode("", "if", lineNumber, {ifCondition, thenStmtList, elseStmtList}),
          ifCondition(ifCondition),
          thenStmtList(thenStmtList),
          elseStmtList(elseStmtList) {}

    void accept(AstVisitor* visitor) override;

    std::shared_ptr<ExpressionNode> ifCondition;
    std::shared_ptr<StatementListNode> thenStmtList;
    std::shared_ptr<StatementListNode> elseStmtList;

    std::shared_ptr<ExpressionNode> getCond();
    std::shared_ptr<StatementListNode> getThenStmtLstNode();
    std::shared_ptr<StatementListNode> getElseStmtLstNode();
};
