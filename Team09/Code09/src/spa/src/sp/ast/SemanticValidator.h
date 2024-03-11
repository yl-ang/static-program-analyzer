#pragma once
#include <memory>
#include <unordered_set>

#include "SemanticValidatorVisitor.h"
#include "sp/de/NodeDeclarations.h"

class SemanticValidator {
public:
    void validateSemantics(std::shared_ptr<ProgramNode> programNode);
    std::unordered_set<std::string> procedureNames;

private:
    void checkDuplicateProcedureNames(std::shared_ptr<ProgramNode>);
    void checkCallingProcedure(std::shared_ptr<ProgramNode>);
    void visitNode(std::shared_ptr<ASTNode>&& node, AstVisitor* visitor);
    SemanticValidatorVisitor* semanticValidatorVisitor;
};
