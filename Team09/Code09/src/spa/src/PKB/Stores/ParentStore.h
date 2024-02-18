#pragma once

#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "PKB/Utils/DataTypes.h"

class ParentStore {
public:
    void setParentStore(
        const std::unordered_set<std::pair<StmtNum, StmtNum>>& parentPairs);

    std::optional<StmtNum> getParent(StmtNum child);
    std::unordered_set<StmtNum> getChildren(StmtNum parent);
    std::unordered_set<StmtNum> getChildrenStar(StmtNum parent);

    std::vector<StmtNum> getParents(StmtNum child);
    std::unordered_set<StmtNum> getParentsStar(StmtNum child);

    bool containsParentRelationship(StmtNum parent, StmtNum child);
    bool containsParentStarRelationship(StmtNum parent, StmtNum child);

private:
    std::unordered_map<StmtNum, StmtNum> parentMap;
    std::unordered_map<StmtNum, std::unordered_set<StmtNum>> parentStarMap;
    std::unordered_map<StmtNum, std::unordered_set<StmtNum>> childStarMap;

    void computeTransitiveClosure();
    void updateTransitiveClosure(StmtNum parent, StmtNum child);
};