#pragma once

#include <qps/clauseArguments/ClauseArgument.h>

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "PKB/Utils/DataTypes.h"

class UsesStore {
public:
    void setUsesStore(const std::unordered_set<std::pair<StmtNum, Variable>>& usesSet);
    bool hasStatementVariableUseRelationship(StmtNum stmt, const Variable& variable) const;
    bool hasStatementVariableUseRelationship(ClauseArgument& arg1, ClauseArgument& arg2);
    std::unordered_set<Variable> getVariablesByStatement(StmtNum stmt) const;
    std::unordered_set<StmtNum> getStatementsByVariable(const Variable& variable) const;

private:
    std::unordered_map<StmtNum, std::unordered_set<Variable>> stmtToUsedVariables;
    std::unordered_map<Variable, std::unordered_set<StmtNum>> variableToUsingStatements;
};
