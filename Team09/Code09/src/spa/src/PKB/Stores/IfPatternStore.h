#pragma once

#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "PKB/Utils/DataTypes.h"

class IfPatternStore {
public:
    void initialiseStore(std::function<bool(std::string, std::string)> exactMatchFP,
                         std::function<bool(std::string, std::string)> partialMatchFP,
                         std::unordered_set<std::pair<StmtNum, std::string>> ifPatternPairs);

    bool hasExactPattern(StmtNum stmtNum, std::string arg);
    bool hasPartialPattern(StmtNum stmtNum, std::string arg);

private:
    std::unordered_map<StmtNum, std::unordered_set<std::string>> ifPatternsMap;
    void setIfPatterns(const std::unordered_set<std::pair<StmtNum, std::string>>& ifPatternPairs);
    bool applyIfPatternFunction(std::function<bool(std::string, std::string)> function, StmtNum stmtNum,
                                std::string arg);
    std::function<bool(std::string, std::string)> exactMatchFP;
    std::function<bool(std::string, std::string)> partialMatchFP;
};
