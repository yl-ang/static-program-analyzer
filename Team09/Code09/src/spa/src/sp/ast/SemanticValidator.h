#pragma once
#include <memory>
#include <unordered_map>

#include "SemanticValidatorVisitor.h"
#include "sp/de/NodeDeclarations.h"

class SemanticValidator {
public:
    void validateSemantics(std::shared_ptr<ProgramNode> programNode);

private:
    void checkDuplicateProcedureNames(std::shared_ptr<ProgramNode> program);
    std::shared_ptr<std::unordered_map<std::string, std::vector<std::string>>> procedureNames;
    void checkCallingProcedure(std::shared_ptr<ProgramNode> program);
    void visitNode(std::shared_ptr<ASTNode>&& node, SemanticValidatorVisitor* visitor);
    void checkCyclicCalls();
    void checkCycle(std::shared_ptr<std::unordered_map<std::string, bool>> visited, std::string currentProcedure);
    SemanticValidatorVisitor* semanticValidatorVisitor;
};
