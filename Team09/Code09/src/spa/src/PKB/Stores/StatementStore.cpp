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
        statementNumMap[stmt.stmtNum] = stmt;
        statementTypeMap[stmt.type].insert(stmt);
    }
}

std::optional<Stmt> StatementStore::getStatementByStmtNum(StmtNum stmtNum) const {
    auto it = statementNumMap.find(stmtNum);
    if (it != statementNumMap.end()) {
        return it->second;
    }
    return std::nullopt;
}

std::unordered_set<Stmt> StatementStore::getStatementsByType(StatementType type) const {
    auto it = statementTypeMap.find(type);
    if (it != statementTypeMap.end()) {
        std::unordered_set<Stmt> result;
        for (const auto& optionalStmt : it->second) {
            if (optionalStmt.has_value()) {
                result.insert(optionalStmt.value());
            }
        }
        return result;
    }
    return {};
}
// ai-gen end
