#include "Query.h"

Query::Query(const std::vector<Synonym>& se, const std::vector<SuchThatClause>& stc,
             const std::vector<PatternClause>& pc)
    : selectEntities(se), suchThatClauses(stc), patternClauses(pc) {}

std::vector<std::string> Query::evaluate(const PKBFacadeReader& pkb) {
    // TODO(Ezekiel): Run evaluate on clauses
    return buildSelectTable(pkb).extractResults(selectEntities);
}

std::vector<Synonym> Query::getSelectEntities() const {
    return selectEntities;
}

Table Query::buildSelectTable(const PKBFacadeReader& pkb) {
    std::vector<Synonym> header{selectEntities};
    std::vector<Row> rows{};

    for (Synonym entity : selectEntities) {
        Row row{};

        switch (entity.getType()) {
        case DesignEntityType::VARIABLE:
            for (std::string var : pkb.getVariables()) {
                row.push_back(var);
            }
            break;
        case DesignEntityType::CONSTANT:
            for (std::string con : pkb.getConstants()) {
                row.push_back(con);
            }
            break;
        case DesignEntityType::PROCEDURE:
            for (std::string prod : pkb.getProcedures()) {
                row.push_back(prod);
            }
            break;
        }
        rows.push_back(row);
    }

    return Table{header, rows};
}
