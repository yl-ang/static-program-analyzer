#include "PQLEvaluator.h"

QueryResult PQLEvaluator::evaluate(Query query) {
    std::vector<QueryEntity> entities = getQueryEntities(query);
    std::vector<Clause> clauses = getQueryClauses(query);
    QueryResult qr = projectClausesOntoEntities(entities, clauses);

    return qr;
}

std::vector<QueryEntity> PQLEvaluator::getQueryEntities(Query) {
    // Extract query entities from Query
    return {};
}

std::vector<Clause> PQLEvaluator::getQueryClauses(Query) {
    // Extract clauses from Query

    return {};
}

QueryResult PQLEvaluator::projectClausesOntoEntities(
    std::vector<QueryEntity> entities, std::vector<Clause> clauses) {
    // Evaluate each clause and project the result onto the entities
    return 0;
}
