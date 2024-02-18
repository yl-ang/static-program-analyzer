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

// TODO(yl-ang): BUGGY
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

    // Update the matrices with transitive closure
    for (const auto& entry : parentStarMap) {
        StmtNum child = entry.first;
        for (const auto& parent : entry.second) {
            updateTransitiveClosure(parent, child);
        }
    }
}

// TODO(yl-ang): BUGGY
void ParentStore::updateTransitiveClosure(StmtNum parent, StmtNum child) {
    if (parentStarMap.count(child)) {
        for (const auto& grandparent : parentStarMap[child]) {
            parentStarMap[parent].insert(grandparent);

            // Also update the child matrix
            childStarMap[grandparent].insert(child);
        }
    }

    if (childStarMap.count(parent)) {
        for (const auto& grandchild : childStarMap[parent]) {
            childStarMap[child].insert(grandchild);

            // Also update the parent matrix
            parentStarMap[grandchild].insert(parent);
        }
    }
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
