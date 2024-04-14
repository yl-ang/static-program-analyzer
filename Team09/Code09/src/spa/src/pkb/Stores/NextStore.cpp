#include "NextStore.h"

void NextStore::setNextStore(const std::unordered_set<std::pair<StmtNum, StmtNum>>& nextSet) {
    for (const auto& pair : nextSet) {
        StmtNum nextee = pair.first;
        StmtNum nexter = pair.second;

        nexterMap[nextee].insert(nexter);
        nexteeMap[nexter].insert(nextee);
    }
}

std::unordered_set<StmtNum> NextStore::getNexter(StmtNum nextee) {
    if (nexterMap.count(nextee)) {
        return nexterMap[nextee];
    }
    return {};
}

std::unordered_set<StmtNum> NextStore::getNextee(StmtNum nexter) {
    if (nexteeMap.count(nexter)) {
        return nexteeMap[nexter];
    }
    return {};
}

std::unordered_set<StmtNum> NextStore::getNexterStar(StmtNum nextee) {
    return computeNextStar(nextee, nexterMap);
}

std::unordered_set<StmtNum> NextStore::getNexteeStar(StmtNum nexter) {
    return computeNextStar(nexter, nexteeMap);
}

bool NextStore::hasNextRelationship(StmtNum s1, StmtNum s2) {
    auto it = nexterMap.find(s1);
    if (it != nexterMap.end()) {
        return it->second.count(s2) > 0;
    }
    return false;
}

bool NextStore::hasNextStarRelationship(StmtNum s1, StmtNum s2) {
    auto nexterSet = computeNextStar(s1, nexterMap);
    return nexterSet.find(s2) != nexterSet.end();
}

bool NextStore::hasNextRelationship(ClauseArgument& arg1, ClauseArgument& arg2) {
    if (arg1.isWildcard() && arg2.isWildcard()) {
        return !(nexterMap.empty());
    }

    // if arg 1 is wildcard, check if arg2 has nextee(s)
    if (arg1.isWildcard()) {
        return !getNextee(std::stoi(arg2.getValue())).empty();
    }

    // if arg2 is wildcard, check if arg1 has nexters(s)
    if (arg2.isWildcard()) {
        return !getNexter(std::stoi(arg1.getValue())).empty();
    }

    return hasNextRelationship(std::stoi(arg1.getValue()), std::stoi(arg2.getValue()));
}

bool NextStore::hasNextStarRelationship(ClauseArgument& arg1, ClauseArgument& arg2) {
    if (arg1.isWildcard() && arg2.isWildcard()) {
        return !(nexterMap.empty());
    }

    // if arg 1 is wildcard, check if arg2 has nexteeStar
    if (arg1.isWildcard()) {
        return !getNexteeStar(std::stoi(arg2.getValue())).empty();
    }

    // if arg2 is wildcard, check if arg1 has nextersStar
    if (arg2.isWildcard()) {
        return !getNexterStar(std::stoi(arg1.getValue())).empty();
    }

    return hasNextStarRelationship(std::stoi(arg1.getValue()), std::stoi(arg2.getValue()));
}

std::unordered_set<StmtNum> NextStore::computeNextStar(
    StmtNum start, const std::unordered_map<StmtNum, std::unordered_set<StmtNum>>& nextMap) {
    std::unordered_set<StmtNum> result;

    std::unordered_map<StmtNum, std::unordered_set<StmtNum>> closureMap = nextMap;
    TransitiveClosureUtility<StmtNum>::computeTransitiveClosure(&closureMap);

    auto it = closureMap.find(start);
    if (it != closureMap.end()) {
        result.insert(it->second.begin(), it->second.end());
        for (StmtNum node : it->second) {
            if (closureMap[node].find(start) != closureMap[node].end()) {
                result.insert(start);
                break;
            }
        }
    }

    return result;
}

std::unordered_map<StmtNum, std::unordered_set<StmtNum>> NextStore::getNextStarMap() {
    std::unordered_map<StmtNum, std::unordered_set<StmtNum>> closureMap = nexterMap;
    TransitiveClosureUtility<StmtNum>::computeTransitiveClosure(&closureMap);
    std::unordered_map<StmtNum, std::unordered_set<StmtNum>> resultMap = closureMap;

    for (const auto& entry : closureMap) {
        auto& set = resultMap[entry.first];

        for (StmtNum node : entry.second) {
            if (closureMap[node].find(entry.first) != closureMap[node].end()) {
                set.insert(entry.first);
                break;
            }
        }
    }
    return resultMap;
}
