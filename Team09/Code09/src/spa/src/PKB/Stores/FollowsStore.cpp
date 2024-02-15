#include "FollowsStore.h"

void FollowsStore::setFollowsStore(
    const std::unordered_set<std::pair<StmtNum, StmtNum>>& followsPairs) {
    // Initialize the followsMap and followedByMap
    for (const auto& pair : followsPairs) {
        StmtNum s1 = pair.first;
        StmtNum s2 = pair.second;

        followsMap[s1] = s2;
        followedByMap[s2] = s1;
    }

    // Compute and store the transitive closure matrix
    computeTransitiveClosure();
}

void FollowsStore::computeTransitiveClosure() {
    // Initialize the matrix
    followsTransitiveClosure.clear();
    for (const auto& pair : followsMap) {
        StmtNum s1 = pair.first;
        StmtNum s2 = pair.second;

        if (!followsTransitiveClosure.count(s1)) {
            followsTransitiveClosure[s1] = std::unordered_set<StmtNum>();
        }
        followsTransitiveClosure[s1].insert(s2);
    }

    // Update the matrix with transitive closure
    for (const auto& entry : followsTransitiveClosure) {
        StmtNum s1 = entry.first;
        for (const auto& s2 : entry.second) {
            updateTransitiveClosure(s1, s2);
        }
    }
}

void FollowsStore::updateTransitiveClosure(StmtNum s1, StmtNum s2) {
    if (followsTransitiveClosure.count(s2)) {
        for (const auto& s3 : followsTransitiveClosure[s2]) {
            followsTransitiveClosure[s1].insert(s3);
        }
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
    if (followsTransitiveClosure.count(s)) {
        return followsTransitiveClosure[s];
    }
    return {};
}

std::unordered_set<StmtNum> FollowsStore::getFollowersStar(StmtNum s) {
    std::unordered_set<StmtNum> followersStar;
    for (const auto& entry : followsTransitiveClosure) {
        if (entry.second.count(s)) {
            followersStar.insert(entry.first);
        }
    }
    return followersStar;
}

bool FollowsStore::containsFollowRelationship(StmtNum s1, StmtNum s2) {
    auto it = followsMap.find(s1);
    if (it != followsMap.end()) {
        return it->second == s2;
    }
    return false;
}

bool FollowsStore::containsFollowStarRelationship(StmtNum s1, StmtNum s2) {
    if (followsTransitiveClosure.count(s1)) {
        return followsTransitiveClosure[s1].count(s2);
    }
    return false;
}
