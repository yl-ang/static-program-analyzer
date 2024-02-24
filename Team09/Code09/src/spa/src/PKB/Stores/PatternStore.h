#pragma once

#include <qps/clauseArguments/ClauseArgument.h>

#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "PKB/Utils/DataTypes.h"

class PatternStore {
public:
    // setPatterns will take in a set containing pairs of stmtNum and nested pair (left hand side variable,
    // and right hand side expression)
    void setPatterns(const std::unordered_set<std::pair<StmtNum, std::pair<std::string, std::string>>>& patterns);

    bool hasPattern(StmtNum stmtNum, std::string lhs, std::string rhs);

    // hasPattern checks if the given stmtNum and arg1 (lhs) and arg2 (rhs) exists in patternsMap
    bool hasPattern(StmtNum stmtNum, ClauseArgument& arg1, ClauseArgument& arg2);

private:
    std::unordered_map<StmtNum, std::unordered_set<std::pair<std::string, std::string>>> patternsMap;
};
