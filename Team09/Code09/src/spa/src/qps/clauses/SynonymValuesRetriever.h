#pragma once

#include "ClauseEvaluatorUtils.h"
#include "qps/clauseArguments/Synonym.h"
#include "qps/clauses/ClauseResult.h"
#include "qps/exceptions/clauses/QPSUnknownDETypeError.h"

using RowValues = std::vector<std::string>;

class SynonymValuesRetriever {
public:
    static RowValues retrieve(PKBFacadeReader& pkb, Synonym synonym, const std::shared_ptr<EvaluationDb>&);
    static std::vector<RowValues> retrieve(PKBFacadeReader& pkb, std::vector<Synonym> synonyms,
                                           const std::shared_ptr<EvaluationDb>&);
    static ClauseResult retrieveAsClauseResult(PKBFacadeReader& pkb, std::vector<Synonym> synonyms,
                                               const std::shared_ptr<EvaluationDb>&);

private:
    static std::vector<RowValues> cartesianProduct(const std::vector<RowValues>& values);
    static std::vector<std::string> getSynonymValues(PKBFacadeReader& pkb, Synonym synonym,
                                                     const std::shared_ptr<EvaluationDb>&);
};
