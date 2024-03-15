#include "UsesStore.h"

void UsesStore::setStatementUsesStore(const std::unordered_set<std::pair<StmtNum, Variable>>& usesStatementPairs) {
    for (const auto& use : usesStatementPairs) {
        StmtNum stmt = use.first;
        Variable variable = use.second;

        stmtToUsedVariables[stmt].insert(variable);
        variableToUsingStatements[variable].insert(stmt);
    }
}

void UsesStore::setProcedureUsesStore(const std::unordered_set<std::pair<Procedure, Variable>>& usesProcedurePairs) {
    for (const auto& use : usesProcedurePairs) {
        Procedure procedure = use.first;
        Variable variable = use.second;

        procedureToUsedVariables[procedure].insert(variable);
        variableToUsingProcedures[variable].insert(procedure);
    }
}

bool UsesStore::hasStatementVariableUseRelationship(StmtNum stmt, const Variable& variable) const {
    auto stmtIt = stmtToUsedVariables.find(stmt);
    if (stmtIt != stmtToUsedVariables.end()) {
        return stmtIt->second.count(variable) > 0;
    }
    return false;
}

bool UsesStore::hasProcedureVariableUseRelationship(const Procedure& procedure, const Variable& variable) const {
    auto procedureIt = procedureToUsedVariables.find(procedure);
    if (procedureIt != procedureToUsedVariables.end()) {
        return procedureIt->second.count(variable) > 0;
    }
    return false;
}

std::unordered_set<Variable> UsesStore::getVariablesByStatement(StmtNum stmt) const {
    auto stmtIt = stmtToUsedVariables.find(stmt);
    if (stmtIt != stmtToUsedVariables.end()) {
        return stmtIt->second;
    }
    return {};
}

std::unordered_set<StmtNum> UsesStore::getStatementsByVariable(const Variable& variable) const {
    auto variableIt = variableToUsingStatements.find(variable);
    if (variableIt != variableToUsingStatements.end()) {
        return variableIt->second;
    }
    return {};
}

std::unordered_set<Variable> UsesStore::getVariablesByProcedure(const Procedure& procedure) const {
    auto procedureIt = procedureToUsedVariables.find(procedure);
    if (procedureIt != procedureToUsedVariables.end()) {
        return procedureIt->second;
    }
    return {};
}

std::unordered_set<Procedure> UsesStore::getProceduresByVariable(const Variable& variable) const {
    auto variableIt = variableToUsingProcedures.find(variable);
    if (variableIt != variableToUsingProcedures.end()) {
        return variableIt->second;
    }
    return {};
}

bool UsesStore::hasStatementVariableUseRelationship(ClauseArgument& arg1, ClauseArgument& arg2) {
    if (arg1.isWildcard() && arg2.isWildcard()) {
        return !stmtToUsedVariables.empty();
    }

    // if arg1 is wildcard, find out if arg2 is used by statement(s)
    if (arg1.isWildcard()) {
        return !getStatementsByVariable(arg2.getValue()).empty();
    }

    // if arg2 is wildcard, find out if arg1 uses any variables
    if (arg2.isWildcard()) {
        return !getVariablesByStatement(std::stoi(arg1.getValue())).empty();
    }

    return hasStatementVariableUseRelationship(std::stoi(arg1.getValue()), arg2.getValue());
}

bool UsesStore::hasProcedureVariableUseRelationship(ClauseArgument& arg1, ClauseArgument& arg2) const {
    if (arg1.isWildcard() && arg2.isWildcard()) {
        return !procedureToUsedVariables.empty();
    }

    // if arg1 is wildcard, find out if arg2 is used by procedures(s)
    if (arg1.isWildcard()) {
        return !getProceduresByVariable(arg2.getValue()).empty();
    }

    // if arg2 is wildcard, find out if arg1 uses any variables
    if (arg2.isWildcard()) {
        return !getVariablesByProcedure(arg1.getValue()).empty();
    }

    return hasProcedureVariableUseRelationship(arg1.getValue(), arg2.getValue());
}
