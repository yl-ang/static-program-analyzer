#include "QueryEntity.h"
#include "QueryClause.h"
#include "PQLParser.h"
#include "../ParserUtils.h"


ParsedQuery PQLParser::parse(UnparsedQuery unparsedQuery) {
    std::string unparsedEntities = PQLParser::getQueryEntities(unparsedQuery);
    std::string unparsedClauses = PQLParser::getQueryClauses(unparsedQuery);
    std::vector<QueryEntity> entities = PQLParser::parseQueryEntities(unparsedEntities);
    std::vector<QueryClause*> clauses = PQLParser::parseQueryClauses(unparsedClauses);
    ParsedQuery query = PQLParser::combineResult(entities, clauses);
    return query;
}

std::string PQLParser::getQueryEntities(UnparsedQuery unparsedQuery) {
    return unparsedQuery[0];
}

std::string PQLParser::getQueryClauses(UnparsedQuery unparsedQuery) {
    return unparsedQuery[1];
}

// Parse query entities from UnparsedQuery (std::vector<std::string>)
// Input "call c1, c2; assign a1; stmt s1, s2" at this point
// Output "std::vector<QueryEntity, QueryEntity, ... >"
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
        // Determine entity type and make appropriate QueryEntity
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


// splits the select, such that, and pattern clauses,
// ASSUMPTION: only have at most one of each type of clause - OUTDATED! ***
// Only works for select now

// Need to implement findPattern(wordList)
// Need to implement findSuchThat(wordList)

// std::unordered_map<ClauseType, std::vector<int>> PQLParser::getClauseStarts(std::vector<std::string> &wordList) {
//     // std::vector<int> suchThatStart = findSuchThat(wordList);
//     // std::vector<int> patternStart = findPattern(wordList);
//     std::vector<int> selectStart{0};

//     std::unordered_map<ClauseType, std::vector<int>> res{{ClauseType::SELECT, selectStart}};
//                                                         // {ClauseType::SUCH_THAT, suchThatStart},
//                                                         // {ClauseType::PATTERN, patternStart}};
//     return res;
// }

// Parse clauses from UnparsedQuery (std::vector<std::string>)
// Input should look something like "Select ... such that ... pattern ..."
// Output should look something like ""
std::vector<QueryClause*> PQLParser::parseQueryClauses(std::string unparsedClauses) {
    // Identify and parse SELECT, SUCH THAT, PATTERN clauses within the query string
    // Identify starting positions of SELECT, SUCH THAT, PATTERN
    std::vector<QueryClause*> parsedClauses;
    std::vector<std::string> wordList = stringToWordList(unparsedClauses);
    // std::unordered_map<ClauseType, std::vector<int>> clauseStarts = getClauseStarts(wordList);
    // there will be a function to get the end of each clause, but for now, will hardcode for 'Select v' alone
    parsedClauses.push_back(new SelectClause(wordList[1]));

    return parsedClauses;
}

// Just combines the two
// into a unordered_map[variables] = clauses
ParsedQuery PQLParser::combineResult(
    const std::vector<QueryEntity> queryEntities, const std::vector<QueryClause*> queryClauses) {
    return std::make_tuple(queryEntities, queryClauses);
}
