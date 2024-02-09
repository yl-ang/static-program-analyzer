#include "PQLParser.h"

Query PQLParser::parse(UnparsedQuery unparsedQuery) {
    std::string unparsedEntities = getQueryEntities(unparsedQuery);
    std::string unparsedClauses = getQueryClauses(unparsedQuery);
    std::vector<QueryEntity> entities = parseQueryEntities(unparsedEntities);
    std::vector<Clause> clauses = parseQueryClauses(unparsedClauses);
    Query query = combineResult(entities, clauses);
    return query;
}

std::string getQueryEntities(UnparsedQuery unparsedQuery) {
    return unparsedQuery[0];
}

std::string getQueryClauses(UnparsedQuery unparsedQuery) {
    return unparsedQuery[1];
}

std::vector<QueryEntity> PQLParser::parseQueryEntities(std::string unparsedEntities) {
    // Parse query entities from UnparsedQuery (std::vector<std::string>)
    // Input should look something like "call cl, c2; assign a1; stmt s1, s2" at this point
    // Output should look something like ""
    return {};
}

std::vector<Clause> PQLParser::parseQueryClauses(std::string unparsedClauses) {
    // Parse clauses from UnparsedQuery (std::vector<std::string>)
    // Input should look something like ""
    // Output should look something like ""
    return {};
}

Query PQLParser::combineResult(
    std::vector<QueryEntity> entities, std::vector<Clause> clauses) {
    // Just combines the two (std::vector<QueryEntity>, std::vector<Clause>) 
    // into a unordered_map {variables, clauses} as keys
    return {};
}
