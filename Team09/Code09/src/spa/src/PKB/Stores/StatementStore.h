#pragma once

#include <unordered_set>
#include <vector>
#include "PKB/Utils/DataTypes.h"

// ai-gen start(gpt, 0, e)
// prompt: https://platform.openai.com/playground/p/JMwYQcYxmb857W2JkifHSp5w?model=gpt-4&mode=chat
class StatementStore {
public:
    const std::unordered_set<Stmt>& getStatements() const;
    void setStatements(const std::unordered_set<Stmt>& inputStmts);
    Stmt* getStatementByStmtNum(StmtNum stmtNum) const;
    std::vector<Stmt*> getStatementsByType(StatementType type) const;

private:
    std::unordered_set<Stmt> statementSet;
};

