#pragma once

#include "PKB/PKBClient/PKBFacadeReader.h"
#include "Relationship.h"
#include "baseClasses/BaseCalls.h"
#include "qps/clauseArguments/ClauseArgument.h"

class Calls : public BaseCalls {
public:
    Calls(std::shared_ptr<ClauseArgument> caller, std::shared_ptr<ClauseArgument> callee) : BaseCalls(caller, callee) {}

private:
    bool hasCallRelationship(PKBFacadeReader& reader) override {
        return reader.hasCallRelationship(*caller, *callee);
    }

    ProcedureSet getCallee(PKBFacadeReader& reader, const Procedure& caller) override {
        return reader.getCallee(caller);
    }

    ProcedureSet getCaller(PKBFacadeReader& reader, const Procedure& callee) override {
        return reader.getCaller(callee);
    }
};
