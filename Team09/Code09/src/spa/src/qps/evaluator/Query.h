#pragma once

#include <unordered_set>

#include "PKB/PKBClient/PKBFacadeReader.h"
#include "Table.h"
#include "TableManager.h"
#include "qps/clauseArguments/Synonym.h"
#include "qps/clauses/PatternClause.h"
#include "qps/clauses/SuchThatClause.h"
#include "qps/clauses/WithClause.h"

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

    static bool evaluateAndJoinClauses(const TableManager& tm,
                                       const std::vector<std::vector<QueryClausePtr>>& connectedClausesList,
                                       PKBFacadeReader& pkb);
    static ValueTransformer projectSynonymAttributesTransformer(PKBFacadeReader& pkb);
    static Synonym headerMatcher(const std::vector<Synonym>& synonyms, Synonym newSynonym);

    ArrangedClauses arrangeClauses() const;
    bool evaluateBooleanClauses(PKBFacadeReader&) const;
    void projectAttributes(const TableManager& tm, PKBFacadeReader& pkb) const;
    void buildAndJoinSelectTable(const TableManager&, PKBFacadeReader&) const;
    std::vector<QueryClausePtr> getNonBooleanClauses() const;
    std::vector<std::vector<QueryClausePtr>> splitIntoConnectedSynonyms() const;

    // Getters and checkers
    bool hasNoClauses() const;
    bool isSelectBoolean() const;
    std::vector<std::string> getEmptyResult() const;
    bool containsSelectSynonyms(QueryClausePtr) const;
};
