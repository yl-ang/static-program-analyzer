#include "PQLParser.h"
#include "QueryEntity.h"
#include "../GrammarUtils.h"
#include "../ParserUtils.h"
#include "../exceptions/Exception.h"
#include <iostream>

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
    // ALL entities declared
    std::vector<QueryEntity> entities = PQLParser::parseQueryEntities(unparsedEntities);
    // Select entities in select clause
    std::vector<QueryEntity> selectEntities = PQLParser::findSelectClauses(entities, unparsedClauses);
    std::vector<SuchThatClause> suchThatClauses = PQLParser::findSuchThatClauses(entities, unparsedClauses);
    std::vector<PatternClause> patternClauses = PQLParser::findPatternClauses(entities, unparsedClauses);
    return Query{selectEntities, suchThatClauses, patternClauses};
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

std::vector<QueryEntity> PQLParser::findSelectClauses(std::vector<QueryEntity> entities, std::string unparsedClauses) {
    std::regex pattern("Select\\s+(\\w+)"); 
    // Select{>=1 whitespaces}{capturing group}

    std::smatch match;
    std::string selectEntity;
    std::vector<QueryEntity> result = {}; // if there is none

    if (std::regex_search(unparsedClauses, match, pattern)) {
        selectEntity = match[1];
        for (const QueryEntity& entity : entities) {
            if (entity.getName() == selectEntity) {
                result.push_back(entity);
            }
        }
    }

    return result;
}

std::vector<SuchThatClause> PQLParser::findSuchThatClauses(std::vector<QueryEntity> entities, std::string unparsedClauses) {
    // ai-gen start(chatgpt, 2, e)
    // prompt: 
    std::vector<SuchThatClause> result = {}; // if there is none
    std::regex stPattern("\\s+such\\s+that\\s+(\\w+\\*?\\s*\\(.*?\\))");
    // {>=1 whitespaces}such{>=1 whitespaces}that{>=1 whitespaces}{capturing group} 
    // capturing group format -> {letters/digits}{optional *}{>=0 whitespaces}{bracketed non-greedy}
    std::smatch match;
    
    std::string::const_iterator searchStart(unparsedClauses.cbegin());
    
    while (std::regex_search(searchStart, unparsedClauses.cend(), match, stPattern)) {
        SuchThatClause st = toSTClause(entities, match.str(1));
        result.push_back(st);
        searchStart = match.suffix().first;
    }
    
    return result;
    // ai-gen end 
}

std::vector<PatternClause> PQLParser::findPatternClauses(std::vector<QueryEntity> entities, std::string unparsedClauses) {
    // ai-gen start(chatgpt, 2, e)
    // prompt: 
    std::vector<PatternClause> result = {}; // if there is none
    std::regex pattern("\\s+pattern\\s+(\\w+\\s*\\(.*?\\))");
    // {>=1 whitespaces}pattern{>=1 whitespaces}{capturing group} 
    // capturing group format -> {letters/digits}{>=0 whitespaces}{bracketed non-greedy}
    std::smatch match;
    
    std::string::const_iterator searchStart(unparsedClauses.cbegin());
    while (std::regex_search(searchStart, unparsedClauses.cend(), match, pattern)) {
    
        PatternClause pt = toPatternClause(entities, match.str(1));
        result.push_back(pt);
        searchStart = match.suffix().first;
    }
    
    return result;
    // ai-gen end
}

// Helper function to findSuchThatClauses
SuchThatClause PQLParser::toSTClause(std::vector<QueryEntity> entities, std::string str) {
    std::regex stArguments("(\\w+\\*?)\\s*\\((.*?)\\)");
    // <{letters/digits}{optional *}>{>=0 whitespaces}<{bracketed non-greedy}>
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

    } else {
        std::cout << "Cannot convert string to SuchThatClause: " << str << "\n";
        exit(1);
    }
}

// Helper function to findPatternClauses
PatternClause PQLParser::toPatternClause(std::vector<QueryEntity> entities, std::string str) {
    std::regex ptClause("(\\w+)\\s*\\((.*?)\\)");
    // <{letters/digits}>{>=0 whitespaces}<{bracketed non-greedy}>
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

    } else {
        std::cout << "Cannot convert string to SuchThatClause: " << str << "\n";
        exit(1);
    }
}
