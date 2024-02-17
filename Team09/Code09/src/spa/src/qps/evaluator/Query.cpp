#include "Query.h"

Query::Query(const std::vector<QueryEntity>& se,
             const std::vector<SuchThatClause>& stc,
             const std::vector<PatternClause>& pc)
    : selectEntities(se), suchThatClauses(stc), patternClauses(pc) {}

std::vector<std::string> Query::evaluate(const PKBFacadeReader& pkb) {
    // TODO(Ezekiel): Run evaluate on clauses
    return buildSelectTable(pkb).extractResults(selectEntities);
}

std::vector<QueryEntity> Query::getSelectEntities() const {
    return selectEntities;
}

Table Query::buildSelectTable(const PKBFacadeReader& pkb) {
    std::vector<QueryEntity> header{selectEntities};
    std::vector<Row> rows{};

    for (QueryEntity entity : selectEntities) {
        Row row{};

        switch (entity.getType()) {
            case EntityType::VARIABLE:
                for (std::string var : pkb.getVariables()) {
                    row.push_back(var);
                }
                break;
            case EntityType::CONSTANT:
                for (std::string con : pkb.getConstants()) {
                    row.push_back(con);
                }
                break;
            case EntityType::PROCEDURE:
                for (std::string prod : pkb.getProcedures()) {
                    row.push_back(prod);
                }
                break;
        }
        rows.push_back(row);
    }

    return Table{header, rows};
}
