#pragma once

#include <string>
#include <unordered_set>
#include <utility>

#include "././PKB/PKBClient/PKBFacadeWriter.h"
#include "EntityExtractor.h"
#include "FollowsExtractor.h"
#include "ModifiesExtractor.h"
#include "ParentExtractor.h"
#include "PatternExtractor.h"
#include "UsesExtractor.h"

class DesignExtractor {
public:
    DesignExtractor() {}

    void extract(ASTNode* root);

    void writePKB(PKBFacadeWriter* pkbWriter);

    void dfsVisit(ASTNode* node, AstVisitor* visitor);

private:
    // PKBFacadeWriter* PKBwriter;
    EntityExtractor* entityExtractor;
    FollowsExtractor* followsExtractor;
    ParentExtractor* parentExtractor;
    UsesExtractor* usesExtractor;
    ModifiesExtractor* modifiesExtractor;
    PatternExtractor* patternExtractor;

    std::unordered_set<std::string> variables;
    std::unordered_set<std::string> constants;
    std::unordered_set<std::string> procedures;
    std::unordered_set<Stmt> statements;
    std::unordered_set<std::pair<StmtNum, StmtNum>> follows;
    std::unordered_set<std::pair<StmtNum, StmtNum>> parent;
    std::unordered_set<std::pair<StmtNum, Variable>> modifies;
    std::unordered_set<std::pair<StmtNum, Variable>> uses;
    std::unordered_set<std::pair<StmtNum, std::pair<std::string, std::string>>> pattern;
};
