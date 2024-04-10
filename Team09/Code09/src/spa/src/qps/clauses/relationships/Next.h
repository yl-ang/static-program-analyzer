#pragma once

#include "PKB/PKBClient/PKBFacadeReader.h"
#include "Relationship.h"
#include "baseClasses/BaseNext.h"
#include "qps/clauseArguments/ClauseArgument.h"
#include "qps/clauseArguments/Integer.h"
#include "qps/clauses/ClauseResult.h"

class Next : public BaseNext {
public:
    Next(std::shared_ptr<ClauseArgument> currentStmt, std::shared_ptr<ClauseArgument> nextStmt)
        : BaseNext(currentStmt, nextStmt) {}

private:
    bool hasNextRelationship(PKBFacadeReader& reader) override {
        return reader.hasNextRelationship(*currentStmt, *nextStmt);
    }

    bool hasNextRelationship(PKBFacadeReader& reader, const StmtNum& curr, const StmtNum& next) override {
        return reader.hasNextRelationship(curr, next);
    }

    StmtSet getNexters(PKBFacadeReader& reader, const StmtNum& nextee) override {
        return reader.getNexter(nextee);
    }

    StmtSet getNextees(PKBFacadeReader& reader, const StmtNum& nexter) override {
        return reader.getNextee(nexter);
    }
};
