#include "PQLParser.h"

#include <iostream>

#include "../GrammarUtils.h"
#include "../ParserUtils.h"
#include "../exceptions/Exception.h"

// Checked with lecturer, this is acceptable format for PQL:
// Select v1 such that Parent(v1,v2) pattern a(v1,v2)
// can >=0 spaces at existing spaces, and between commas and in front of brackets

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
    std::vector<Synonym> entities = PQLParser::parseQueryEntities(unparsedEntities);
    // Select entities in select clause
    std::vector<Synonym> selectEntities = PQLParser::findSelectClauses(entities, unparsedClauses);
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
std::vector<Synonym> PQLParser::parseQueryEntities(std::vector<std::string> unparsedEntities) {
    std::vector<Synonym> queryEntities = {};
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
        DesignEntityType entityType = Synonym::determineType(type);
        Synonym firstQueryDeclaration = Synonym(entityType, firstArg);
        queryEntities.push_back(firstQueryDeclaration);

        // skip first element for other synonyms
        std::vector<std::string> sublist = std::vector(typeAndSynonyms.begin() + 1, typeAndSynonyms.end());

        for (std::string synonym : sublist) {
            Synonym currQueryDeclaration = Synonym(entityType, trim(synonym));
            queryEntities.push_back(currQueryDeclaration);
        }
    }
    return queryEntities;
}

std::vector<Synonym> PQLParser::findSelectClauses(std::vector<Synonym> entities, std::string unparsedClauses) {
    std::regex pattern("\\s*Select\\s+(\\w+)\\s*");
    // Select{>=1 whitespaces}{capturing group}

    std::smatch match;
    std::string selectEntity;
    std::vector<Synonym> result = {};  // if there is none

    if (std::regex_search(unparsedClauses, match, pattern)) {
        selectEntity = match[1];
        for (const Synonym& entity : entities) {
            if (entity.getValue() == selectEntity) {
                result.push_back(entity);
            }
        }
    }

    return result;
}

std::vector<SuchThatClause> PQLParser::findSuchThatClauses(std::vector<Synonym> entities, std::string unparsedClauses) {
    std::vector<SuchThatClause> result = {};  // if there is none
    std::regex stPattern("\\s+such\\s+that\\s+(\\w+\\*?\\s*\\(.*?\\))\\s*");
    // {>=1 whitespaces}such{>=1 whitespaces}that{>=1 whitespaces}{capturing group}
    // capturing group format -> {letters/digits}{optional *}{>=0 whitespaces}{bracketed non-greedy}
    for (std::string clauseString : searchClause(stPattern, unparsedClauses)) {
        SuchThatClause st = toSTClause(entities, clauseString);
        result.push_back(st);
    }
    return result;
}

std::vector<PatternClause> PQLParser::findPatternClauses(std::vector<Synonym> entities, std::string unparsedClauses) {
    std::vector<PatternClause> result = {};  // if there is none
    std::regex pattern("\\s+pattern\\s+(\\w+\\s*\\(.*?\\))\\s*");

    for (std::string clauseString : searchClause(pattern, unparsedClauses)) {
        PatternClause st = toPatternClause(entities, clauseString);
        result.push_back(st);
    }

    return result;
}

/**
 * Helper function to findSuchThatClauses, findPatternClauses
 */
std::vector<std::string> PQLParser::searchClause(const std::regex& pattern, const std::string& unparsedClauses) {
    // ai-gen start(chatgpt, 2, e)
    // prompt: https://platform.openai.com/playground/p/xBykoVKvFKrMIxAn4pwwVhlY?model=gpt-4&mode=chat
    std::vector<std::string> result = {};
    std::smatch match;
    std::string::const_iterator searchStart(unparsedClauses.cbegin());
    while (std::regex_search(searchStart, unparsedClauses.cend(), match, pattern)) {
        result.push_back(match.str(1));
        searchStart = match.suffix().first;
    }
    return result;
    // ai-gen end
}

/**
 * Helper function to findSuchThatClauses
 */
SuchThatClause PQLParser::toSTClause(std::vector<Synonym> entities, std::string str) {
    std::regex stArguments("\\s*(\\w+\\*?)\\s*\\((.*?)\\)\\s*");
    // <{letters/digits}{optional *}>{>=0 whitespaces}<{bracketed non-greedy}>
    std::smatch argMatch;
    if (std::regex_search(str, argMatch, stArguments)) {
        std::string type = argMatch[1];
        std::string parameters = argMatch[2];

        std::vector<std::string> parameterStringsToParse{cleanParameters(parameters)};
        std::vector<ClauseArgument*> entityVector{buildSTParameters(entities, parameterStringsToParse)};
        return SuchThatClause(SuchThatClause::determineRelationshipType(type), entityVector[0], entityVector[1]);
    } else {
        throw Exception("Cannot convert string to SuchThatClause: " + str);
    }
}

