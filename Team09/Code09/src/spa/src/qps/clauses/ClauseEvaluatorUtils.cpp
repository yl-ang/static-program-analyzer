#include "ClauseEvaluatorUtils.h"

std::unordered_set<StmtNum> ClauseEvaluatorUtils::filterStatementsByType(PKBFacadeReader& reader, DesignEntityType type,
                                                                         std::unordered_set<StmtNum> stmts) {
    StatementType mappedStmtType = DESIGN_ENTITY_TYPE_TO_STMT_TYPE_MAP[type];

    if (type == DesignEntityType::STMT) {
        return stmts;
    }

    std::unordered_set<StmtNum> filteredSet{};
    for (StmtNum currStmt : stmts) {
        if (reader.getStatementByStmtNum(currStmt)->type == mappedStmtType) {
            filteredSet.insert(currStmt);
        }
    }
    return filteredSet;
}

bool ClauseEvaluatorUtils::isIdentLiteral(const std::string& str) {
    if (str.empty()) {
        return false;
    }

    return std::isdigit(str[0]) == 0;
}
