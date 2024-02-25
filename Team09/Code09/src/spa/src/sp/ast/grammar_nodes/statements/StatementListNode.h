#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "sp/ast/AstNode.h"

class StatementListNode : public ASTNode {
public:
    explicit StatementListNode(std::vector<std::unique_ptr<ASTNode>> children)
        : ASTNode("", "stmtList", std::move(children), -1) {}

    void accept(AstVisitor* visitor) override;

    std::vector<int> getStmtsStmtNum();
};
