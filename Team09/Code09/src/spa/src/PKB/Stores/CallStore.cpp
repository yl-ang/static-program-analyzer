#include "CallStore.h"

void CallStore::setCallStore(const std::unordered_set<std::pair<Procedure, Procedure>>& callPairs) {
    for (const auto& pair : callPairs) {
        Procedure caller = pair.first;
        Procedure callee = pair.second;

        callerToCalleeMap[caller].insert(callee);
        calleeToCallerMap[callee].insert(caller);
    }

    computeTransitiveClosure();
}

void CallStore::computeTransitiveClosure() {
    callerToCalleeStarMap.clear();
    calleeToCallerStarMap.clear();

    // Initialize the matrices
    for (const auto& entry : calleeToCallerMap) {
        Procedure callee = entry.first;
        const auto& callers = entry.second;

        for (const auto& caller : callers) {
            // Initialize callerStarMap
            if (!callerToCalleeStarMap.count(caller)) {
                callerToCalleeStarMap[caller] = std::unordered_set<Procedure>();
            }
            // Initialize calleeStarMap
            if (!calleeToCallerStarMap.count(callee)) {
                calleeToCallerStarMap[callee] = std::unordered_set<Procedure>();
            }

            // Initialize the direct caller-callee relationships
            callerToCalleeStarMap[caller].insert(callee);
            calleeToCallerStarMap[callee].insert(caller);
        }
    }

    // Compute transitive closure
    TransitiveClosureUtility<Procedure>::computeTransitiveClosure(&callerToCalleeStarMap);
    TransitiveClosureUtility<Procedure>::computeTransitiveClosure(&calleeToCallerStarMap);
}

std::unordered_set<Procedure> CallStore::getCaller(Procedure callee) {
    if (calleeToCallerMap.count(callee)) {
        return calleeToCallerMap[callee];
    }
    return {};
}

std::unordered_set<Procedure> CallStore::getCallee(Procedure caller) {
    if (callerToCalleeMap.count(caller)) {
        return callerToCalleeMap[caller];
    }
    return {};
}

std::unordered_set<Procedure> CallStore::getCallerStar(Procedure callee) {
    return calleeToCallerStarMap.count(callee) ? calleeToCallerStarMap[callee] : std::unordered_set<Procedure>();
}

std::unordered_set<Procedure> CallStore::getCalleeStar(Procedure caller) {
    return callerToCalleeStarMap.count(caller) ? callerToCalleeStarMap[caller] : std::unordered_set<Procedure>();
}

bool CallStore::hasCallRelationship(Procedure caller, Procedure callee) {
    auto it = callerToCalleeMap.find(caller);
    if (it != callerToCalleeMap.end()) {
        return it->second.count(callee) > 0;
    }
    return false;
}

bool CallStore::hasCallRelationship(ClauseArgument& arg1, ClauseArgument& arg2) {
    if (arg1.isWildcard() && arg2.isWildcard()) {
        return !(callerToCalleeMap.empty());
    }

    // if arg 1 is wildcard, check if arg2 has callers(s)
    if (arg1.isWildcard()) {
        return !getCaller(arg2.getValue()).empty();
    }

    // if arg2 is wildcard, check if arg1 has callee(s)
    if (arg2.isWildcard()) {
        return !getCallee(arg1.getValue()).empty();
    }

    return hasCallRelationship(arg1.getValue(), arg2.getValue());
}

bool CallStore::hasCallStarRelationship(Procedure caller, Procedure callee) {
    return callerToCalleeStarMap[caller].count(callee) && calleeToCallerStarMap[callee].count(caller);
}

bool CallStore::hasCallStarRelationship(ClauseArgument& arg1, ClauseArgument& arg2) {
    if (arg1.isWildcard() && arg2.isWildcard()) {
        return !callerToCalleeMap.empty();
    }

    // if arg 1 is wildcard, check if arg2 has caller Stars
    if (arg1.isWildcard()) {
        return !getCallerStar(arg2.getValue()).empty();
    }

    // if arg 2 is wildcard, check if arg1 has callee Stars
    if (arg2.isWildcard()) {
        return !getCalleeStar(arg1.getValue()).empty();
    }

    return hasCallStarRelationship(arg1.getValue(), arg2.getValue());
}
