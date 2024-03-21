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
        const auto& arg = pattern.second;
        ifPatternsMap[pattern.first].insert(arg);
    }
}

bool IfPatternStore::hasExactPattern(StmtNum stmtNum, std::string arg) {
    return applyIfPatternFunction(exactMatchFP, stmtNum, arg);
}

bool IfPatternStore::hasPartialPattern(StmtNum stmtNum, std::string arg) {
    return applyIfPatternFunction(partialMatchFP, stmtNum, arg);
}

bool IfPatternStore::applyIfPatternFunction(std::function<bool(std::string, std::string)> function, StmtNum stmtNum,
                                            std::string arg) {
    auto it = ifPatternsMap.find(stmtNum);

    // Check if stmtNum is found
    if (it != ifPatternsMap.end()) {
        // Retrieve all the control variables with stmtNum
        const auto& vars = it->second;
        for (const auto& var : vars) {
            return function(var, arg);
        }
    }
    return false;
}
