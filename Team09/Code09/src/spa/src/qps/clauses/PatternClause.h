#pragma once

#include "QueryClause.h"
#include "qps/ParserUtils.h"
#include "qps/clauseArguments/ExpressionSpec.h"
#include "qps/clauseArguments/Synonym.h"
#include "qps/clauses/patterns/AssignPattern.h"
#include "qps/clauses/patterns/IfPattern.h"
#include "qps/clauses/patterns/Pattern.h"
#include "qps/clauses/patterns/WhilePattern.h"

class PatternClause : public QueryClause {
protected:
    ClauseArgument& synonym;
    std::vector<ClauseArgument*> args;

public:
    PatternClause(ClauseArgument*, std::vector<ClauseArgument*>);
    bool equals(const QueryClause&) const override;
    ClauseResult evaluate(PKBFacadeReader&) override;
    bool isBooleanResult() const override;
    bool containsSynonym(const Synonym&) const override;
    std::vector<Synonym> getSynonyms() const override;
    bool validateArguments(SynonymStore* store) override;
};
