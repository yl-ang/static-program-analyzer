#include "AssignPatternStore.h"

#include <algorithm>
#include <functional>
void AssignPatternStore::initialiseStore(
    std::function<bool(std::string, std::string)> exactMatchFP,
    std::function<bool(std::string, std::string)> partialMatchFP,
    std::unordered_set<std::pair<StmtNum, std::pair<std::string, std::string>>> patterns) {
    this->exactMatchFP = exactMatchFP;
    this->partialMatchFP = partialMatchFP;
    setPatterns(patterns);
}

void AssignPatternStore::setPatterns(
    const std::unordered_set<std::pair<StmtNum, std::pair<std::string, std::string>>>& patterns) {
    for (const auto& pattern : patterns) {
        const auto& pairOfStrings = pattern.second;
        patternsMap[pattern.first].insert(pairOfStrings);
    }
}

bool AssignPatternStore::hasExactPatternHelper(StmtNum stmtNum, std::string lhs, std::string rhs) {
    auto it = patternsMap.find(stmtNum);

    // Check if stmtNum is found
    if (it != patternsMap.end()) {
        // Retrieve the set associated with stmtNum
        const auto& pairSet = it->second;

        // Check if there is any pair in the set where the values match lhs and rhs
        for (const auto& pair : pairSet) {
            // Assuming StmtNum is an integer type
            if (exactMatchFP(pair.first, lhs) && exactMatchFP(pair.second, rhs)) {
                return true;
            }
        }
    }

    return false;
}

bool AssignPatternStore::hasPartialPatternHelper(StmtNum stmtNum, std::string lhs, std::string rhs) {
    auto it = patternsMap.find(stmtNum);

    if (it != patternsMap.end()) {
        const auto& pairSet = it->second;

        for (const auto& pair : pairSet) {
            if (partialMatchFP(pair.first, lhs) && partialMatchFP(pair.second, rhs)) {
                return true;
            }
        }
    }
    return false;
}

bool AssignPatternStore::hasExactPattern(StmtNum stmtNum, std::string arg1, std::string arg2) {
    // if arg1 is wildcard, check if arg2 expression is found in any of the expressions for the stmtNum supplied
    if (arg1 == "_") {
        const auto& rhsSet = patternsMap[stmtNum];

        // Check if any RHS in the set matches arg2.getValue()
        return std::any_of(rhsSet.begin(), rhsSet.end(),
                           [&](const auto& pair) { return exactMatchFP(pair.second, arg2); });
    }

    // if arg2 is wildcard, check if variable arg1 contains an expression in patternsMap
    if (arg2 == "_") {
        auto stmtNumIter = patternsMap.find(stmtNum);

        if (stmtNumIter != patternsMap.end()) {
            return std::any_of(stmtNumIter->second.begin(), stmtNumIter->second.end(),
                               [&](const auto& pair) { return exactMatchFP(pair.first, arg1); });
        } else {
            return false;
        }
    }

    // if both are not wildcards, we want to call the fp
    return hasExactPatternHelper(stmtNum, arg1, arg2);
}

bool AssignPatternStore::hasPartialPattern(StmtNum stmtNum, std::string arg1, std::string arg2) {
    // if arg1 is wildcard, check if arg2 expression is found in any of the expressions for the stmtNum supplied
    if (arg1 == "_") {
        const auto& rhsSet = patternsMap[stmtNum];

        // Check if any RHS in the set matches arg2.getValue()
        return std::any_of(rhsSet.begin(), rhsSet.end(),
                           [&](const auto& pair) { return partialMatchFP(pair.second, arg2); });
    }

    // if arg2 is wildcard, check if variable arg1 contains an expression in patternsMap
    if (arg2 == "_") {
        auto stmtNumIter = patternsMap.find(stmtNum);

        if (stmtNumIter != patternsMap.end()) {
            return std::any_of(stmtNumIter->second.begin(), stmtNumIter->second.end(),
                               [&](const auto& pair) { return partialMatchFP(pair.first, arg1); });
        } else {
            return false;
        }
    }

    // if both are not wildcards, we want to call the fp
    return hasPartialPatternHelper(stmtNum, arg1, arg2);
}
