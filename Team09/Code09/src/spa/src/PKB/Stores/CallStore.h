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
    std::unordered_map<Procedure, std::unordered_set<Procedure>> callerMap;
    std::unordered_map<Procedure, std::unordered_set<Procedure>> calleeMap;
    std::unordered_map<Procedure, std::unordered_set<Procedure>> callerStarMap;
    std::unordered_map<Procedure, std::unordered_set<Procedure>> calleeStarMap;

    void computeTransitiveClosure();
};
