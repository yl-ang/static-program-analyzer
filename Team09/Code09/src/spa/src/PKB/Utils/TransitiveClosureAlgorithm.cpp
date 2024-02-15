#include "TransitiveClosureAlgorithm.h"
#include <algorithm>

void TransitiveClosureAlgorithm::initializeMatrix() {
    transitiveClosureMatrix.assign(maxStmtNum + 1, std::vector<bool>(maxStmtNum + 1, false));
}

void TransitiveClosureAlgorithm::computeTransitiveClosure(
        const std::unordered_set<std::pair<int, int>>& relationships) {
    maxStmtNum = 0;
    for (const auto& relationship : relationships) {
        maxStmtNum = static_cast<size_t>(std::max(maxStmtNum, std::max(static_cast<size_t>(relationship.first),
                                                                       static_cast<size_t>(relationship.second))));
    }

    initializeMatrix();

    for (const auto& relationship : relationships) {
        transitiveClosureMatrix[relationship.first][relationship.second] = true;
    }

    for (int k = 1; k <= static_cast<int>(maxStmtNum); ++k) {
        for (int i = 1; i <= static_cast<int>(maxStmtNum); ++i) {
            for (int j = 1; j <= static_cast<int>(maxStmtNum); ++j) {
                transitiveClosureMatrix[i][j] = transitiveClosureMatrix[i][j] || (transitiveClosureMatrix[i][k]
                        && transitiveClosureMatrix[k][j]);
            }
        }
    }
}

bool TransitiveClosureAlgorithm::isTransitiveClosure(int s1, int s2) {
    if (s1 > static_cast<int>(maxStmtNum) || s2 > static_cast<int>(maxStmtNum)) {
        return false;  // Invalid statement numbers
    }

    return transitiveClosureMatrix[s1][s2];
}
