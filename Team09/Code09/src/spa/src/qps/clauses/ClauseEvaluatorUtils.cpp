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

std::vector<std::vector<std::string>> ClauseEvaluatorUtils::transpose(std::vector<std::vector<std::string>> target) {
    if (target.empty()) {
        return target;
    }

    // ai-gen start(0, copilot, e)
    // prompt: transpose
    std::vector<std::vector<std::string>> columns{};
    for (size_t i = 0; i < target[0].size(); ++i) {
        std::vector<std::string> column{};
        for (size_t j = 0; j < target.size(); ++j) {
            column.push_back(target[j][i]);
        }
        columns.push_back(column);
    }
    // ai-gen end

    return columns;
}
