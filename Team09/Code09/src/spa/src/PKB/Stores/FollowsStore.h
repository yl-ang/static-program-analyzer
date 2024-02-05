#pragma once

#include <unordered_map>
#include <optional>
#include "PKB/Utils/PairHash.h"

class FollowsStore {

public:
    using StmtNum = int;

    void setFollowsStore(StmtNum s1, StmtNum s2);

    std::optional<StmtNum> getFollower(StmtNum s1);

    std::optional<StmtNum> getFollowee(StmtNum s2);

private:
    std::unordered_map<StmtNum, StmtNum> followsStorage;
};
