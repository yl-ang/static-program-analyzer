#pragma once

#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "PKB/Utils/DataTypes.h"

class IfPatternStore {
public:
    void setIfPatternStore(std::unordered_set<std::pair<StmtNum, std::string>> ifPatternPairs);
    bool hasIfPattern(StmtNum stmtNum, const std::string& arg);

private:
    std::unordered_map<StmtNum, std::unordered_set<std::string>> ifPatternsMap;
};
