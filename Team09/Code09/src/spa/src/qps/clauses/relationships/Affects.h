#pragma once

#include "PKB/PKBClient/PKBFacadeReader.h"
#include "Relationship.h"
#include "qps/clauseArguments/ClauseArgument.h"
#include "qps/clauseArguments/Integer.h"
#include "qps/clauses/ClauseResult.h"

// Define AffectsSet
using AffectsSet = std::unordered_set<std::pair<StmtNum, StmtNum>>;

class Affects : public Relationship {
private:
    std::shared_ptr<ClauseArgument> affector;
    std::shared_ptr<ClauseArgument> affected;

    ClauseResult evaluateSynonymInteger(PKBFacadeReader&);
    ClauseResult evaluateBothIntegers(PKBFacadeReader&);
    ClauseResult evaluateBothWildcards(PKBFacadeReader&);
    ClauseResult evaluateWildcardInteger(PKBFacadeReader&);
    ClauseResult evaluateSynonymWildcard(PKBFacadeReader&);
    ClauseResult evaluateBothSynonyms(PKBFacadeReader&);

    AffectsSet generateAffectsRelation(PKBFacadeReader& reader);
    void generateAffectsfromAffector(AffectsSet&, StmtNum&, PKBFacadeReader&);
    void generateAffectsfromAffected(AffectsSet& result, StmtNum& affectedStmtNum, PKBFacadeReader& reader);
    bool isAffectsfromAffector(StmtNum& affectorStmtNum, PKBFacadeReader& reader);
    bool intAffectsfromAffector(StmtNum& affectorStmtNum, StmtNum& affectedStmtNum, PKBFacadeReader& reader);
    
    bool isAffectsfromAffected(StmtNum& affectedStmtNum, PKBFacadeReader& reader);
    
    bool checkSynonym(std::shared_ptr<ClauseArgument> clauseArgument);
    bool checkAssign(std::shared_ptr<Synonym> synonym);
    

public:
    Affects(std::shared_ptr<ClauseArgument> affector, std::shared_ptr<ClauseArgument> affected);
    static void handleCommonAffectedLogic(StmtNum& stmtNum, std::unordered_set<Variable>& modifiedVariables,
                                   StatementType& stmtType, PKBFacadeReader& reader, std::vector<StmtNum>& stack,
                                   std::unordered_set<StmtNum>& visited);
    static void handleCommonAffectorLogic(StmtNum& stmtNum, std::unordered_set<Variable>& modifiedVariables,
                                    StatementType& stmtType, PKBFacadeReader& reader,
                                    std::vector<StmtNum>& stack, std::unordered_set<StmtNum>& visited);
    ClauseResult evaluate(PKBFacadeReader&) override;
    static bool hasCommonValue(const std::unordered_set<Variable>&, const std::unordered_set<Variable>&);
    bool validateArguments() override;
};
