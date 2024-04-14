#pragma once

#include <qps/clauseArguments/ClauseArgument.h>

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "pkb/Utils/DataTypes.h"

class ModifiesStore {
public:
    void setStatementModifiesStore(const std::unordered_set<std::pair<StmtNum, Variable>>& modifiesStatementSet);
    void setProcedureModifiesStore(const std::unordered_set<std::pair<Procedure, Variable>>& modifiesProcedureSet);
    bool hasStatementVariableModifiesRelationship(StmtNum stmt, const Variable& variable) const;
    bool hasStatementVariableModifiesRelationship(ClauseArgument& arg1, ClauseArgument& arg2) const;
    bool hasProcedureVariableModifiesRelationship(Procedure procedure, const Variable& variable) const;
    bool hasProcedureVariableModifiesRelationship(ClauseArgument& arg1, ClauseArgument& arg2) const;
    std::unordered_set<Variable> getVariablesByStatement(StmtNum stmt) const;
    std::unordered_set<StmtNum> getStatementsByVariable(const Variable& variable) const;
    std::unordered_set<Variable> getVariablesByProcedure(const Procedure& procedure) const;
    std::unordered_set<Procedure> getProceduresByVariable(const Variable& variable) const;

private:
    std::unordered_map<StmtNum, std::unordered_set<Variable>> stmtToModifiedVariables;
    std::unordered_map<Variable, std::unordered_set<StmtNum>> variableToModifyingStatements;
    std::unordered_map<Procedure, std::unordered_set<Variable>> procedureToModifiedVariables;
    std::unordered_map<Variable, std::unordered_set<Procedure>> variableToModifyingProcedures;
};
