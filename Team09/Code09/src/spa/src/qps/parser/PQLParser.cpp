#include "PQLParser.h"

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
    std::smatch match;
    std::string selectEntity;
    std::vector<Synonym> result = {};  // if there is none
    if (std::regex_search(unparsedClauses, match, QPSRegexes::SELECT_CLAUSE)) {
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
    for (std::string clauseString : searchClause(QPSRegexes::SUCHTHAT_CLAUSE, unparsedClauses)) {
        // replaced only the below line for strategy pattern refactor
        // SuchThatClause st = toSTClause(entities, clauseString);
        // UnparsedClause(std::vector<Synonym> entities, std::string str, std::unique_ptr<ParsingStrategy> &&strategy);
        UnparsedClause unparsedClause = UnparsedClause(entities, clauseString, std::make_unique<SuchThatStrategy>());
        std::unique_ptr<QueryClause> qc = unparsedClause.execute();
        SuchThatClause* stPtr = dynamic_cast<SuchThatClause*>(qc.get());
        if (stPtr) {
            SuchThatClause stClause = std::move(*stPtr);
            result.push_back(stClause);
        } else {
            throw Exception("Issues with Strategy Pattern for SuchThatClauses");
        }
    }
    return result;
}

std::vector<PatternClause> PQLParser::findPatternClauses(std::vector<Synonym> entities, std::string unparsedClauses) {
    std::vector<PatternClause> result = {};  // if there is none
    for (std::string clauseString : searchClause(QPSRegexes::PATTERN_CLAUSE, unparsedClauses)) {
        // replaced only the below line for strategy pattern refactor
        // PatternClause st = toPatternClause(entities, clauseString);
        UnparsedClause unparsedClause = UnparsedClause(entities, clauseString, std::make_unique<PatternStrategy>());
        std::unique_ptr<QueryClause> qc = unparsedClause.execute();
        PatternClause* ptPtr = dynamic_cast<PatternClause*>(qc.get());
        if (ptPtr) {
            PatternClause ptClause = std::move(*ptPtr);
            result.push_back(ptClause);
        } else {
            throw Exception("Issues with Strategy Pattern for SuchThatClauses");
        }
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
