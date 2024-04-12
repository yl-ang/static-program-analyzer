#pragma once

#include "QueryClause.h"

enum class WithType { INTEGER, NAME };

static std::unordered_map<SynonymAttributeType, WithType> SYNONYM_ATTRIBUTE_TYPE_TO_WITH_TYPE_MAP = {
    {SynonymAttributeType::PROCNAME, WithType::NAME},
    {SynonymAttributeType::VARNAME, WithType::NAME},
    {SynonymAttributeType::VALUE, WithType::INTEGER},
    {SynonymAttributeType::STMTNUM, WithType::INTEGER},
    {SynonymAttributeType::NONE, WithType::INTEGER}};

class WithClause : public QueryClause {
private:
    std::shared_ptr<ClauseArgument> lhs;
    std::shared_ptr<ClauseArgument> rhs;

    WithType determineWithType(std::shared_ptr<ClauseArgument>);

    ClauseResult evaluateValueEquality() const;
    ClauseResult evaluateOneSynonym(PKBFacadeReader& pkb, EvaluationDb&) const;
    ClauseResult evaluateBothSynonyms(PKBFacadeReader& pkb, EvaluationDb&) const;

public:
    WithClause(std::shared_ptr<ClauseArgument>, std::shared_ptr<ClauseArgument>);
    bool equals(const QueryClause& other) const override;
    ClauseResult evaluate(PKBFacadeReader&, EvaluationDb&) override;
    bool isBooleanResult() const override;
    bool containsSynonym(const Synonym&) const override;
    std::vector<Synonym> getSynonyms() const override;
    bool validateArguments(SynonymStore* store) override;
};
