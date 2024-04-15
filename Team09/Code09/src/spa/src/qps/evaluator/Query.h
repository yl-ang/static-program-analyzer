#pragma once

#include <unordered_set>

#include "EvaluationDb.h"
#include "QueryDb.h"
#include "Table.h"
#include "TableManager.h"
#include "pkb/PKBClient/PKBFacadeReader.h"
#include "qps/clauseArguments/Synonym.h"
#include "qps/clauses/PatternClause.h"
#include "qps/clauses/SuchThatClause.h"
#include "qps/clauses/WithClause.h"
#include "qps/exceptions/evaluator/QPSNonSynonymAccessError.h"

using QueryClausePtr = std::shared_ptr<QueryClause>;

struct ArrangedClauses {
    std::vector<std::vector<QueryClausePtr>> selectConnectedClauses;
    std::vector<std::vector<QueryClausePtr>> nonSelectConnectedClauses;
};

class Query {
public:
    Query(const std::vector<Synonym>&, const std::vector<std::shared_ptr<SuchThatClause>>&,
          const std::vector<std::shared_ptr<PatternClause>>&, const std::vector<std::shared_ptr<WithClause>>&);
    std::vector<std::string> evaluate(PKBFacadeReader&) const;

private:
    std::vector<Synonym> selectEntities;
    std::vector<std::shared_ptr<SuchThatClause>> suchThatClauses;
    std::vector<std::shared_ptr<PatternClause>> patternClauses;
    std::vector<std::shared_ptr<WithClause>> withClauses;

    void evaluateAndJoinClauses(TableManager& tm, QueryDb&, PKBFacadeReader& pkb, EvaluationDb&) const;
    static ValueTransformer projectSynonymAttributesTransformer(PKBFacadeReader& pkb);
    static Synonym headerMatcher(const std::vector<Synonym>& synonyms, Synonym newSynonym);

    bool evaluateBooleanClauses(PKBFacadeReader&, EvaluationDb& evalDb) const;
    void projectAttributes(const TableManager& tm, PKBFacadeReader& pkb) const;
    void buildAndJoinSelectTable(const TableManager&, PKBFacadeReader&, EvaluationDb&) const;
    std::vector<QueryClausePtr> getNonBooleanClauses() const;

    // Getters and checkers
    bool hasNoClauses() const;
    bool isSelectBoolean() const;
    std::vector<std::string> getEmptyResult() const;
    std::unordered_set<SynonymValue> getSynonymsResultsToRetain(QueryDb& queryDb) const;
};
