#pragma once

#include <queue>

#include "Relationship.h"
#include "baseClasses/BaseNext.h"
#include "pkb/PKBClient/PKBFacadeReader.h"
#include "qps/clauseArguments/ClauseArgument.h"
#include "qps/clauseArguments/Integer.h"
#include "qps/clauses/ClauseResult.h"

class NextStar : public BaseNext {
public:
    NextStar(std::shared_ptr<ClauseArgument> currentStmt, std::shared_ptr<ClauseArgument> nextStmt)
        : BaseNext(currentStmt, nextStmt) {}

private:
    static StmtSet findStars(PKBFacadeReader& reader, const StmtNum& sourceStmt, bool isGetNexter);
    static std::unordered_map<StmtNum, std::unordered_set<StmtNum>> getNextStarMap(PKBFacadeReader& reader);

    bool hasNextRelationship(PKBFacadeReader& reader) override;
    bool hasNextRelationship(PKBFacadeReader& reader, const StmtNum& curr, const StmtNum& next) override;
    StmtSet getNexters(PKBFacadeReader& reader, const StmtNum& nextee) override;
    StmtSet getNextees(PKBFacadeReader& reader, const StmtNum& nexter) override;
    ClauseResult evaluateBothSynonyms(PKBFacadeReader& reader, EvaluationDb& evalDb) override;
};
