#pragma once
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "../../PKB/PKBClient/PKBFacadeWriter.h"
#include "../../PKB/utils/DataTypes.h"
#include "AstVisitor.h"

class AbstractionExtractor : public AstVisitor {
private:
public:
    AbstractionExtractor() {}

    std::unordered_set<std::pair<StmtNum, StmtNum>> follows;
    std::unordered_set<std::pair<StmtNum, StmtNum>> parent;
    std::unordered_set<std::pair<StmtNum, Variable>> modifies;
    std::unordered_set<std::pair<StmtNum, Variable>> uses;
    std::unordered_set<std::pair<StmtNum, std::pair<std::string, std::string>>> pattern;
};
