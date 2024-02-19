#include "PQLParser.h"

#include "../GrammarUtils.h"
#include "../ParserUtils.h"
#include "../exceptions/Exception.h"
#include "QueryEntity.h"

Query PQLParser::parse(UnparsedQuery unparsedQuery) {

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
    std::vector<SuchThatClause> suchThatClauses = PQLParser::findSuchThatClauses(entities, unparsedClauses);
    std::vector<PatternClause> patternClauses = PQLParser::findPatternClauses(entities, unparsedClauses);
    return Query{entities, suchThatClauses, patternClauses};
}

// Used functions:
/**
 * isDeclarationStatement
 * isSelectStatement
 * 
 * getQueryClauses
 * parseQueryEntities
 * findSuchThatClauses
 * findPatternClauses
*/

// std::vector<std::string> PQLParser::getQueryEntities(std::vector<std::string> unparsedQuery) {
//     std::vector<std::string> out = std::vector<std::string>(unparsedQuery.begin(), unparsedQuery.end() - 1);
//     return out;
// }

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
        synonymTypeList.pop_back();  // remove semi-colon
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

std::vector<SuchThatClause> PQLParser::findSuchThatClauses(std::vector<QueryEntity> entities, std::string unparsedClauses) {
    // TODO(Parser Team): implement

    // AI-START
    std::vector<SuchThatClause> result;
    std::regex stPattern("such that\\s(\\w+\\*?\\(.*?\\))");
    std::smatch match;
    
    std::string::const_iterator searchStart(unparsedClauses.cbegin());
    // AI-END
    while (std::regex_search(searchStart, unparsedClauses.cend(), match, stPattern)) {
        SuchThatClause st = toSTClause(entities, match.str(1));
        result.push_back(st);
        searchStart = match.suffix().first;
    }
    
    return result;
}

SuchThatClause PQLParser::toSTClause(std::vector<QueryEntity> entities, std::string str) {
    std::regex stArguments("\\b(\\w+\\*?)\\((.*?)\\)");
    std::smatch argMatch;
    if (std::regex_search(str, argMatch, stArguments)) {
        std::string type = argMatch[1];
        std::string parameters = argMatch[2];

        std::istringstream iss(parameters);
        std::string parameter;

        std::vector<QueryEntity> entityVector;

        while (std::getline(iss, parameter, ',')) {
            parameter = trim(parameter);
            for (const QueryEntity& entity : entities) {
                if (entity.getName() == parameter) {
                    entityVector.push_back(entity);
                }
            }
        }
        return SuchThatClause(SuchThatClause::determineType(type), entityVector[0], entityVector[1]);
    }
}

PatternClause PQLParser::toPatternClause(std::vector<QueryEntity> entities, std::string str) {
    std::regex ptClause("\\b(\\w+)\\((.*?)\\)");
    std::smatch argMatch;
    if (std::regex_search(str, argMatch, ptClause)) {
        std::string assignSynonym = argMatch[1];
        std::string parameters = argMatch[2];

        std::istringstream iss(parameters);
        std::string parameter;

        std::vector<QueryEntity> entityVector;

        for (const QueryEntity& entity : entities) {
            if (entity.getName() == assignSynonym) {
                entityVector.push_back(entity);
            }
        }

        while (std::getline(iss, parameter, ',')) {
            parameter = trim(parameter);
            for (const QueryEntity& entity : entities) {
                if (entity.getName() == parameter) {
                    entityVector.push_back(entity);
                }
            }
        }

        return PatternClause(entityVector[0], entityVector[1], entityVector[2]);
    }
}

std::vector<PatternClause> PQLParser::findPatternClauses(std::vector<QueryEntity> entities, std::string unparsedClauses) {
    // TODO(Parser Team): implement

    // AI-START
    std::vector<PatternClause> result;
    std::regex pattern("pattern\\s(\\w+\\(.*?\\))");
    std::smatch match;
    
    std::string::const_iterator searchStart(unparsedClauses.cbegin());
    while (std::regex_search(searchStart, unparsedClauses.cend(), match, pattern)) {
    // AI-END
        PatternClause pt = toPatternClause(entities, match.str(1));
        result.push_back(pt);
        searchStart = match.suffix().first;
    }
    
    return result;
}

// splits the select, such that, and pattern clauses,
// ASSUMPTION: only have at most one of each type of clause - OUTDATED! ***
// Only works for select now

// Need to implement findPattern(wordList)
// Need to implement findSuchThat(wordList)

// std::unordered_map<ClauseType, std::vector<int>>
// PQLParser::getClauseStarts(std::vector<std::string> &wordList) {
//     // std::vector<int> suchThatStart = findSuchThat(wordList);
//     // std::vector<int> patternStart = findPattern(wordList);
//     std::vector<int> selectStart{0};

//     std::unordered_map<ClauseType, std::vector<int>> res{{ClauseType::SELECT,
//     selectStart}};
//                                                         //
//                                                         {ClauseType::SUCH_THAT,
//                                                         suchThatStart},
//                                                         //
//                                                         {ClauseType::PATTERN,
//                                                         patternStart}};
//     return res;
// }

// Parse clauses from UnparsedQuery (std::vector<std::string>)
// Input should look something like "Select ... such that ... pattern ..."
// Output should look something like ""
// std::vector<QueryClause*> PQLParser::parseQueryClauses(std::string unparsedClauses) {
//     // Identify and parse SELECT, SUCH THAT, PATTERN clauses within the query
//     // string Identify starting positions of SELECT, SUCH THAT, PATTERN
//     std::vector<QueryClause*> parsedClauses;
//     std::vector<std::string> wordList = stringToWordList(unparsedClauses);
//     // std::unordered_map<ClauseType, std::vector<int>> clauseStarts =
//     // getClauseStarts(wordList); there will be a function to get the end of
//     // each clause, but for now, will hardcode for 'Select v' alone
//     parsedClauses.push_back(new SelectClause(wordList[1]));

//     return parsedClauses;
// }

// // Just combines the two
// // into a unordered_map[variables] = clauses
// Query PQLParser::combineResult(const std::vector<QueryEntity> queryEntities,
//                                const std::vector<SuchThatClause>& stc,
//                                const std::vector<PatternClause>& pc) {
//     return Query(queryEntities, stc, pc);
//     // return std::make_tuple(queryEntities, queryClauses);
// }
