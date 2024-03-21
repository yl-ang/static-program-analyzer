#pragma once

#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "PKB/Utils/DataTypes.h"

class WhilePatternStore {
public:
    void initialiseStore(std::function<bool(std::string, std::string)> exactMatchFP,
                         std::function<bool(std::string, std::string)> partialMatchFP,
                         std::unordered_set<std::pair<StmtNum, std::string>> whilePatternPairs);

    bool hasExactPattern(StmtNum stmtNum, std::string arg);
    bool hasPartialPattern(StmtNum stmtNum, std::string arg);

private:
    std::unordered_map<StmtNum, std::unordered_set<std::string>> whilePatternsMap;
    void setWhilePatterns(const std::unordered_set<std::pair<StmtNum, std::string>>& whilePatternPairs);
    bool applyWhilePatternFunction(std::function<bool(std::string, std::string)> function, StmtNum stmtNum,
                                   std::string arg);
    std::function<bool(std::string, std::string)> exactMatchFP;
    std::function<bool(std::string, std::string)> partialMatchFP;
};
