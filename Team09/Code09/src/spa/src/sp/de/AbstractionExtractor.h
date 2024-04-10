#pragma once
#include <string>
#include <unordered_set>
#include <utility>

#include "../../PKB/PKBClient/PKBFacadeWriter.h"
#include "../../PKB/utils/DataTypes.h"
#include "AstVisitor.h"

class AbstractionExtractor : public AstVisitor {
public:
    AbstractionExtractor() {}

    std::unordered_set<std::pair<StmtNum, StmtNum>> follows;
    std::unordered_set<std::pair<StmtNum, StmtNum>> parent;
    std::unordered_set<std::pair<StmtNum, Variable>> modifies;
    std::unordered_set<std::pair<Procedure, Variable>> procedureModifies;
    std::unordered_set<std::pair<StmtNum, Variable>> uses;
    std::unordered_set<std::pair<Procedure, Variable>> procedureUses;
    std::unordered_set<std::pair<StmtNum, std::pair<std::string, std::string>>> pattern;
    std::unordered_set<std::pair<Procedure, Procedure>> calls;
    std::unordered_set<std::pair<Procedure, StmtNum>> callStmts;
    std::unordered_set<std::pair<StmtNum, std::pair<std::string, std::string>>> assignmentTraversals;
    std::unordered_set<std::pair<StmtNum, std::string>> variablesInIfConditionExpr;
    std::unordered_set<std::pair<StmtNum, std::string>> variablesInWhileConditionExpr;
};
