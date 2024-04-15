#pragma once

#include <pkb/PKBClient/PKBFacadeReader.h>

#include "ClauseResult.h"
#include "SynonymValuesRetriever.h"
#include "qps/evaluator/EvaluationDb.h"

class QueryClause {
private:
    bool negate = false;

protected:
    virtual ClauseResult evaluate(PKBFacadeReader&, EvaluationDb&) = 0;

public:
    virtual ~QueryClause() {}
    virtual bool equals(const QueryClause& other) const = 0;
    virtual bool isBooleanResult() const = 0;
    virtual bool containsSynonym(const Synonym&) const = 0;
    virtual std::vector<Synonym> getSynonyms() const = 0;
    virtual bool validateArguments(SynonymStore* store) = 0;

    void setNegation(bool n);
    std::string rowValuesToString(const std::vector<Synonym>& synonyms, const Row& row);
    ClauseResult runEvaluation(PKBFacadeReader& pkb, EvaluationDb& evalDb);
};
