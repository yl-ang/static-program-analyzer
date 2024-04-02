#pragma once

#include "PKB/PKBClient/PKBFacadeReader.h"
#include "Relationship.h"
#include "baseClasses/BaseCalls.h"
#include "qps/clauseArguments/ClauseArgument.h"
#include "qps/clauseArguments/Integer.h"
#include "qps/clauses/ClauseResult.h"

class CallsStar : public BaseCalls {
public:
    CallsStar(std::shared_ptr<ClauseArgument> caller, std::shared_ptr<ClauseArgument> callee)
        : BaseCalls(caller, callee) {}

private:
    bool hasCallRelationship(PKBFacadeReader& reader) override {
        return reader.hasCallStarRelationship(*caller, *callee);
    }

    ProcedureSet getCallee(PKBFacadeReader& reader, const Procedure& caller) override {
        return reader.getCalleeStar(caller);
    }

    ProcedureSet getCaller(PKBFacadeReader& reader, const Procedure& callee) override {
        return reader.getCallerStar(callee);
    }
};
