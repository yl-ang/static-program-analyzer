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

std::vector<StmtNum> ParentStore::getParents(StmtNum child) {
    std::vector<StmtNum> parents;
    auto it = parentMap.find(child);
    while (it != parentMap.end()) {
        parents.push_back(it->second);
        it = parentMap.find(it->second);
    }
    return parents;
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

void ParentStore::computeTransitiveClosure() {
    parentStarMap.clear();
    childStarMap.clear();

    for (const auto& entry : parentMap) {
        StmtNum parent = entry.second;
        StmtNum child = entry.first;

        if (!parentStarMap.count(child)) {
            parentStarMap[child] = std::unordered_set<StmtNum>();
        }

        parentStarMap[child].insert(parent);

        if (!childStarMap.count(parent)) {
            childStarMap[parent] = std::unordered_set<StmtNum>();
        }

        childStarMap[parent].insert(child);

        for (const auto& grandparent : parentStarMap[parent]) {
            updateTransitiveClosure(grandparent, child);
        }

        for (const auto& grandchild : childStarMap[child]) {
            updateTransitiveClosure(parent, grandchild);
        }
    }
}

void ParentStore::updateTransitiveClosure(StmtNum parent, StmtNum child) {
    if (parentStarMap.count(child)) {
        for (const auto& grandparent : parentStarMap[child]) {
            parentStarMap[parent].insert(grandparent);
        }
    }

    if (childStarMap.count(parent)) {
        for (const auto& grandchild : childStarMap[parent]) {
            childStarMap[child].insert(grandchild);
        }
    }
}
