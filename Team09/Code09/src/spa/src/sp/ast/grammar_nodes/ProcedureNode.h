#pragma once

#include <string>

#include "sp/ast/AstNode.h"

class ProcedureNode : public ASTNode {
public:
    explicit ProcedureNode(std::string name) : ASTNode(name, "proc") {}
    void accept(AstVisitor* visitor) override;
};
