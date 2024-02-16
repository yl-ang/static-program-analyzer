#pragma once

#include <unordered_set>

#include "PKB/PKBClient/PKBFacadeReader.h"
#include "Table.h"
#include "qps/clauses/QueryClause.h"
#include "qps/parser/QueryEntity.h"

class Query {
public:
    Query(const std::vector<QueryEntity>&);  // TODO: Add suchthatclause and
                                             // patternclause into arguments
    std::vector<std::string> evaluate(const PKBFacadeReader&);

    std::vector<QueryEntity> getSelectEntities() const;

private:
    std::vector<QueryEntity> selectEntities;

    Table buildSelectTable(const PKBFacadeReader&);
};
