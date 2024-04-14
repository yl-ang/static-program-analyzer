#include "WhilePatternStore.h"

void WhilePatternStore::setWhilePatternStore(std::unordered_set<std::pair<StmtNum, std::string>> whilePatternPairs) {
    for (const auto& pattern : whilePatternPairs) {
        const auto& variable = pattern.second;
        whilePatternsMap[pattern.first].insert(variable);
    }
}

bool WhilePatternStore::hasWhilePattern(StmtNum stmtNum, const std::string& arg) {
    auto it = whilePatternsMap.find(stmtNum);

    // Check if stmtNum is found
    if (it != whilePatternsMap.end()) {
        // Retrieve all the control variables with stmtNum
        const auto& vars = it->second;
        for (const auto& var : vars) {
            if (var == arg) {
                return true;
            }
        }
    }
    return false;
}
