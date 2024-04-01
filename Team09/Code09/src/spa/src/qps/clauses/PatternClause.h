#pragma once

#include "QueryClause.h"
#include "qps/ParserUtils.h"
#include "qps/clauseArguments/ExpressionSpec.h"
#include "qps/clauseArguments/Synonym.h"
#include "qps/clauses/patterns/Pattern.h"
#include "qps/clauses/patterns/PatternBuilder.h"

class PatternClause : public QueryClause {
protected:
    ClauseArgument& synonym;
    std::vector<ClauseArgument*> args;

    std::optional<std::shared_ptr<Pattern>> pattern;

public:
    PatternClause(ClauseArgument*, std::vector<ClauseArgument*>);
    bool equals(const QueryClause&) const override;
    ClauseResult evaluate(PKBFacadeReader&) override;
    bool isBooleanResult() const override;
    bool containsSynonym(const Synonym&) const override;
    std::vector<Synonym> getSynonyms() const override;
    bool validateArguments(SynonymStore* store) override;
    std::shared_ptr<Pattern> getPattern();
};
