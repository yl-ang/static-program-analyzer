#include "EvaluationDb.h"

#include "qps/clauses/relationships/Relationship.h"

std::unordered_set<StmtNum> EvaluationDb::getStmts(const Synonym& syn) {
    // Check if in cache
    SynonymName value = syn.getValue();

    if (tableManager->hasHeader(syn)) {
        std::unordered_set<StmtNum> stmtNums{};
        for (std::string value : tableManager->getColumn(syn)) {
            stmtNums.insert(std::stoi(value));
        }
        return stmtNums;
    }

    // If not, then retrieve all
    std::unordered_set<Stmt> allStmts{};

    if (syn.getType() == DesignEntityType::STMT) {
        allStmts = reader->getStmts();
    } else {
        allStmts = reader->getStatementsByType(DESIGN_ENTITY_TYPE_TO_STMT_TYPE_MAP[syn.getType()]);
    }

    std::unordered_set<StmtNum> result{};
    for (Stmt stmt : allStmts) {
        result.insert(stmt.stmtNum);
    }

    return result;
}
