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

// bool FollowsStore::hasFollowRelationship(ClauseArgument arg1, ClauseArgument
// arg2) {
//     if (arg1.isWildcard() && arg2.isWildcard()) {
//         return !followerMap.empty() && !followerByMap.empty();
//
//         // return all the pairs
//     }
//
//     if (arg1.isWildcard()) {
//         return followerByMap.count(arg2.getValue()) > 0;
//
//         // return getFollower(arg2) : form a pair return as a unordered_set
//     }
//
//     if (arg2.isWildcard()) {
//         return followerMap.count(arg1.getValue()) > 0;
//
//         // return getFollower(arg1) : form a pair return as a unordered_set
//     }
//
//     return hasFollowRelationship(arg1.getValue(), arg2.getValue());
// }
//
// bool FollowsStore::hasFollowStarRelationship(ClauseArgument arg1,
//                                          ClauseArgument arg2) {
//     if (arg1.isWildcard() && arg2.isWildcard()) {
//         return !followerStarMap.empty() && !followeeStarMap.empty();
//     }
//
//     if (arg1.isWildcard()) {
//         return followerStarMap.count(arg2.getValue()) > 0;
//     }
//
//     if (arg2.isWildcard()) {
//         return followeeStarMap.count(arg1.getValue()) > 0;
//     }
//
//     return hasFollowStarRelationship(arg1.getValue(), arg2.getValue());
// }
