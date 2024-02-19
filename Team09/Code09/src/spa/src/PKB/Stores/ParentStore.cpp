#include "ParentStore.h"

void ParentStore::setParentStore(
    const std::unordered_set<std::pair<StmtNum, StmtNum>>& parentPairs) {
    for (const auto& pair : parentPairs) {
        StmtNum parent = pair.first;
        StmtNum child = pair.second;
        parentMap[child] = parent;
        parentToChildrenMap[parent].insert(child);
    }

    computeTransitiveClosure();
}

void ParentStore::computeTransitiveClosure() {
    parentStarMap.clear();
    childStarMap.clear();

    // Initialize the matrices
    for (const auto& entry : parentMap) {
        StmtNum parent = entry.second;
        StmtNum child = entry.first;

        // Ensure that the direct parent is not included in the transitive
        // closure set
        if (!parentStarMap.count(child)) {
            parentStarMap[child] = std::unordered_set<StmtNum>();
        }

        // Initialize the child matrix
        if (!childStarMap.count(parent)) {
            childStarMap[parent] = std::unordered_set<StmtNum>();
        }

        // Initialize the direct parent-child relationships
        parentStarMap[child].insert(parent);
        childStarMap[parent].insert(child);
    }

    // Use TransitiveClosureUtility to update the matrices with transitive
    // closure
    TransitiveClosureUtility::computeTransitiveClosure(parentStarMap);
    TransitiveClosureUtility::computeTransitiveClosure(childStarMap);
}

std::optional<StmtNum> ParentStore::getParent(StmtNum child) {
    auto it = parentMap.find(child);
    if (it != parentMap.end()) {
        return it->second;
    }
    return std::nullopt;
}

std::unordered_set<StmtNum> ParentStore::getChildren(StmtNum parent) {
    if (parentToChildrenMap.count(parent)) {
        return parentToChildrenMap[parent];
    }
    return {};
}

std::unordered_set<StmtNum> ParentStore::getChildrenStar(StmtNum parent) {
    return childStarMap.count(parent) ? childStarMap[parent]
                                      : std::unordered_set<StmtNum>();
}

std::unordered_set<StmtNum> ParentStore::getParentsStar(StmtNum child) {
    return parentStarMap.count(child) ? parentStarMap[child]
                                      : std::unordered_set<StmtNum>();
}

bool ParentStore::containsParentRelationship(StmtNum parent, StmtNum child) {
    auto it = parentMap.find(child);
    return it != parentMap.end() && it->second == parent;
}

bool ParentStore::containsParentStarRelationship(StmtNum parent,
                                                 StmtNum child) {
    return parentStarMap.count(child) && parentStarMap[child].count(parent);
}
