#pragma once
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "sp/ast/AstNode.h"
#include "sp/ast/grammar_nodes/ProcedureNode.h"

class ProcedureNode : public ASTNode {
public:
    explicit ProcedureNode(std::string name, std::vector<std::unique_ptr<ASTNode>> children)
        : ASTNode(name, "proc", std::move(children), -1) {}

    void accept(AstVisitor* visitor) override {
        visitor->visitProcedure(this);
    }
};
