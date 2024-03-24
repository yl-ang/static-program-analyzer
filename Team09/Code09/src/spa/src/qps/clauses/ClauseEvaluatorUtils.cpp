#include "ClauseEvaluatorUtils.h"

std::vector<std::string> ClauseEvaluatorUtils::filterStatementsByType(PKBFacadeReader& reader, DesignEntityType type,
                                                                      std::unordered_set<StmtNum> stmts) {
    StatementType mappedStmtType = DESIGN_ENTITY_TYPE_TO_STMT_TYPE_MAP[type];

    std::vector<std::string> stmtsVector{};

    for (StmtNum currStmt : stmts) {
        if (type == DesignEntityType::STMT || reader.getStatementByStmtNum(currStmt)->type == mappedStmtType) {
            stmtsVector.push_back(std::to_string(currStmt));
        }
    }
    return stmtsVector;
}

bool ClauseEvaluatorUtils::isIdentLiteral(const std::string& str) {
    if (str.empty()) {
        return false;
    }

    return std::isdigit(str[0]) == 0;
}
