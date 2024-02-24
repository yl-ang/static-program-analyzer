#include "FollowsStore.h"

void FollowsStore::setFollowsStore(const std::unordered_set<std::pair<StmtNum, StmtNum>>& followsPairs) {
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

    // Update the matrices with transitive closure using the utility class
    TransitiveClosureUtility::computeTransitiveClosure(followerStarMap);
    TransitiveClosureUtility::computeTransitiveClosure(followeeStarMap);
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

bool FollowsStore::hasFollowRelationship(StmtNum s1, StmtNum s2) {
    auto it = followerMap.find(s1);
    if (it != followerMap.end()) {
        return it->second == s2;
    }
    return false;
}

bool FollowsStore::hasFollowStarRelationship(StmtNum s1, StmtNum s2) {
    if (followerStarMap.count(s1)) {
        return followerStarMap[s1].count(s2);
    }
    return false;
}

bool FollowsStore::hasFollowRelationship(ClauseArgument& arg1, ClauseArgument& arg2) {
    // Both are wildcards, just check if followerMap is not empty
    if (arg1.isWildcard() && arg2.isWildcard()) {
        return !followerMap.empty();
    }

    // If arg1 is wildcard, look at arg2 to find out if it is following a statement
    if (arg1.isWildcard()) {
        return getFollowee(std::stoi(arg2.getValue())).has_value();
    }

    // if arg2 is wildcard, look at arg1 to find out if it has a follower
    if (arg2.isWildcard()) {
        return getFollower(std::stoi(arg1.getValue())).has_value();
    }

    return hasFollowRelationship(std::stoi(arg1.getValue()), std::stoi(arg2.getValue()));
}

bool FollowsStore::hasFollowStarRelationship(ClauseArgument& arg1, ClauseArgument& arg2) {
    // Both are wildcards, just check if the followerStarMap is not empty
    if (arg1.isWildcard() && arg2.isWildcard()) {
        return !followerStarMap.empty();
    }

    // if arg1 is wildcard, then look at arg2 to check if it is following other statements
    if (arg1.isWildcard()) {
        return !getFolloweesStar(std::stoi(arg2.getValue())).empty();
    }

    // if arg2 is wildcard, then look at arg1 to check if it has followers
    if (arg2.isWildcard()) {
        return !getFollowersStar(std::stoi(arg1.getValue())).empty();
    }

    return hasFollowStarRelationship(std::stoi(arg1.getValue()), std::stoi(arg2.getValue()));
}
