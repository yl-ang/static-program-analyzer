#pragma once

#include "qps/parser/QueryEntity.h"
#include "Table.h"
#include "PKB/PKBClient/PKBFacadeReader.h"
#include <unordered_set>


class Query {
public:
    Query(QueryEntity&); // TODO: Add suchthatclause and patternclause into arguments
    std::vector<std::string> evaluate(const PKBFacadeReader&);
private:
    QueryEntity selectEntity;

    Table buildSelectTable(const PKBFacadeReader&);
};
