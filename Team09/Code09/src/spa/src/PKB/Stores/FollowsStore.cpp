#include "FollowsStore.h"

void FollowsStore::setFollowsStore(
    const std::unordered_set<std::pair<StmtNum, StmtNum>>& followsPairs) {
    for (const auto& pair : followsPairs) {
        StmtNum s1 = pair.first;
        StmtNum s2 = pair.second;

        followerMap[s1] = s2;
        followerByMap[s2] = s1;
    }

    computeTransitiveClosure();
}

void FollowsStore::computeTransitiveClosure() {
    // Initialize the matrices
    followerStarMap.clear();
    followeeStarMap.clear();

    // Initialize the follower matrix
    for (const auto& pair : followerMap) {
        StmtNum s1 = pair.first;
        StmtNum s2 = pair.second;

        if (!followerStarMap.count(s1)) {
            followerStarMap[s1] = std::unordered_set<StmtNum>();
        }
        followerStarMap[s1].insert(s2);

        // Initialize the followee matrix
        if (!followeeStarMap.count(s2)) {
            followeeStarMap[s2] = std::unordered_set<StmtNum>();
        }
        followeeStarMap[s2].insert(s1);
    }

    // Update the matrices with transitive closure
    for (const auto& entry : followerStarMap) {
        StmtNum s1 = entry.first;
        for (const auto& s2 : entry.second) {
            updateTransitiveClosure(s1, s2);
        }
    }
}

void FollowsStore::updateTransitiveClosure(StmtNum s1, StmtNum s2) {
    if (followerStarMap.count(s2)) {
        for (const auto& s3 : followerStarMap[s2]) {
            followerStarMap[s1].insert(s3);
            followeeStarMap[s3].insert(s1);
        }
    }
}

std::optional<StmtNum> FollowsStore::getFollower(StmtNum s1) {
    auto it = followerMap.find(s1);
    if (it != followerMap.end()) {
        return it->second;
    }
    return std::nullopt;
}

std::optional<StmtNum> FollowsStore::getFollowee(StmtNum s2) {
    auto it = followerByMap.find(s2);
    if (it != followerByMap.end()) {
        return it->second;
    }
    return std::nullopt;
}

std::unordered_set<StmtNum> FollowsStore::getFollowersStar(StmtNum s) {
    if (followerStarMap.count(s)) {
        return followerStarMap[s];
    }
    return {};
}

std::unordered_set<StmtNum> FollowsStore::getFolloweesStar(StmtNum s) {
    if (followeeStarMap.count(s)) {
        return followeeStarMap[s];
    }
    return {};
}

bool FollowsStore::containsFollowRelationship(StmtNum s1, StmtNum s2) {
    auto it = followerMap.find(s1);
    if (it != followerMap.end()) {
        return it->second == s2;
    }
    return false;
}

bool FollowsStore::containsFollowStarRelationship(StmtNum s1, StmtNum s2) {
    if (followerStarMap.count(s1)) {
        return followerStarMap[s1].count(s2);
    }
    return false;
}
