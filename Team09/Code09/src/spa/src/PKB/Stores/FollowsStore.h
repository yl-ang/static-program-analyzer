#pragma once
#include <unordered_map>
#include <unordered_set>
#include <optional>
#include "PKB/Utils/DataTypes.h"

// ai-gen start(gpt, 1, e)
// prompt: https://platform.openai.com/playground/p/bAw35gvF9trwUeKVOgSPAObi?model=gpt-4&mode=chat
class FollowsStore {
public:
    void setFollowsStore(const std::unordered_set<std::pair<StmtNum, StmtNum>>& followsPairs);
    std::optional<StmtNum> getFollower(StmtNum s1);
    std::optional<StmtNum> getFollowee(StmtNum s2);
    std::unordered_set<StmtNum> getFolloweesStar(StmtNum s);
    std::unordered_set<StmtNum> getFollowersStar(StmtNum s);
    bool containsFollowRelationship(StmtNum s1, StmtNum s2);
    bool containsFollowStarRelationship(StmtNum s1, StmtNum s2);

private:
    std::unordered_map<StmtNum, StmtNum> followsMap;
    std::unordered_map<StmtNum, StmtNum> followedByMap;
    std::unordered_map<StmtNum, std::unordered_set<StmtNum>> followsStarMap;
    std::unordered_map<StmtNum, std::unordered_set<StmtNum>> followedByStarMap;
};
