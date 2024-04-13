#pragma once

#include "Relationship.h"
#include "pkb/PKBClient/PKBFacadeReader.h"
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

    AffectsSet generateAffectsRelation(PKBFacadeReader&);
    void generateAffectsfromAffector(AffectsSet& result, StmtNum& affectorStmtNum, PKBFacadeReader& reader);
    void generateAffectsfromAffected(AffectsSet& result, StmtNum& affectedStmtNum, PKBFacadeReader& reader);

    bool isAffectsfromAffector(StmtNum& affectorStmtNum, PKBFacadeReader& reader);
    bool intAffectsfromAffector(StmtNum& affectorStmtNum, StmtNum& affectedStmtNum, PKBFacadeReader& reader);
    bool isAffectsfromAffected(StmtNum& affectedStmtNum, PKBFacadeReader& reader);

    bool checkSynonym(std::shared_ptr<ClauseArgument> clauseArgument);
    bool checkAssign(std::shared_ptr<Synonym> synonym);
    static bool hasCommonValue(const std::unordered_set<Variable>&, const std::unordered_set<Variable>&);

    template <typename Func>
    friend void processAffects(Func func, StmtNum affectorStmtNum, PKBFacadeReader reader);
    static void handleCommonAffectorLogic(StmtNum& stmtNum, std::unordered_set<Variable>& modifiedVariables,
                                          StatementType& stmtType, PKBFacadeReader& reader, std::vector<StmtNum>& stack,
                                          std::unordered_set<StmtNum>& visited);
    template <typename Func>
    friend void processAffected(Func func, StmtNum affectedStmtNum, PKBFacadeReader reader);
    static void handleCommonAffectedLogic(StmtNum& stmtNum, std::unordered_set<Variable>& modifiedVariables,
                                          StatementType& stmtType, PKBFacadeReader& reader, std::vector<StmtNum>& stack,
                                          std::unordered_set<StmtNum>& visited);

public:
    Affects(std::shared_ptr<ClauseArgument> affector, std::shared_ptr<ClauseArgument> affected);

    ClauseResult evaluate(PKBFacadeReader&, EvaluationDb&) override;
    bool validateArguments() override;
};
