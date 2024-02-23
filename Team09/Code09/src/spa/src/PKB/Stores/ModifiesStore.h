#pragma once

#include <qps/clauseArguments/ClauseArgument.h>

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "PKB/Utils/DataTypes.h"

class ModifiesStore {
public:
    void setModifiesStore(const std::unordered_set<std::pair<StmtNum, Variable>>& modifiesSet);
    bool hasStatementVariableModifiesRelationship(StmtNum stmt, const Variable& variable) const;
    bool hasStatementVariableModifiesRelationship(ClauseArgument& arg1, ClauseArgument& arg2) const;
    std::unordered_set<Variable> getVariablesByStatement(StmtNum stmt) const;
    std::unordered_set<StmtNum> getStatementsByVariable(const Variable& variable) const;

private:
    std::unordered_map<StmtNum, std::unordered_set<Variable>> stmtToModifiedVariables;
    std::unordered_map<Variable, std::unordered_set<StmtNum>> variableToModifyingStatements;
};
