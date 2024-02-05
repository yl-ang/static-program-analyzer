#include <optional>
#include "FollowsStore.h"

void FollowsStore::setFollowsStore(StmtNum s1, StmtNum s2) {
    followsStorage[s1] = s2;
}

std::optional<FollowsStore::StmtNum> FollowsStore::getFollower(StmtNum s1) {
    auto it = followsStorage.find(s1);
    if (it != followsStorage.end()) {
        return it->second;
    }
    return std::nullopt;
}

std::optional<FollowsStore::StmtNum> FollowsStore::getFollowee(StmtNum s2) {
    for (const auto& pair : followsStorage) {
        if (pair.second == s2) {
            // Found a pair where s2 is the followee
            return pair.first;
        }
    }
    return std::nullopt;
}
