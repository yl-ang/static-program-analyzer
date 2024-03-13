#include "CallStore.h"

void CallStore::setCallStore(const std::unordered_set<std::pair<Procedure, Procedure>>& callPairs) {
    for (const auto& pair : callPairs) {
        Procedure caller = pair.first;
        Procedure callee = pair.second;

        callerMap[caller].insert(callee);
        calleeMap[callee].insert(caller);
    }

    // TODO(yl-ang): call* pre-computation
}

std::unordered_set<Procedure> CallStore::getCaller(Procedure callee) {
    if (callerMap.count(callee)) {
        return callerMap[callee];
    }
    return {};
}

std::unordered_set<Procedure> CallStore::getCallee(Procedure caller) {
    if (calleeMap.count(caller)) {
        return calleeMap[caller];
    }
    return {};
}

bool CallStore::hasCallRelationship(Procedure caller, Procedure callee) {
    auto it = callerMap.find(caller);
    if (it != callerMap.end()) {
        return it->second.count(callee) > 0;
    }
    return false;
}

bool CallStore::hasCallRelationship(ClauseArgument& arg1, ClauseArgument& arg2) {
    if (arg1.isWildcard() && arg2.isWildcard()) {
        return !(callerMap.empty());
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
