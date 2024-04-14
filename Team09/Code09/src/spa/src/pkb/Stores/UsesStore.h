#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "pkb/Utils/DataTypes.h"
#include "qps/clauseArguments/ClauseArgument.h"
#include "qps/clauseArguments/Literal.h"

class UsesStore {
public:
    void setStatementUsesStore(const std::unordered_set<std::pair<StmtNum, Variable>>& usesStatementPairs);
    void setProcedureUsesStore(const std::unordered_set<std::pair<Procedure, Variable>>& usesProcedurePairs);
    bool hasStatementVariableUseRelationship(StmtNum stmt, const Variable& variable) const;
    bool hasStatementVariableUseRelationship(ClauseArgument& arg1, ClauseArgument& arg2);
    bool hasProcedureVariableUseRelationship(const Procedure& procedure, const Variable& variable) const;
    bool hasProcedureVariableUseRelationship(ClauseArgument& arg1, ClauseArgument& arg2) const;
    std::unordered_set<Variable> getVariablesByStatement(StmtNum stmt) const;
    std::unordered_set<StmtNum> getStatementsByVariable(const Variable& variable) const;
    std::unordered_set<Variable> getVariablesByProcedure(const Procedure& procedure) const;
    std::unordered_set<Procedure> getProceduresByVariable(const Variable& variable) const;

private:
    std::unordered_map<StmtNum, std::unordered_set<Variable>> stmtToUsedVariables;
    std::unordered_map<Variable, std::unordered_set<StmtNum>> variableToUsingStatements;
    std::unordered_map<Procedure, std::unordered_set<Variable>> procedureToUsedVariables;
    std::unordered_map<Variable, std::unordered_set<Procedure>> variableToUsingProcedures;
};
