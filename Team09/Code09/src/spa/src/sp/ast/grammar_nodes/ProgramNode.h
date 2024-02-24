#pragma once

#include <string>
#include <vector>

#include "sp/ast/AstNode.h"
#include "sp/tokenizer/Token.h"
class ProgramNode : public ASTNode {
public:
    ProgramNode() : ASTNode("main", "program") {}

    void accept(AstVisitor* visitor) override {
        visitor->visitProgram(this);
    }
};
