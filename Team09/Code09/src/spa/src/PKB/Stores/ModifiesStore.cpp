#include "ModifiesStore.h"

void ModifiesStore::setModifiesStore(
    const std::unordered_set<std::pair<StmtNum, Variable>>& modifiesSet) {
    for (const auto& modify : modifiesSet) {
        StmtNum stmt = use.first;
        Variable variable = use.second;

        stmtToModifiedVariables[stmt].insert(variable);
        variableToModifyingStatements[variable].insert(stmt);
    }
}

bool ModifiesStore::hasStatementVariableModifiesRelationship(
    StmtNum stmt, const Variable& variable) const {
    auto stmtIt = stmtToModifiedVariables.find(stmt);
    if (stmtIt != stmtToModifiedVariables.end()) {
        return stmtIt->second.count(variable) > 0;
    }
    return false;
}

std::unordered_set<Variable> ModifiesStore::getVariablesByStatement(
    StmtNum stmt) const {
    auto stmtIt = stmtToModifiedVariables.find(stmt);
    if (stmtIt != stmtToModifiedVariables.end()) {
        return stmtIt->second;
    }
    return {};
}

std::unordered_set<StmtNum> ModifiesStore::getStatementsByVariable(
    const Variable& variable) const {
    auto variableIt = variableToModifyingStatements.find(variable);
    if (variableIt != variableToModifyingStatements.end()) {
        return variableIt->second;
    }
    return {};
}
