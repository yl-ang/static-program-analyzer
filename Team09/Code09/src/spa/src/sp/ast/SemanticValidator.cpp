#include "sp/ast/SemanticValidator.h"

#include <memory>
#include <unordered_map>
#include <utility>

#include "sp/ast/SemanticValidatorVisitor.h"
#include "sp/de/AstVisitor.h"
#include "sp/de/NodeDeclarations.h"
#include "sp/exceptions/semantic/CyclicProcedureCallsError.h"
#include "sp/exceptions/semantic/DuplicateProcError.h"

void SemanticValidator::validateSemantics(const std::shared_ptr<ProgramNode> programNode) {
    procedureNames = std::make_shared<std::unordered_map<std::string, std::vector<std::string>>>();
    checkDuplicateProcedureNames(programNode);
    this->semanticValidatorVisitor = new SemanticValidatorVisitor(procedureNames);
    checkCallingProcedure(programNode);
    checkCyclicCalls();
}

void SemanticValidator::checkDuplicateProcedureNames(std::shared_ptr<ProgramNode> programNode) {
    for (auto procedure : programNode->children) {
        std::string procedureName = procedure->name;
        if (procedureNames->find(procedureName) != procedureNames->end()) {
            throw DuplicateProcedureError(procedureName);
        }
        procedureNames->insert(std::make_pair(procedureName, std::vector<std::string>{}));
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

void SemanticValidator::checkCyclicCalls() {
    std::unordered_map<std::string, bool> visited;
    for (auto procedureName : *(procedureNames.get())) {
        visited[procedureName.first] = false;
    }

    for (auto& [_, value] : *(procedureNames.get())) {
        for (auto procedureName : value) {
            visited[procedureName] = true;
            std::string currentCall = procedureName;
            checkCycle(std::make_shared<std::unordered_map<std::string, bool>>(visited), currentCall);
            visited[procedureName] = false;
        }
    }
}

void SemanticValidator::checkCycle(std::shared_ptr<std::unordered_map<std::string, bool>> visited,
                                   std::string currentProcedure) {
    if (visited->at(currentProcedure)) {
        throw CyclicProcedureCallsError();
    }

    for (auto& [_, value] : *(procedureNames.get())) {
        for (auto procedureName : value) {
            visited->at(procedureName) = true;
            checkCycle(visited, procedureName);
            visited->at(procedureName) = false;
        }
    }
}
