#pragma once

#include <unordered_set>

#include "PKB/PKBClient/PKBFacadeReader.h"
#include "Table.h"
#include "qps/clauseArguments/Synonym.h"
#include "qps/clauses/PatternClause.h"
#include "qps/clauses/SuchThatClause.h"

class Query {
public:
    Query(const std::vector<Synonym>&, const std::vector<SuchThatClause>&, const std::vector<PatternClause>&);
    std::vector<std::string> evaluate(PKBFacadeReader&);
    std::vector<Synonym> getSelectEntities() const;

private:
    std::vector<Synonym> selectEntities;
    std::vector<SuchThatClause> suchThatClauses;
    std::vector<PatternClause> patternClauses;

    Table buildSelectTable(const PKBFacadeReader&);
};
