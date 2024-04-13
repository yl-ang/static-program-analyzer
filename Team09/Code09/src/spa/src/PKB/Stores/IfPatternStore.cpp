#include "IfPatternStore.h"

#include <algorithm>
#include <functional>

#include "pkb/Utils/DataTypes.h"

void IfPatternStore::setIfPatternStore(std::unordered_set<std::pair<StmtNum, std::string>> ifPatternPairs) {
    for (const auto& pattern : ifPatternPairs) {
        const auto& arg = pattern.second;
        ifPatternsMap[pattern.first].insert(arg);
    }
}

bool IfPatternStore::hasIfPattern(StmtNum stmtNum, const std::string& arg) {
    auto it = ifPatternsMap.find(stmtNum);

    // Check if stmtNum is found
    if (it != ifPatternsMap.end()) {
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
