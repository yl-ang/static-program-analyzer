#include "AssignPatternStore.h"

#include <functional>

void AssignPatternStore::initialiseStore(
    std::function<bool(std::string, std::string)> fp,
    std::unordered_set<std::pair<StmtNum, std::pair<std::string, std::string>>> patterns) {
    functionPointer = fp;
    setPatterns(patterns);
}

void AssignPatternStore::setPatterns(
    const std::unordered_set<std::pair<StmtNum, std::pair<std::string, std::string>>>& patterns) {
    for (const auto& pattern : patterns) {
        const auto& pairOfStrings = pattern.second;
        patternsMap[pattern.first].insert(pairOfStrings);
    }
}

bool AssignPatternStore::hasPattern(StmtNum stmtNum, std::string lhs, std::string rhs) {
    auto it = patternsMap.find(stmtNum);

    // Check if stmtNum is found
    if (it != patternsMap.end()) {
        // Retrieve the set associated with stmtNum
        const auto& pairSet = it->second;

        // Check if there is any pair in the set where the values match lhs and rhs
        for (const auto& pair : pairSet) {
            // Assuming StmtNum is an integer type
            if (pair.first == lhs && pair.second == rhs) {
                return true;
            }
        }
    }

    return false;
}

bool AssignPatternStore::hasPattern(StmtNum stmtNum, ClauseArgument& arg1, ClauseArgument& arg2) {
    if (arg1.isWildcard() & arg2.isWildcard()) {
        return !patternsMap[stmtNum].empty();
    }

    // if arg1 is wildcard, check if arg2 expression is found in any of the expressions for the stmtNum supplied
    if (arg1.isWildcard()) {
        const auto& rhsSet = patternsMap[stmtNum];

        // Check if any RHS in the set matches arg2.getValue()
        return std::any_of(rhsSet.begin(), rhsSet.end(),
                           [&](const auto& pair) { return functionPointer(pair.second, arg2.getValue()); });
    }

    // if arg2 is wildcard, check if variable arg1 contains an expression in patternsMap
    if (arg2.isWildcard()) {
        auto stmtNumIter = patternsMap.find(stmtNum);

        if (stmtNumIter != patternsMap.end()) {
            return std::any_of(stmtNumIter->second.begin(), stmtNumIter->second.end(),
                               [&](const auto& pair) { return functionPointer(pair.first, arg1.getValue()); });
        } else {
            return false;
        }
    }

    // if both are not wildcards, check if the pair of patternsMap.find(stmtNum) is the same as arg1.getValue and
    // arg2.value
    return hasPattern(stmtNum, arg1.getValue(), arg2.getValue());
}
