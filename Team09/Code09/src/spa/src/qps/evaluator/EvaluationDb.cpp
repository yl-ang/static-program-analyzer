#include "EvaluationDb.h"

#include "qps/clauses/relationships/Relationship.h"

std::unordered_set<StmtNum> EvaluationDb::getStmts(const Synonym& syn) const {
    // Check if in cache
    if (tableManager->hasHeader(syn)) {
        std::unordered_set<StmtNum> stmtNums{};
        stmtNums.reserve(tableManager->getColumn(syn).size());
        for (const std::string& value : tableManager->getColumn(syn)) {
            stmtNums.insert(std::stoi(value));
        }
        return stmtNums;
    }

    // If not, then retrieve all
    std::unordered_set<Stmt> allStmts{};

    if (syn.getType() == DesignEntityType::STMT) {
        allStmts = reader.getStmts();
    } else {
        allStmts = reader.getStatementsByType(DESIGN_ENTITY_TYPE_TO_STMT_TYPE_MAP[syn.getType()]);
    }

    std::unordered_set<StmtNum> result{};
    for (Stmt stmt : allStmts) {
        result.insert(stmt.stmtNum);
    }

    return result;
}

std::unordered_set<Procedure> EvaluationDb::getProcedures(const Synonym& syn) const {
    if (tableManager->hasHeader(syn)) {
        std::unordered_set<Procedure> procedures{};
        procedures.reserve(tableManager->getColumn(syn).size());
        for (const std::string& value : tableManager->getColumn(syn)) {
            procedures.insert(value);
        }
        return procedures;
    }

    return reader.getProcedures();
}

std::unordered_set<Variable> EvaluationDb::getVariables(const Synonym& syn) const {
    if (tableManager->hasHeader(syn)) {
        std::unordered_set<Procedure> variables{};
        variables.reserve(tableManager->getColumn(syn).size());
        for (const std::string& value : tableManager->getColumn(syn)) {
            variables.insert(value);
        }
        return variables;
    }

    return reader.getVariables();
}

std::unordered_set<Constant> EvaluationDb::getConstants(const Synonym& syn) const {
    if (tableManager->hasHeader(syn)) {
        std::unordered_set<Procedure> constants{};
        constants.reserve(tableManager->getColumn(syn).size());
        for (const std::string& value : tableManager->getColumn(syn)) {
            constants.insert(value);
        }
        return constants;
    }

    return reader.getConstants();
}
