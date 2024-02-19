#include "UsesStore.h"

void UsesStore::setUsesStore(
    const std::unordered_set<std::pair<StmtNum, Variable>>& usesSet) {
    for (const auto& use : usesSet) {
        StmtNum stmt = use.first;
        Variable variable = use.second;

        stmtToUsedVariables[stmt].insert(variable);
        variableToUsingStatements[variable].insert(stmt);
    }
}

bool UsesStore::hasStatementVariableUseRelationship(
    StmtNum stmt, const Variable& variable) const {
    auto stmtIt = stmtToUsedVariables.find(stmt);
    if (stmtIt != stmtToUsedVariables.end()) {
        return stmtIt->second.count(variable) > 0;
    }
    return false;
}

std::unordered_set<Variable> UsesStore::getVariablesByStatement(
    StmtNum stmt) const {
    auto stmtIt = stmtToUsedVariables.find(stmt);
    if (stmtIt != stmtToUsedVariables.end()) {
        return stmtIt->second;
    }
    return {};
}

std::unordered_set<StmtNum> UsesStore::getStatementsByVariable(
    const Variable& variable) const {
    auto variableIt = variableToUsingStatements.find(variable);
    if (variableIt != variableToUsingStatements.end()) {
        return variableIt->second;
    }
    return {};
}
