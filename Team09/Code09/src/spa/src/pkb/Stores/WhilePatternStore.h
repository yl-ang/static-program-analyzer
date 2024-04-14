#pragma once

#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "pkb/Utils/DataTypes.h"

class WhilePatternStore {
public:
    void setWhilePatternStore(std::unordered_set<std::pair<StmtNum, std::string>> whilePatternPairs);
    bool hasWhilePattern(StmtNum stmtNum, const std::string& arg);

private:
    std::unordered_map<StmtNum, std::unordered_set<std::string>> whilePatternsMap;
};
