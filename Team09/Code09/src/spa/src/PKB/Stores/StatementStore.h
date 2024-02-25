#pragma once

#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "PKB/Utils/DataTypes.h"

// ai-gen start(gpt, 1, e)
// prompt:
// https://platform.openai.com/playground/p/JMwYQcYxmb857W2JkifHSp5w?model=gpt-4&mode=chat
class StatementStore {
public:
    void setStatements(const std::unordered_set<Stmt>& inputStmts);
    const std::unordered_set<Stmt>& getStatements() const;
    std::optional<Stmt> getStatementByStmtNum(StmtNum stmtNum) const;
    std::unordered_set<std::optional<Stmt>> getStatementsByType(StatementType type) const;

private:
    std::unordered_set<Stmt> statementSet;
    std::unordered_map<StmtNum, std::optional<Stmt>> statementNumMap;
    std::unordered_map<StatementType, std::unordered_set<std::optional<Stmt>>> statementTypeMap;
};
// ai-gen end
