#pragma once

#include <qps/clauseArguments/ClauseArgument.h>

#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "PKB/Utils/DataTypes.h"

class AssignPatternStore {
public:
    void initialiseStore(std::function<bool(std::string, std::string)> exactMatchFP,
                         std::function<bool(std::string, std::string)> partialMatchFP,
                         std::unordered_set<std::pair<StmtNum, std::pair<std::string, std::string>>> input);
    // setPatterns will take in a set containing pairs of stmtNum and nested pair (left hand side variable,
    // and right hand side expression)

    bool hasExactPatternHelper(StmtNum stmtNum, std::string lhs, std::string rhs);
    bool hasPartialPatternHelper(StmtNum stmtNum, std::string lhs, std::string rhs);
    bool hasExactPattern(StmtNum stmtNum, std::string arg1, std::string arg2);
    bool hasPartialPattern(StmtNum stmtNum, std::string arg1, std::string arg2);

private:
    std::unordered_map<StmtNum, std::unordered_set<std::pair<std::string, std::string>>> patternsMap;
    void setPatterns(const std::unordered_set<std::pair<StmtNum, std::pair<std::string, std::string>>>& patterns);
    void setFunctionPointer(const std::function<bool(std::string, std::string)>);

    std::function<bool(std::string, std::string)> exactMatchFP;
    std::function<bool(std::string, std::string)> partialMatchFP;
};
