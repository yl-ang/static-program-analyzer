#pragma once

#include <unordered_set>

#include "PKB/PKBClient/PKBFacadeReader.h"
#include "Table.h"
#include "qps/clauses/QueryClause.h"
#include "qps/parser/QueryEntity.h"

class Query {
public:
    Query(const std::vector<QueryEntity>&, const std::vector<QueryClause>&);
    std::vector<std::string> evaluate(const PKBFacadeReader&);
    std::vector<QueryEntity> getSelectEntities() const;

private:
    std::vector<QueryEntity> selectEntities;
    std::vector<QueryClause> clauses;
    Table buildSelectTable(const PKBFacadeReader&);
};
