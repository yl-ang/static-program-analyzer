#pragma once

#include "PKB/PKBClient/PKBFacadeReader.h"
#include "Relationship.h"
#include "baseClasses/BaseNext.h"
#include "qps/clauseArguments/ClauseArgument.h"
#include "qps/clauseArguments/Integer.h"
#include "qps/clauses/ClauseResult.h"

class NextStar : public BaseNext {
public:
    NextStar(ClauseArgument&, ClauseArgument&);

    bool hasNextRelationship(PKBFacadeReader&) override;
    StmtSet getNexters(PKBFacadeReader&, const StmtNum&) override;
    StmtSet getNextees(PKBFacadeReader&, const StmtNum&) override;
};
