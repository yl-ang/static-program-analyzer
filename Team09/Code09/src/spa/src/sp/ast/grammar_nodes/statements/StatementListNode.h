#pragma once

#include <memory>
#include <vector>

#include "sp/ast/AstNode.h"
#include "sp/ast/grammar_nodes/statements/StatementNode.h"

class StatementListNode : public ASTNode {
public:
    StatementListNode(std::vector<std::shared_ptr<StatementNode>> children)
        : ASTNode("", "stmtList", std::vector<std::shared_ptr<ASTNode>>(children.begin(), children.end()), -1),
          children(children) {}

    std::vector<std::shared_ptr<StatementNode>> children;
    void accept(AstVisitor* visitor) override;

    std::vector<int> getStmtsStmtNum();
    int firstChildStatementNumber();
};
