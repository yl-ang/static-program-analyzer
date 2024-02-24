#pragma once

#include <unordered_map>
#include <unordered_set>

using StmtNum = int;

class TransitiveClosureUtility {
public:
    static void computeTransitiveClosure(
        std::unordered_map<StmtNum, std::unordered_set<StmtNum>>& starMap);
    static void updateTransitiveClosure(
        StmtNum s1, StmtNum s2,
        std::unordered_map<StmtNum, std::unordered_set<StmtNum>>& starMap);
};
