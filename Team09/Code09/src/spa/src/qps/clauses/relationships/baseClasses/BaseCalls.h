#pragma once

#include "qps/clauses/relationships/Relationship.h"

using ProcedureSet = std::unordered_set<Procedure>;

class BaseCalls : public Relationship {
protected:
    std::shared_ptr<ClauseArgument> caller;
    std::shared_ptr<ClauseArgument> callee;

private:
    bool isSimpleResult() const;

    ClauseResult evaluateSynonymWildcard(PKBFacadeReader&, EvaluationDb&);
    ClauseResult evaluateSynonymLiteral(PKBFacadeReader&, EvaluationDb&);
    ClauseResult evaluateBothSynonyms(PKBFacadeReader&, EvaluationDb&);

    virtual bool hasCallRelationship(PKBFacadeReader&) = 0;
    virtual ProcedureSet getCallee(PKBFacadeReader&, const Procedure&) = 0;
    virtual ProcedureSet getCaller(PKBFacadeReader&, const Procedure&) = 0;

public:
    BaseCalls(std::shared_ptr<ClauseArgument>, std::shared_ptr<ClauseArgument>);

    ClauseResult evaluate(PKBFacadeReader&, EvaluationDb&) override;
    bool validateArguments() override;
};
