#include "FollowsStore.h"

void FollowsStore::setFollowsStore(const std::unordered_set<std::pair<StmtNum, StmtNum>>& followsPairs) {
    for (const auto& pair : followsPairs) {
        StmtNum s1 = pair.first;
        StmtNum s2 = pair.second;

        followsMap[s1] = s2;
        followedByMap[s2] = s1;

        // TODO(yl-ang): Fix Buggy Implementation of FollowsStarMap
        followsStarMap[s1].insert(s2);
        followedByStarMap[s2].insert(s1);
    }
}

std::optional<StmtNum> FollowsStore::getFollower(StmtNum s1) {
    auto it = followsMap.find(s1);
    if (it != followsMap.end()) {
        return it->second;
    }
    return std::nullopt;
}

std::optional<StmtNum> FollowsStore::getFollowee(StmtNum s2) {
    auto it = followedByMap.find(s2);
    if (it != followedByMap.end()) {
        return it->second;
    }
    return std::nullopt;
}

std::unordered_set<StmtNum> FollowsStore::getFolloweesStar(StmtNum s) {
    auto it = followsStarMap.find(s);
    if (it != followsStarMap.end()) {
        return it->second;
    }
    return {};
}

std::unordered_set<StmtNum> FollowsStore::getFollowersStar(StmtNum s) {
    auto it = followedByStarMap.find(s);
    if (it != followedByStarMap.end()) {
        return it->second;
    }
    return {};
}

bool FollowsStore::containsFollowRelationship(StmtNum s1, StmtNum s2) {
    auto it = followsMap.find(s1);
    if (it != followsMap.end()) {
        return it->second == s2;
    }
    return false;
}

bool FollowsStore::containsFollowStarRelationship(StmtNum s1, StmtNum s2) {
    auto it = followsStarMap.find(s1);
    if (it != followsStarMap.end()) {
        const auto& followers = it->second;
        return followers.find(s2) != followers.end();
    }
    return false;
}
