#include "PQLParser.h"

// Checked with lecturer, this is acceptable format for PQL:
// Select v1 such that Parent(v1,v2) pattern a(v1,v2)
// can >=0 spaces at existing spaces, and between commas and in front of brackets

Query PQLParser::parse(UnparsedQueries unparsedQueries) {
    if (unparsedQueries.empty()) {
        throw QPSSyntaxError();
    }

    std::vector<std::string> unparsedEntities = {};
    std::string unparsedClauses;

    unparsedClauses = PQLParser::getQueryClauses(unparsedQueries);
    unparsedQueries.pop_back();  // Remove Select statement in unparsedQueries, left with the declarations
    for (std::string queryStatement : unparsedQueries) {
        if (isDeclarationStatement(queryStatement)) {
            unparsedEntities.push_back(queryStatement);
        } else {
            throw QPSSyntaxError();
        }
    }
    // Select entities in select clause
    std::vector<Synonym> selectEntities = PQLParser::findSelectClauses(unparsedClauses);
    std::vector<SuchThatClause> suchThatClauses = PQLParser::findSuchThatClauses(unparsedClauses);
    std::vector<PatternClause> patternClauses = PQLParser::findPatternClauses(unparsedClauses);

    // ALL entities declared
    SynonymStore entities = PQLParser::parseQueryEntities(unparsedEntities);
    Validator::validateClauses(&entities, selectEntities, suchThatClauses, patternClauses);
    return Query{selectEntities, suchThatClauses, patternClauses};
}

std::string PQLParser::getQueryClauses(UnparsedQueries unparsedQuery) {
    std::string selectClause = unparsedQuery.back();
    if (!isSelectStatement(selectClause)) {
        throw QPSSyntaxError();
    }
    if (selectClause.back() == ';') {
        throw QPSSyntaxError();
    }
    return selectClause;
}

// Parse query entities from UnparsedQuery (std::vector<std::string>)
// Input "call c1, c2; assign a1; stmt s1, s2" at this point
// Output "std::vector<QueryEntity, QueryEntity, ... >"
SynonymStore PQLParser::parseQueryEntities(std::vector<std::string> unparsedEntities) {
    SynonymStore synonymStore = {};

    for (std::string synonymTypeList : unparsedEntities) {
        // synonymTypeList should look something like "call cl, c2;"
        // splitting up synonyms individually
        if (synonymTypeList.back() != ';') {
            throw QPSSyntaxError();
        }
        synonymTypeList.pop_back();  // remove semi-colon

        std::string type;
        std::string synonymsString;
        std::tie(type, synonymsString) = substringUntilDelimiter(synonymTypeList, SPACE);
        std::vector<std::string> synonyms = splitByDelimiter(synonymsString, ",");

        if (synonyms.empty()) {
            throw QPSSyntaxError();
        }

        // Determine entity type and make appropriate QueryEntity
        DesignEntityType entityType = Synonym::determineType(type);

        for (std::string synonym : synonyms) {
            if (!isSynonym(synonym)) {
                throw QPSSyntaxError();
            }
        }

        for (std::string synonym : synonyms) {
            synonymStore.storeSynonym(entityType, synonym);
        }
    }
    return synonymStore;
}

std::vector<Synonym> PQLParser::findSelectClauses(std::string unparsedClauses) {
    std::smatch match;
    std::string selectEntity;
    std::vector<Synonym> result = {};  // if there is none
    if (std::regex_search(unparsedClauses, match, QPSRegexes::SELECT_CLAUSE)) {
        selectEntity = match[1];
        if (isSynonym(selectEntity)) {
            result.push_back(Synonym(DesignEntityType::UNKNOWN, selectEntity));
        } else {
            throw QPSSyntaxError();
        }
    }

    return result;
}

std::vector<SuchThatClause> PQLParser::findSuchThatClauses(std::string unparsedClauses) {
    std::vector<SuchThatClause> result = {};  // if there is none
    for (std::string clauseString : searchClause(QPSRegexes::SUCHTHAT_CLAUSE, unparsedClauses)) {
        // replaced only the below line for strategy pattern refactor
        // SuchThatClause st = toSTClause(entities, clauseString);
        // UnparsedClause(std::vector<Synonym> entities, std::string str, std::unique_ptr<ParsingStrategy> &&strategy);
        UnparsedClause unparsedClause = UnparsedClause(clauseString, std::make_unique<SuchThatStrategy>());
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

std::vector<PatternClause> PQLParser::findPatternClauses(std::string unparsedClauses) {
    std::vector<PatternClause> result = {};  // if there is none
    for (std::string clauseString : searchClause(QPSRegexes::PATTERN_CLAUSE, unparsedClauses)) {
        // replaced only the below line for strategy pattern refactor
        // PatternClause st = toPatternClause(entities, clauseString);
        UnparsedClause unparsedClause = UnparsedClause(clauseString, std::make_unique<PatternStrategy>());
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
