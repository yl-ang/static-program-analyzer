#pragma once

#include <unordered_set>

#include "PKB/PKBClient/PKBFacadeReader.h"
#include "Table.h"
#include "qps/clauses/PatternClause.h"
#include "qps/clauses/SuchThatClause.h"
#include "qps/parser/QueryEntity.h"

class Query {
public:
    Query(const std::vector<QueryEntity>&, const std::vector<SuchThatClause>&,
          const std::vector<PatternClause>&);
    std::vector<std::string> evaluate(const PKBFacadeReader&);
    std::vector<QueryEntity> getSelectEntities() const;

private:
    std::vector<QueryEntity> selectEntities;
    std::vector<SuchThatClause> suchThatClauses;
    std::vector<PatternClause> patternClauses;

    Table buildSelectTable(const PKBFacadeReader&);
};
