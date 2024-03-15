#pragma once

#include "QueryClause.h"
#include "qps/clauseArguments/ExpressionSpec.h"
#include "qps/clauseArguments/Synonym.h"

class PatternClause : public QueryClause {
private:
    ClauseArgument& assignSynonym;
    ClauseArgument& firstArg;
    ClauseArgument& secondArg;

    ClauseResult evaluateArgSyn(PKBFacadeReader&);
    ClauseResult evaluateArgNonSyns(PKBFacadeReader&);

public:
    PatternClause(ClauseArgument* assign, ClauseArgument* f, ClauseArgument* s);
    bool equals(const QueryClause&) const override;
    ClauseResult evaluate(PKBFacadeReader&) override;
    bool isBooleanResult() const override;
    bool containsSynonym(const Synonym&) const override;
    std::vector<Synonym> getSynonyms() const override;
    void checkSemantic(SynonymStore* store) override;
};
