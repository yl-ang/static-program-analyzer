#pragma once
#include <memory>
#include <unordered_map>

#include "SemanticValidatorVisitor.h"
#include "sp/de/NodeDeclarations.h"

class SemanticValidator {
public:
    void validateSemantics(std::shared_ptr<ProgramNode> programNode);
    std::string currentProcedure;
    std::shared_ptr<std::unordered_map<std::string, std::vector<std::string>>> procedureNames;

private:
    void checkDuplicateProcedureNames(std::shared_ptr<ProgramNode>);
    void checkCallingProcedure(std::shared_ptr<ProgramNode>);
    void visitNode(std::shared_ptr<ASTNode>&& node, SemanticValidatorVisitor* visitor);
    void checkCyclicCalls();
    void checkCycle(std::shared_ptr<std::unordered_map<std::string, bool>> visited, std::string currentProcedure);
    SemanticValidatorVisitor* semanticValidatorVisitor;
};
