#pragma once

#include <qps/clauseArguments/ClauseArgument.h>

#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "pkb/Utils/DataTypes.h"
#include "pkb/Utils/TransitiveClosureUtility.h"

class ParentStore {
public:
    void setParentStore(const std::unordered_set<std::pair<StmtNum, StmtNum>>& parentPairs);

    std::optional<StmtNum> getParent(StmtNum child);
    std::unordered_set<StmtNum> getChildren(StmtNum parent);
    std::unordered_set<StmtNum> getChildrenStar(StmtNum parent);
    std::unordered_set<StmtNum> getParentsStar(StmtNum child);
    bool hasParentRelationship(StmtNum parent, StmtNum child);
    bool hasParentStarRelationship(StmtNum parent, StmtNum child);
    bool hasParentRelationship(ClauseArgument& arg1, ClauseArgument& arg2);
    bool hasParentStarRelationship(ClauseArgument& arg1, ClauseArgument& arg2);

private:
    std::unordered_map<StmtNum, StmtNum> parentMap;
    std::unordered_map<StmtNum, std::unordered_set<StmtNum>> parentToChildrenMap;
    std::unordered_map<StmtNum, std::unordered_set<StmtNum>> parentStarMap;
    std::unordered_map<StmtNum, std::unordered_set<StmtNum>> childStarMap;

    void computeTransitiveClosure();
};
