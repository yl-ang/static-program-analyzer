#pragma once

#include "PKB/PKBClient/PKBFacadeReader.h"
#include "Relationship.h"
#include "baseClasses/BaseNext.h"
#include "qps/clauseArguments/ClauseArgument.h"
#include "qps/clauseArguments/Integer.h"
#include "qps/clauses/ClauseResult.h"

class NextStar : public BaseNext {
public:
    NextStar(std::shared_ptr<ClauseArgument> currentStmt, std::shared_ptr<ClauseArgument> nextStmt)
        : BaseNext(currentStmt, nextStmt) {}

private:
    bool hasNextRelationship(PKBFacadeReader& reader) override {
        return reader.hasNextStarRelationship(*currentStmt, *nextStmt);
    }

    StmtSet getNexters(PKBFacadeReader& reader, const StmtNum& nextee) override {
        return reader.getNexterStar(nextee);
    }

    StmtSet getNextees(PKBFacadeReader& reader, const StmtNum& nexter) override {
        return reader.getNexteeStar(nexter);
    }
};
