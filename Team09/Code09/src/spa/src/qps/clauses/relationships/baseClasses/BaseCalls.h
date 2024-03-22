#pragma once

#include "qps/clauses/relationships/Relationship.h"

using ProcedureSet = const std::unordered_set<Procedure>&;

class BaseCalls : public Relationship {
protected:
    ClauseArgument& caller;
    ClauseArgument& callee;

private:
    bool isSimpleResult() const;

    ClauseResult evaluateCalleeSynonym(PKBFacadeReader&);
    ClauseResult evaluateCallerSynonym(PKBFacadeReader&);
    ClauseResult evaluateBothSynonyms(PKBFacadeReader&);

    virtual bool hasCallRelationship(PKBFacadeReader&) = 0;
    virtual ProcedureSet getCallee(PKBFacadeReader&, const Procedure&) = 0;
    virtual ProcedureSet getCaller(PKBFacadeReader&, const Procedure&) = 0;

public:
    BaseCalls(ClauseArgument&, ClauseArgument&);

    ClauseResult evaluate(PKBFacadeReader&) override;
    bool validateArguments() override;
};
