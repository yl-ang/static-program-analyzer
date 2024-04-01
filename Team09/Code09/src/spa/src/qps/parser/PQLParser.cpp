#include "PQLParser.h"

#include <iostream>
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

    // Returns such that, pattern, and
    std::vector<std::string> clauseList = getAllClauses(unparsedClauses);

    // ALL entities declared
    SynonymStore entities = PQLParser::parseQueryEntities(unparsedEntities);

    // Replace 'and' clause with 'such that', 'pattern', etc. before processing
    modifyClauseList(clauseList);

    auto result = parseClauses(clauseList);
    std::shared_ptr<SelectEntContainer> selectEntities = std::get<0>(result);
    std::vector<SuchThatClause> suchThatClauses = std::get<1>(result);
    std::vector<PatternClause> patternClauses = std::get<2>(result);
    std::vector<WithClause> withClauses = std::get<3>(result);

    Validator::validateClauses(&entities, selectEntities, suchThatClauses, patternClauses, withClauses);
    return Query{selectEntities->getSynonyms(), suchThatClauses, patternClauses, withClauses};
}

std::tuple<std::shared_ptr<SelectEntContainer>, std::vector<SuchThatClause>, std::vector<PatternClause>,
           std::vector<WithClause>>
PQLParser::parseClauses(const std::vector<std::string>& clauseList) {
    std::shared_ptr<SelectEntContainer> selectEntities;
    std::vector<SuchThatClause> suchThatClauses;
    std::vector<PatternClause> patternClauses;
    std::vector<WithClause> withClauses;

    for (std::string clauseString : clauseList) {
        if (std::regex_match(clauseString, QPSRegexes::SELECT_CLAUSE)) {
            selectEntities = PQLParser::parseSelectClause(clauseString);
        } else if (std::regex_match(clauseString, QPSRegexes::SUCHTHAT_CLAUSE)) {
            suchThatClauses.push_back(PQLParser::parseSuchThatClauses(clauseString));
        } else if (std::regex_match(clauseString, QPSRegexes::PATTERN_CLAUSE)) {
            patternClauses.push_back(PQLParser::parsePatternClauses(clauseString));
        } else if (std::regex_match(clauseString, QPSRegexes::WITH_CLAUSE)) {
            withClauses.push_back(PQLParser::parseWithClauses(clauseString));
        } else {
            throw QPSSyntaxError();
        }
    }
    return std::make_tuple(selectEntities, suchThatClauses, patternClauses, withClauses);
}

void PQLParser::modifyClauseList(std::vector<std::string>& clauseList) {
    std::string currClauseType;
    int selectCounter = 0;
    for (size_t i = 0; i < clauseList.size(); i++) {
        if (std::regex_match(clauseList[i], QPSRegexes::SELECT_CLAUSE)) {
            selectCounter++;
        } else if (std::regex_match(clauseList[i], QPSRegexes::SUCHTHAT_CLAUSE)) {
            currClauseType = QPSConstants::SUCH_THAT;
        } else if (std::regex_match(clauseList[i], QPSRegexes::PATTERN_CLAUSE)) {
            currClauseType = QPSConstants::PATTERN;
        } else if (std::regex_match(clauseList[i], QPSRegexes::AND_CLAUSE)) {
            clauseList[i].replace(0, 3, currClauseType);
        } else {
            throw QPSSyntaxError();
        }
    }
    if (selectCounter != 1) {
        throw QPSSyntaxError();
    }
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
        std::tie(type, synonymsString) = substringUntilDelimiter(synonymTypeList, QPSConstants::SPACE);
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

std::shared_ptr<SelectEntContainer> PQLParser::parseSelectClause(std::string clauseString) {
    std::smatch match;
    if (std::regex_search(clauseString, match, QPSRegexes::SELECT_CLAUSE)) {
        std::string selectEntity = match[1];

        // if BOOLEAN, since we have no idea if it is a variable or not now, will
        // not perform any case on it. This will be done in semantic checking.
        // Capture it like a synonym, for now.

        // if single return, will return a vector with a single ClauseArgument.
        if (isElem(selectEntity)) {
            std::shared_ptr<Elem> container = std::make_shared<Elem>();
            container->add(selectEntity);
            return container;
        }
        // if multiple return, will return a vector with multiple ClauseArgument.
        if (isTuple(selectEntity)) {
            std::shared_ptr<Tuple> container = std::make_shared<Tuple>();
            std::string elems = selectEntity.substr(1, selectEntity.size() - 2);
            std::vector<std::string> splitResult = splitByDelimiter(elems, ",");
            // Push back the split results into a vector
            for (const auto& str : splitResult) {
                container->add(str);
            }
            return container;
        }
    }
    throw QPSSyntaxError();
}

SuchThatClause PQLParser::parseSuchThatClauses(std::string clauseString) {
    std::vector<SuchThatClause> result = {};  // if there is none

    // replaced only the below line for strategy pattern refactor
    // SuchThatClause st = toSTClause(entities, clauseString);
    // UnparsedClause(std::vector<Synonym> entities,
    // std::string str, std::unique_ptr<ParsingStrategy> &&strategy);
    UnparsedClause unparsedClause = UnparsedClause(clauseString, std::make_unique<SuchThatStrategy>());
    std::unique_ptr<QueryClause> qc = unparsedClause.execute();
    SuchThatClause* stPtr = dynamic_cast<SuchThatClause*>(qc.get());
    if (stPtr) {
        SuchThatClause stClause = std::move(*stPtr);
        return stClause;
    } else {
        throw Exception("Issues with Strategy Pattern for SuchThatClauses");
    }
}

PatternClause PQLParser::parsePatternClauses(std::string clauseString) {
    std::vector<PatternClause> result = {};  // if there is none
    // replaced only the below line for strategy pattern refactor
    // PatternClause st = toPatternClause(entities, clauseString);
    UnparsedClause unparsedClause = UnparsedClause(clauseString, std::make_unique<PatternStrategy>());
    std::unique_ptr<QueryClause> qc = unparsedClause.execute();
    PatternClause* ptPtr = dynamic_cast<PatternClause*>(qc.get());
    if (ptPtr) {
        PatternClause ptClause = std::move(*ptPtr);
        return ptClause;
    } else {
        throw Exception("Issues with Strategy Pattern for PatternClauses");
    }
}

WithClause PQLParser::parseWithClauses(std::string clauseString) {
    std::vector<WithClause> result = {};
    UnparsedClause unparsedClause = UnparsedClause(clauseString, std::make_unique<WithStrategy>());
    std::unique_ptr<QueryClause> qc = unparsedClause.execute();
    WithClause* ptPtr = dynamic_cast<WithClause*>(qc.get());
    if (ptPtr) {
        WithClause ptClause = std::move(*ptPtr);
        return ptClause;
    } else {
        throw Exception("Issues with Strategy Pattern for PatternClauses");
    }
}
