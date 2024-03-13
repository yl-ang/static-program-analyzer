#pragma once

#include <memory>
#include <vector>

#include "sp/ast/AstNode.h"
#include "sp/ast/grammar_nodes/ProcedureNode.h"
class ProgramNode : public ASTNode {
public:
    explicit ProgramNode(std::vector<std::shared_ptr<ProcedureNode>> children)
        : ASTNode("main", "prog", std::vector<std::shared_ptr<ASTNode>>(children.begin(), children.end()), -1),
          children(children) {}

    void accept(AstVisitor* visitor) override {
        visitor->visitProgram(this);
    }
    std::vector<std::shared_ptr<ProcedureNode>> children;
};
