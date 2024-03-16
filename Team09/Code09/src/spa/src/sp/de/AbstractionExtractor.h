#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "../../PKB/PKBClient/PKBFacadeWriter.h"
#include "../../PKB/utils/DataTypes.h"
#include "AstVisitor.h"
#include "sp/ast/Matchable.h"

class AbstractionExtractor : public AstVisitor {
private:
public:
    AbstractionExtractor() {}

    std::unordered_set<std::pair<StmtNum, StmtNum>> follows;
    std::unordered_set<std::pair<StmtNum, StmtNum>> parent;
    std::unordered_set<std::pair<StmtNum, Variable>> modifies;
    std::unordered_set<std::pair<Procedure, Variable>> procedureModifies;
    std::unordered_set<std::pair<StmtNum, Variable>> uses;
    std::unordered_set<std::pair<Procedure, Variable>> procedureUses;
    std::unordered_set<std::pair<StmtNum, std::pair<std::string, std::string>>> pattern;
    std::unordered_map<StmtNum, std::pair<std::string, std::shared_ptr<Matchable>>> matchablePattern;
    std::unordered_set<std::pair<Procedure, Procedure>> calls;
};
