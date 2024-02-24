#include "ModifiesStore.h"

void ModifiesStore::setModifiesStore(const std::unordered_set<std::pair<StmtNum, Variable>>& modifiesSet) {
    for (const auto& modify : modifiesSet) {
        StmtNum stmt = modify.first;
        Variable variable = modify.second;

        stmtToModifiedVariables[stmt].insert(variable);
        variableToModifyingStatements[variable].insert(stmt);
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
