#include "IfPatternStore.h"

#include <algorithm>
#include <functional>

#include "PKB/Utils/DataTypes.h"

void IfPatternStore::initialiseStore(std::function<bool(std::string, std::string)> exactMatchFP,
                                     std::function<bool(std::string, std::string)> partialMatchFP,
                                     std::unordered_set<std::pair<StmtNum, std::string>> ifPatternPairs) {
    this->exactMatchFP = exactMatchFP;
    this->partialMatchFP = partialMatchFP;
    setIfPatterns(ifPatternPairs);
}

void IfPatternStore::setIfPatterns(const std::unordered_set<std::pair<StmtNum, std::string>>& ifPatternPairs) {
    for (const auto& pattern : ifPatternPairs) {
        const auto& condExpr = pattern.second;
        ifPatternsMap[pattern.first] = condExpr;
    }
}

bool IfPatternStore::hasExactPattern(StmtNum stmtNum, std::string expr) {
    return applyIfPatternFunction(exactMatchFP, stmtNum, expr);
}

bool IfPatternStore::hasPartialPattern(StmtNum stmtNum, std::string expr) {
    return applyIfPatternFunction(partialMatchFP, stmtNum, expr);
}

bool IfPatternStore::applyIfPatternFunction(std::function<bool(std::string, std::string)> function, StmtNum stmtNum,
                                            std::string expr) {
    auto it = ifPatternsMap.find(stmtNum);

    // Check if stmtNum is found
    if (it != ifPatternsMap.end()) {
        // Retrieve the expr associated with stmtNum
        const auto& IfPatternExpr = it->second;
        return function(IfPatternExpr, expr);
    }
    return false;
}
