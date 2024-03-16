#include "sp/ast/SemanticValidator.h"

#include <memory>
#include <unordered_set>

#include "sp/ast/SemanticValidatorVisitor.h"
#include "sp/de/AstVisitor.h"
#include "sp/de/NodeDeclarations.h"
#include "sp/exceptions/semantic/DuplicateProcError.h"

void SemanticValidator::validateSemantics(const std::shared_ptr<ProgramNode> programNode) {
    checkDuplicateProcedureNames(programNode);
    this->semanticValidatorVisitor =
        new SemanticValidatorVisitor(std::make_shared<std::unordered_set<std::string>>(procedureNames));
    checkCallingProcedure(programNode);
}

void SemanticValidator::checkDuplicateProcedureNames(std::shared_ptr<ProgramNode> programNode) {
    for (auto procedure : programNode->children) {
        std::string procedureName = procedure->name;
        if (procedureNames.find(procedureName) != procedureNames.end()) {
            throw DuplicateProcedureError(procedureName);
        }
        procedureNames.insert(procedure->name);
    }
}

void SemanticValidator::checkCallingProcedure(const std::shared_ptr<ProgramNode> programNode) {
    visitNode(programNode, this->semanticValidatorVisitor);
}

void SemanticValidator::visitNode(std::shared_ptr<ASTNode>&& node, SemanticValidatorVisitor* visitor) {
    if (!node) {
        return;
    }
    node->accept(visitor);

    for (auto child : node->getChildren()) {
        visitNode((std::shared_ptr<ASTNode>&&)child, visitor);
    }
}
