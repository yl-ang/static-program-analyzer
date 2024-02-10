#include "../ParserUtils.h"
#include "QueryEntity.h"
#include "PQLParser.h"

ParsedQuery PQLParser::parse(UnparsedQuery unparsedQuery) {
    std::string unparsedEntities = getQueryEntities(unparsedQuery);
    std::string unparsedClauses = getQueryClauses(unparsedQuery);
    std::vector<QueryEntity> entities = parseQueryEntities(unparsedEntities);
    std::vector<QueryClause> clauses = parseQueryClauses(unparsedClauses);
    ParsedQuery query = combineResult(entities, clauses);
    return query;
}

std::string getQueryDeclarations(UnparsedQuery unparsedQuery) {
    return unparsedQuery[0];
}

std::string getQueryClauses(UnparsedQuery unparsedQuery) {
    return unparsedQuery[1];
}

// Parse query entities from UnparsedQuery (std::vector<std::string>)
// Input should look something like "call cl, c2; assign a1; stmt s1, s2" at this point
// Output should look something like ""
std::vector<QueryEntity> PQLParser::parseQueryEntities(std::string unparsedEntities) {
    std::vector<QueryEntity> queryEntities = {};

    // Split up synonyms by types
    std::vector<std::string> synonymTypesList = splitByDelimiter(unparsedEntities, ";");
    

    for (std::string synonymTypeList : synonymTypesList) {
        // synonymTypeList should look something like "call cl, c2"

        // splitting up synonyms individually after splitting by type
        std::vector<std::string> typeAndSynonyms = splitByDelimiter(synonymTypeList, ", ");
        std::vector<std::string> typeAndFirstSynonym = splitByDelimiter(typeAndSynonyms[0], " ");

        // first synonym and type
        std::string type = typeAndFirstSynonym[0];
        std::string firstArg = typeAndFirstSynonym[1];

        // Determine entity type and make appropriate QueryEntity --> STOPPED HERE
        EntityType entityType = QueryEntity::determineType(type);
        QueryEntity firstQueryDeclaration = QueryEntity::QueryEntity(entityType, firstArg);
        queryEntities.push_back(firstQueryDeclaration);

        // skip first element for other synonyms
        std::vector<std::string> sublist(typeAndSynonyms.begin() + 1, typeAndSynonyms.end());

        for (std::string synonym : sublist) {
            QueryEntity currQueryDeclaration = QueryEntity::QueryEntity(entityType, synonym);
            queryEntities.push_back(currQueryDeclaration);
        }
    }
    return queryEntities;
}

// Parse clauses from UnparsedQuery (std::vector<std::string>)
// Input should look something like "Select ... such that ... pattern ..."
// Output should look something like ""
std::vector<QueryClause> PQLParser::parseQueryClauses(std::string unparsedClauses) {
    
    return {};
}

// Just combines the two
// into a unordered_map[variables] = clauses
ParsedQuery PQLParser::combineResult(
    std::vector<QueryEntity> queryEntity, std::vector<QueryClause> queryClauses) {
    
    ParsedQuery parsedQuery;
    parsedQuery[queryEntity] = queryClauses; 
    return parsedQuery;
}
