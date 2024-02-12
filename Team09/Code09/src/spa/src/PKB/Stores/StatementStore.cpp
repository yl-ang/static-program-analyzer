#include "StatementStore.h"

// ai-gen start(gpt, 1, e)
// prompt: https://platform.openai.com/playground/p/JMwYQcYxmb857W2JkifHSp5w?model=gpt-4&mode=chat
const std::unordered_set<Stmt>& StatementStore::getStatements() const {
    return statementSet;
}

void StatementStore::setStatements(const std::unordered_set<Stmt> &inputStmts) {
    statementSet.insert(inputStmts.begin(), inputStmts.end());
}

Stmt* StatementStore::getStatementByStmtNum(StmtNum stmtNum) const {
    for (const auto& stmt : statementSet) {
        if (stmt.stmtNum == stmtNum) {
            return const_cast<Stmt*>(&stmt);
        }
    }
    return nullptr;
}

std::vector<Stmt*> StatementStore::getStatementsByType(StatementType type) const {
    std::vector<Stmt*> stmts;
    for (const auto& stmt : statementSet) {
        if (stmt.type == type) {
            stmts.push_back(const_cast<Stmt*>(&stmt));
        }
    }
    return stmts;
}
