#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "sp/ast/AstNode.h"
#include "sp/tokenizer/Token.h"
class ProgramNode : public ASTNode {
public:
    explicit ProgramNode(std::vector<std::unique_ptr<ASTNode>> children)
        : ASTNode("main", "program", std::move(children), -1) {}
};
