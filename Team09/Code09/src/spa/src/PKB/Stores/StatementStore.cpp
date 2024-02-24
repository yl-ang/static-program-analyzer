#include "StatementStore.h"

// ai-gen start(gpt, 1, e)
// prompt:
// https://platform.openai.com/playground/p/JMwYQcYxmb857W2JkifHSp5w?model=gpt-4&mode=chat
const std::unordered_set<Stmt>& StatementStore::getStatements() const {
    return statementSet;
}

void StatementStore::setStatements(const std::unordered_set<Stmt>& inputStmts) {
    statementSet.insert(inputStmts.begin(), inputStmts.end());

    // Update statementTypeMap
    for (const auto& stmt : inputStmts) {
        statementNumMap[stmt.stmtNum] = const_cast<Stmt*>(&stmt);
        statementTypeMap[stmt.type].insert(const_cast<Stmt*>(&stmt));
    }
}

Stmt* StatementStore::getStatementByStmtNum(StmtNum stmtNum) const {
    auto it = statementNumMap.find(stmtNum);
    if (it != statementNumMap.end()) {
        return it->second;
    }
    return nullptr;
}

std::unordered_set<Stmt*> StatementStore::getStatementsByType(
    StatementType type) const {
    auto it = statementTypeMap.find(type);
    if (it != statementTypeMap.end()) {
        return it->second;
    }
    return {};
}
// ai-gen end
