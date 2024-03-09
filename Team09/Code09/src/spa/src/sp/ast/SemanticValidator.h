#pragma once
#include <memory>

#include "sp/ast/grammar_nodes/ProgramNode.h"

class SemanticValidator {
public:
    void validate(std::shared_ptr<ProgramNode> programNode);

private:
    bool hasDuplicateProcedureName(std::shared_ptr<ProgramNode>);
};
