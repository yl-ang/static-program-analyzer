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

bool NextStore::hasNextRelationship(StmtNum s1, StmtNum s2) {
    auto it = nexterMap.find(s1);
    if (it != nexterMap.end()) {
        return it->second.count(s2) > 0;
    }
    return false;
}

bool NextStore::hasNextRelationship(ClauseArgument& arg1, ClauseArgument& arg2) {
    if (arg1.isWildcard() & arg2.isWildcard()) {
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
