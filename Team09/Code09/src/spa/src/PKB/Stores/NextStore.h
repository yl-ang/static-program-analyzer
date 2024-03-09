#pragma once

#include <qps/clauseArguments/ClauseArgument.h>

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "PKB/Utils/DataTypes.h"

class NextStore {
public:
    void setNextStore(const std::unordered_set<std::pair<StmtNum, StmtNum>>& nextSet);
    std::unordered_set<StmtNum> getNexter(StmtNum nextee);
    std::unordered_set<StmtNum> getNextee(StmtNum nexter);
    bool hasNextRelationship(StmtNum s1, StmtNum s2);
    bool hasNextRelationship(ClauseArgument& arg1, ClauseArgument& arg2);

private:
    // {<s1, s2>, <s1, s3>, <s1, s4>} -> {<s1, {s2, s3, s4}>}
    std::unordered_map<StmtNum, std::unordered_set<StmtNum>> nexterMap;

    // {<s1, s2>, <s1, s3>, <s1, s4>} -> {<s2, {s1}>, <s3, {s2, s1}>, <s4, {s3, s2, s1>}
    std::unordered_map<StmtNum, std::unordered_set<StmtNum>> nexteeMap;
};
