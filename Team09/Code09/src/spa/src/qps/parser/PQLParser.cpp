#include "QueryEntity.h"
#include "QueryClause.h"
#include "PQLParser.h"
#include "../GrammarUtils.h"
#include "../ParserUtils.h"
#include "../exceptions/Exception.h"


ParsedQuery PQLParser::parse(UnparsedQuery unparsedQuery) {
    std::vector<std::string> unparsedEntities = {};
    std::string unparsedClauses;
    for (std::string queryStatement : unparsedQuery) {
        if (isDeclarationStatement(queryStatement)) {
            unparsedEntities.push_back(queryStatement);
        } else if (isSelectStatement(queryStatement)) {
            unparsedClauses = PQLParser::getQueryClauses(unparsedQuery);
        } else {
            throw Exception("Syntax Error");
        }
    }
    std::vector<QueryEntity> entities = PQLParser::parseQueryEntities(unparsedEntities);
    std::vector<QueryClause*> clauses = PQLParser::parseQueryClauses(unparsedClauses);
    Query query = PQLParser::combineResult(entities, clauses);
    return query;
}

std::vector<std::string> PQLParser::getQueryEntities(std::vector<std::string> unparsedQuery) {
    std::vector<std::string> out = std::vector<std::string>(unparsedQuery.begin(), unparsedQuery.end() - 1);
    return out;
}

std::string PQLParser::getQueryClauses(UnparsedQuery unparsedQuery) {
    return unparsedQuery[unparsedQuery.size() - 1];
}

// Parse query entities from UnparsedQuery (std::vector<std::string>)
// Input "call c1, c2; assign a1; stmt s1, s2" at this point
// Output "std::vector<QueryEntity, QueryEntity, ... >"
std::vector<QueryEntity> PQLParser::parseQueryEntities(std::vector<std::string> unparsedEntities) {
    std::vector<QueryEntity> queryEntities = {};
    for (std::string synonymTypeList : unparsedEntities) {
        // synonymTypeList should look something like "call cl, c2;"
        // splitting up synonyms individually
        if (!isValidDeclarationStatement(synonymTypeList)) {
            throw Exception("Syntax Error: Invalid declaration statement!");
        }
        synonymTypeList.pop_back();
        std::vector<std::string> typeAndSynonyms = splitByDelimiter(synonymTypeList, ",");
        std::vector<std::string> typeAndFirstSynonym = splitByDelimiter(typeAndSynonyms[0], " ");
        // first synonym and type
        std::string type = typeAndFirstSynonym[0];
        std::string firstArg = trim(typeAndFirstSynonym[1]);
        // Determine entity type and make appropriate QueryEntity
        EntityType entityType = QueryEntity::determineType(type);
        QueryEntity firstQueryDeclaration = QueryEntity(entityType, firstArg);
        queryEntities.push_back(firstQueryDeclaration);
        // skip first element for other synonyms
        std::vector<std::string> sublist = std::vector(typeAndSynonyms.begin() + 1, typeAndSynonyms.end());

        for (std::string synonym : sublist) {
            QueryEntity currQueryDeclaration = QueryEntity(entityType, trim(synonym));
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
    if (!isValidSelectStatement(unparsedClauses)) {
        throw new Exception("Syntax Error: Invalid Select syntax!");
    }
    std::vector<std::string> wordList = stringToWordList(unparsedClauses);
    // std::unordered_map<ClauseType, std::vector<int>> clauseStarts = getClauseStarts(wordList);
    // there will be a function to get the end of each clause, but for now, will hardcode for 'Select v' alone
    parsedClauses.push_back(new SelectClause(wordList[1]));

    return parsedClauses;
}

// Just combines the two
// into a unordered_map[variables] = clauses
Query PQLParser::combineResult(
    const std::vector<QueryEntity> queryEntities, const std::vector<QueryClause*> queryClauses) {
    return Query{ queryEntities };
    // return std::make_tuple(queryEntities, queryClauses);
}
