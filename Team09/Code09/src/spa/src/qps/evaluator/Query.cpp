#include "Query.h"

Query::Query(QueryEntity& se) : selectEntity(se) {}

std::vector<std::string> Query::evaluate(const PKBFacadeReader& pkb) {
    return buildSelectTable(pkb).extractResults(selectEntity);
}

Table Query::buildSelectTable(const PKBFacadeReader& pkb) {
    std::vector<QueryEntity> header{ selectEntity };
    Row row{};

    switch (selectEntity.getType()) {
    case EntityType::VARIABLE:
        for (std::string var : pkb.getVariables()) {
            row.push_back({ var });
        }
        break;
    case EntityType::CONSTANT:
        for (std::string con : pkb.getConstants()) {
            row.push_back({ con });
        }
        break;
    case EntityType::PROCEDURE:
        for (std::string prod : pkb.getProcedures()) {
            row.push_back({ prod });
        }
        break;
    }

    return Table{ header, { row } };
}
