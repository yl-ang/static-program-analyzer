#pragma once

#include <qps/clauseArguments/ClauseArgument.h>

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "PKB/Utils/DataTypes.h"
#include "PKB/Utils/TransitiveClosureUtility.h"

class CallStore {
public:
    void setCallStore(const std::unordered_set<std::pair<Procedure, Procedure>>& callPairs);
    std::unordered_set<Procedure> getCaller(Procedure callee);
    std::unordered_set<Procedure> getCallee(Procedure caller);
    bool hasCallRelationship(Procedure caller, Procedure callee);
    bool hasCallRelationship(ClauseArgument& arg1, ClauseArgument& arg2);

private:
    // Each caller (Procedure) can have 0 or more direct callees (Procedure)
    std::unordered_map<Procedure, std::unordered_set<Procedure>> callerMap;
    // Each callee (Procedure) can only have 0 or more direct callers (Procedure)
    std::unordered_map<Procedure, std::unordered_set<Procedure>> calleeMap;
};
