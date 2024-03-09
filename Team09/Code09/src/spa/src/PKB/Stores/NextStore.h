#pragma once

#include <qps/clauseArguments/ClauseArgument.h>

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "PKB/Utils/DataTypes.h"

class NextStore {
public:
    void setNextStore(const std::unordered_set<std::pair<StmtNum, StmtNum>> &nextSet);

    std::unordered_set<StmtNum> getNexter(StmtNum nextee);
    std::unordered_set<StmtNum> getNextee(StmtNum nexter);
    std::unordered_set<StmtNum> getNexterStar(StmtNum nextee);
    std::unordered_set<StmtNum> getNexteeStar(StmtNum nexter);
    bool hasNextRelationship(StmtNum s1, StmtNum s2);
    bool hasNextRelationship(ClauseArgument &arg1, ClauseArgument &arg2);
    bool hasNextStarRelationship(StmtNum s1, StmtNum s2);
    bool hasNextStarRelationship(ClauseArgument &arg1, ClauseArgument &arg2);

private:
    std::unordered_map<StmtNum, std::unordered_set<StmtNum>> nexterMap;
    std::unordered_map<StmtNum, std::unordered_set<StmtNum>> nexteeMap;

    // helper functions for next star
    void dfsNextStar(StmtNum current, std::unordered_set<StmtNum> &result, std::unordered_set<StmtNum> &visited,
                     const std::unordered_map<StmtNum, std::unordered_set<StmtNum>> &nextMap);
    std::unordered_set<StmtNum> computeNextStar(
        StmtNum start, const std::unordered_map<StmtNum, std::unordered_set<StmtNum>> &nextMap);
};