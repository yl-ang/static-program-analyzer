#include "PQLParser.h"

Query PQLParser::parse(UnparsedQuery unparsedQuery) {
    std::vector<QueryEntity> entities = parseQueryEntities(unparsedQuery);
    std::vector<Clause> clauses = parseQueryClauses(unparsedQuery);
    Query query = combineResult(entities, clauses);
    return query;
}

std::vector<QueryEntity> PQLParser::parseQueryEntities(UnparsedQuery) {
    // Parse query entities from UnparsedQuery (std::vector<std::string>)
    return {};
}

std::vector<Clause> PQLParser::parseQueryClauses(UnparsedQuery) {
    // Parse clauses from UnparsedQuery (std::vector<std::string>)
    return {};
}

Query PQLParser::combineResult(
    std::vector<QueryEntity> entities, std::vector<Clause> clauses) {
    // Just combines the two into a vector
    return {};
}
