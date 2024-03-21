#include "WhilePatternStore.h"

void WhilePatternStore::initialiseStore(std::function<bool(std::string, std::string)> exactMatchFP,
                                        std::function<bool(std::string, std::string)> partialMatchFP,
                                        std::unordered_set<std::pair<StmtNum, std::string>> whilePatternPairs) {
    this->exactMatchFP = exactMatchFP;
    this->partialMatchFP = partialMatchFP;
    setWhilePatterns(whilePatternPairs);
}

void WhilePatternStore::setWhilePatterns(const std::unordered_set<std::pair<StmtNum, std::string>>& whilePatternPairs) {
    for (const auto& pattern : whilePatternPairs) {
        const auto& condExpr = pattern.second;
        whilePatternsMap[pattern.first] = condExpr;
    }
}

bool WhilePatternStore::hasExactPattern(StmtNum stmtNum, std::string expr) {
    return applyWhilePatternFunction(exactMatchFP, stmtNum, expr);
}

bool WhilePatternStore::hasPartialPattern(StmtNum stmtNum, std::string expr) {
    return applyWhilePatternFunction(partialMatchFP, stmtNum, expr);
}

bool WhilePatternStore::applyWhilePatternFunction(std::function<bool(std::string, std::string)> function,
                                                  StmtNum stmtNum, std::string expr) {
    auto it = whilePatternsMap.find(stmtNum);

    // Check if stmtNum is found
    if (it != whilePatternsMap.end()) {
        // Retrieve the expr associated with stmtNum
        const auto& whilePatternExpr = it->second;
        return function(whilePatternExpr, expr);
    }
    return false;
}
