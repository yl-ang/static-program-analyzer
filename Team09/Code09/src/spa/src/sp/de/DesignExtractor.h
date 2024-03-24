#pragma once

#include <memory>
#include <string>
#include <unordered_set>
#include <utility>

#include "././PKB/PKBClient/PKBFacadeWriter.h"
#include "CallsExtractor.h"
#include "EntityExtractor.h"
#include "FollowsExtractor.h"
#include "ModifiesExtractor.h"
#include "NextExtractor.h"
#include "ParentExtractor.h"
#include "PatternExtractor.h"
#include "UsesExtractor.h"

class DesignExtractor {
public:
    DesignExtractor() {}

    void extract(std::shared_ptr<ProgramNode> root);

    void writePKB(PKBFacadeWriter* pkbWriter);

    void dfsVisit(std::shared_ptr<ASTNode>&& node, AstVisitor* visitor);
    std::unordered_set<std::string> getVariables();
    std::unordered_set<std::string> getConstants();
    std::unordered_set<std::string> getProcedures();
    std::unordered_set<Stmt> getStatements();
    std::unordered_set<std::pair<StmtNum, StmtNum>> getFollows();
    std::unordered_set<std::pair<StmtNum, StmtNum>> getParent();
    std::unordered_set<std::pair<StmtNum, Variable>> getUses();
    std::unordered_set<std::pair<Procedure, Variable>> getProcedureUses();
    std::unordered_set<std::pair<StmtNum, Variable>> getModifies();
    std::unordered_set<std::pair<Procedure, Variable>> getProcedureModifies();
    std::unordered_set<std::pair<StmtNum, std::pair<std::string, std::string>>> getPattern();
    std::unordered_set<std::pair<StmtNum, StmtNum>> getNext();
    std::unordered_set<std::pair<Procedure, Procedure>> getCalls();
    std::unordered_set<std::pair<Procedure, StmtNum>> getCallStmts();
    std::unordered_set<std::pair<StmtNum, std::pair<std::string, std::string>>> getAssignmentPattern();
    std::unordered_set<std::pair<StmtNum, std::string>> getIfPattern();
    std::unordered_set<std::pair<StmtNum, std::string>> getWhilePattern();

private:
    ProcedureTracker* procedureTracker;
    EntityExtractor* entityExtractor;
    FollowsExtractor* followsExtractor;
    ParentExtractor* parentExtractor;
    UsesExtractor* usesExtractor;
    ModifiesExtractor* modifiesExtractor;
    PatternExtractor* patternExtractor;
    NextExtractor* nextExtractor;
    CallsExtractor* callsExtractor;
};