/**
 * Helper function to findPatternClauses
 */
PatternClause PQLParser::toPatternClause(std::vector<Synonym> entities, std::string str) {
    std::regex ptClause("\\s*(\\w+)\\s*\\((.*?)\\)\\s*");
    // <{letters/digits}>{>=0 whitespaces}<{bracketed non-greedy}>
    std::smatch argMatch;
    if (std::regex_search(str, argMatch, ptClause)) {
        std::string assignSynonym = argMatch[1];
        std::string parameters = argMatch[2];

        std::vector<std::string> parameterStringsToParse{assignSynonym};
        std::vector<std::string> cleanedParameters{cleanParameters(parameters)};

        parameterStringsToParse.insert(parameterStringsToParse.end(), cleanedParameters.begin(),
                                       cleanedParameters.end());
        std::vector<ClauseArgument*> entityVector{buildPatternParameters(entities, parameterStringsToParse)};
        return PatternClause(entityVector[0], entityVector[1], entityVector[2]);
    } else {
        throw Exception("Cannot convert string to PatternClause: " + str);
    }
}

/**
 * Helper function to toSTClause, toPatternClause
 */
std::vector<std::string> PQLParser::cleanParameters(const std::string& parametersString) {
    std::vector<std::string> result{};
    std::istringstream iss(parametersString);
    std::string parameter;
    while (std::getline(iss, parameter, ',')) {
        parameter = trim(parameter);
        result.push_back(parameter);
    }
    return result;
}

/**
 * Helper function to toSTClause
 *
 * Determine if parameters are:
 * literal, wildcard, integer, synonym
 */
std::vector<ClauseArgument*> PQLParser::buildSTParameters(const std::vector<Synonym>& entities,
                                                          const std::vector<std::string>& strings) {
    std::vector<ClauseArgument*> results{};
    for (const std::string& str : strings) {
        if (isQuotedIdent(str)) {
            results.push_back(new Literal(removeAllQuotations(removeAllWhitespaces(str))));
        } else if (isWildcard(str)) {
            results.push_back(new Wildcard());
        } else if (isInteger(str)) {
            results.push_back(new Integer(str));
        } else if (isSynonym(str)) {
            results.push_back(buildSynonym(entities, str));
        } else {
            throw Exception("Issues determining if is literal, wildcard, integer or synonym: " + str);
        }
    }
    return results;
}

/**
 * Helper function to toPatternClause
 *
 * For pattern parameters:
 * first parameter is always synonym
 * second parameter is always entRef
 * third parameter is always expressionSpec
 */
std::vector<ClauseArgument*> PQLParser::buildPatternParameters(const std::vector<Synonym>& entities,
                                                               const std::vector<std::string>& strings) {
    std::vector<ClauseArgument*> results{};

    std::string ptSynonym = strings[0];
    std::string ptEntRef = strings[1];
    std::string ptExpressionSpec = strings[2];

    // first argument is synonym
    results.push_back(buildSynonym(entities, ptSynonym));

    // second argument is ent-ref
    if (isQuotedIdent(ptEntRef)) {
        results.push_back(new Literal(removeAllQuotations(removeAllWhitespaces(ptEntRef))));
    } else if (isWildcard(ptEntRef)) {
        results.push_back(new Wildcard());
    } else if (isSynonym(ptEntRef)) {
        results.push_back(buildSynonym(entities, ptEntRef));
    } else {
        throw Exception("Issues determining if Pattern EntRef is literal, wildcard, or synonym: " + ptEntRef);
    }

    // third argument is expression-spec
    //if (isExpressionSpec(ptExpressionSpec)) {
        if (isWildcard(ptExpressionSpec)) {
            results.push_back(new Wildcard());
        } else {
            results.push_back(new ExpressionSpec(removeAllQuotations(removeAllWhitespaces(ptExpressionSpec))));
        }
    //} else {
        //throw Exception("Pattern Expression-spec is not expression-spec or wildcard: " + ptEntRef);
    //}

    return results;
}

/**
 * Helper function to buildSTParameters, buildPatternParameters
 */
Synonym* PQLParser::buildSynonym(const std::vector<Synonym>& entities, const std::string& str) {
    for (const Synonym& entity : entities) {
        if (entity.getValue() == str) {
            return new Synonym(entity.getType(), entity.getValue());
            break;  // We have already matched an entity, no need to continue searching
        }
    }
    throw Exception("Could not find Synonym when parsing clauses, amongst declarations: " + str);
}
