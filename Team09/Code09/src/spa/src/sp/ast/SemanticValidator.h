#pragma once
#include <memory>

#include "sp/ast/grammar_nodes/ProgramNode.h"

class SemanticValidator {
public:
    void validateSemantics(std::shared_ptr<ProgramNode> programNode);

private:
    void checkDuplicateProcedureNames(std::shared_ptr<ProgramNode>);
};
