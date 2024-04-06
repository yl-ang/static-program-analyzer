#pragma once

#include "QueryClause.h"

enum class WithType { INTEGER, NAME };

class WithClause : public QueryClause {
private:
    std::shared_ptr<ClauseArgument> lhs;
    std::shared_ptr<ClauseArgument> rhs;

    WithType determineWithType(std::shared_ptr<ClauseArgument>);

    ClauseResult evaluateValueEquality() const;
    ClauseResult evaluateOneSynonym(PKBFacadeReader& pkb) const;
    ClauseResult evaluateBothSynonyms(PKBFacadeReader& pkb) const;

public:
    WithClause(std::shared_ptr<ClauseArgument>, std::shared_ptr<ClauseArgument>);
    bool equals(const QueryClause& other) const override;
    ClauseResult evaluate(PKBFacadeReader&) override;
    bool isBooleanResult() const override;
    bool containsSynonym(const Synonym&) const override;
    std::vector<Synonym> getSynonyms() const override;
    bool validateArguments(SynonymStore* store) override;
};
