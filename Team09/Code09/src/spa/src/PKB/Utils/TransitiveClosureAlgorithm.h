#pragma once

#include <unordered_set>
#include <vector>

class TransitiveClosureAlgorithm {
public:
    void computeTransitiveClosure(const std::unordered_set<std::pair<int, int>>& relationships);

    bool isTransitiveClosure(int s1, int s2);

private:
    std::vector<std::vector<bool>> transitiveClosureMatrix;
    size_t maxStmtNum;

    void initializeMatrix();
};
