#pragma once

#include "QueryClause.h"
#include "qps/ParserUtils.h"
#include "qps/clauseArguments/ExpressionSpec.h"
#include "qps/clauseArguments/Synonym.h"
#include "qps/clauses/patterns/Pattern.h"
#include "qps/clauses/patterns/PatternBuilder.h"

class PatternClause : public QueryClause {
protected:
    std::shared_ptr<ClauseArgument> synonym;
    std::vector<std::shared_ptr<ClauseArgument>> arguments;

    std::optional<std::shared_ptr<Pattern>> pattern;
    ClauseResult evaluate(PKBFacadeReader&) override;

public:
    PatternClause(std::shared_ptr<ClauseArgument>, std::vector<std::shared_ptr<ClauseArgument>>);
    bool equals(const QueryClause&) const override;
    bool isBooleanResult() const override;
    bool containsSynonym(const Synonym&) const override;
    std::vector<Synonym> getSynonyms() const override;
    bool validateArguments(SynonymStore* store) override;
    std::shared_ptr<Pattern> getPattern();
};
