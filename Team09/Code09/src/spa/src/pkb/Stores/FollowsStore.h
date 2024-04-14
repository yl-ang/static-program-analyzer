#pragma once

#include <qps/clauseArguments/ClauseArgument.h>

#include <optional>
#include <unordered_map>
#include <unordered_set>

#include "pkb/Utils/DataTypes.h"
#include "pkb/Utils/TransitiveClosureUtility.h"

class FollowsStore {
public:
    void setFollowsStore(const std::unordered_set<std::pair<StmtNum, StmtNum>>& followsPairs);

    std::optional<StmtNum> getFollower(StmtNum s1);
    std::optional<StmtNum> getFollowee(StmtNum s2);
    std::unordered_set<StmtNum> getFollowersStar(StmtNum s);
    std::unordered_set<StmtNum> getFolloweesStar(StmtNum s);
    bool hasFollowRelationship(StmtNum s1, StmtNum s2);
    bool hasFollowStarRelationship(StmtNum s1, StmtNum s2);
    bool hasFollowRelationship(ClauseArgument& arg1, ClauseArgument& arg2);
    bool hasFollowStarRelationship(ClauseArgument& arg1, ClauseArgument& arg2);

private:
    std::unordered_map<StmtNum, StmtNum> followerMap;
    std::unordered_map<StmtNum, StmtNum> followerByMap;
    std::unordered_map<StmtNum, std::unordered_set<StmtNum>> followerStarMap;
    std::unordered_map<StmtNum, std::unordered_set<StmtNum>> followeeStarMap;

    void computeTransitiveClosure();
};
