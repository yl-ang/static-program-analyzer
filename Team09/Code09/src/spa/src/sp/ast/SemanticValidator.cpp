#include "sp/ast/SemanticValidator.h"

#include <unordered_set>

#include "sp/exceptions/semantic/DuplicateProcError.h"
void SemanticValidator::validateSemantics(std::shared_ptr<ProgramNode> programNode) {
    checkDuplicateProcedureNames(programNode);
}

void SemanticValidator::checkDuplicateProcedureNames(std::shared_ptr<ProgramNode> programNode) {
    std::unordered_set<std::string> procedureNames;
    for (auto procedure : programNode->getChildren()) {
        std::string procedureName = procedure->getValue();
        if (procedureNames.find(procedureName) != procedureNames.end()) {
            throw DuplicateProcedureError(procedureName);
        }
        procedureNames.insert(procedure->getValue());
    }
}
