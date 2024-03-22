#pragma once

#include "PKB/PKBClient/PKBFacadeReader.h"
#include "Relationship.h"
#include "baseClasses/BaseCalls.h"
#include "qps/clauseArguments/ClauseArgument.h"
#include "qps/clauseArguments/Integer.h"
#include "qps/clauses/ClauseResult.h"

class Calls : public BaseCalls {
private:
    bool hasCallRelationship(PKBFacadeReader&) override;
    ProcedureSet getCallee(PKBFacadeReader&, const Procedure&) override;
    ProcedureSet getCaller(PKBFacadeReader&, const Procedure&) override;

public:
    Calls(ClauseArgument&, ClauseArgument&);
};
