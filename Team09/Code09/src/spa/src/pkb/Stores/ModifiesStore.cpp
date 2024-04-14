#include "ModifiesStore.h"

void ModifiesStore::setStatementModifiesStore(
    const std::unordered_set<std::pair<StmtNum, Variable>>& modifiesStatementSet) {
    for (const auto& modify : modifiesStatementSet) {
        StmtNum stmt = modify.first;
        Variable variable = modify.second;

        stmtToModifiedVariables[stmt].insert(variable);
        variableToModifyingStatements[variable].insert(stmt);
    }
}

void ModifiesStore::setProcedureModifiesStore(
    const std::unordered_set<std::pair<Procedure, Variable>>& modifiesProcedureSet) {
    for (const auto& modify : modifiesProcedureSet) {
        Procedure procedure = modify.first;
        Variable variable = modify.second;

        procedureToModifiedVariables[procedure].insert(variable);
        variableToModifyingProcedures[variable].insert(procedure);
    }
}

bool ModifiesStore::hasStatementVariableModifiesRelationship(StmtNum stmt, const Variable& variable) const {
    auto stmtIt = stmtToModifiedVariables.find(stmt);
    if (stmtIt != stmtToModifiedVariables.end()) {
        return stmtIt->second.count(variable) > 0;
    }
    return false;
}

bool ModifiesStore::hasStatementVariableModifiesRelationship(ClauseArgument& arg1, ClauseArgument& arg2) const {
    if (arg1.isWildcard() && arg2.isWildcard()) {
        return !stmtToModifiedVariables.empty();
    }

    // if arg1 is wildcard, check if arg2 is modified by any statements
    if (arg1.isWildcard()) {
        return !getStatementsByVariable(arg2.getValue()).empty();
    }

    // if arg2 is wildcard, check if arg1 modifies any variables
    if (arg2.isWildcard()) {
        return !getVariablesByStatement(std::stoi(arg1.getValue())).empty();
    }

    return hasStatementVariableModifiesRelationship(std::stoi(arg1.getValue()), arg2.getValue());
}

bool ModifiesStore::hasProcedureVariableModifiesRelationship(Procedure procedure, const Variable& variable) const {
    auto stmtIt = procedureToModifiedVariables.find(procedure);
    if (stmtIt != procedureToModifiedVariables.end()) {
        return stmtIt->second.count(variable) > 0;
    }
    return false;
}

bool ModifiesStore::hasProcedureVariableModifiesRelationship(ClauseArgument& arg1, ClauseArgument& arg2) const {
    if (arg1.isWildcard() && arg2.isWildcard()) {
        return !procedureToModifiedVariables.empty();
    }

    // if arg1 is wildcard, check if arg2 is modified by any procedures
    if (arg1.isWildcard()) {
        return !getProceduresByVariable(arg2.getValue()).empty();
    }

    // if arg2 is wildcard, check if arg1 modifies any variables
    if (arg2.isWildcard()) {
        return !getVariablesByProcedure(arg1.getValue()).empty();
    }

    return hasProcedureVariableModifiesRelationship(arg1.getValue(), arg2.getValue());
}

std::unordered_set<Variable> ModifiesStore::getVariablesByStatement(StmtNum stmt) const {
    auto stmtIt = stmtToModifiedVariables.find(stmt);
    if (stmtIt != stmtToModifiedVariables.end()) {
        return stmtIt->second;
    }
    return {};
}

std::unordered_set<StmtNum> ModifiesStore::getStatementsByVariable(const Variable& variable) const {
    auto variableIt = variableToModifyingStatements.find(variable);
    if (variableIt != variableToModifyingStatements.end()) {
        return variableIt->second;
    }
    return {};
}

std::unordered_set<Variable> ModifiesStore::getVariablesByProcedure(const Procedure& procedure) const {
    auto procIt = procedureToModifiedVariables.find(procedure);
    if (procIt != procedureToModifiedVariables.end()) {
        return procIt->second;
    }
    return {};
}

std::unordered_set<Procedure> ModifiesStore::getProceduresByVariable(const Variable& variable) const {
    auto variableIt = variableToModifyingProcedures.find(variable);
    if (variableIt != variableToModifyingProcedures.end()) {
        return variableIt->second;
    }
    return {};
}
