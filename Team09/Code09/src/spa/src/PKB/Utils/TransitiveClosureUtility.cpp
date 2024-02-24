#include "TransitiveClosureUtility.h"

void TransitiveClosureUtility::computeTransitiveClosure(
    std::unordered_map<StmtNum, std::unordered_set<StmtNum>>& starMap) {
    for (const auto& entry : starMap) {
        StmtNum s1 = entry.first;
        for (const auto& s2 : entry.second) {
            updateTransitiveClosure(s1, s2, starMap);
        }
    }
}

void TransitiveClosureUtility::updateTransitiveClosure(
    StmtNum s1, StmtNum s2,
    std::unordered_map<StmtNum, std::unordered_set<StmtNum>>& starMap) {
    if (starMap.count(s2)) {
        for (const auto& s3 : starMap[s2]) {
            starMap[s1].insert(s3);
        }
    }
}
