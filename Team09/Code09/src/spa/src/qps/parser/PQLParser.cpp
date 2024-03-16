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
    // Select entities in select clause (without checking declaration)
    std::vector<Synonym> selectEntities = PQLParser::findSelectClauses(unparsedClauses);

    // Returns such that, pattern, and
    std::vector<std::string> clauseList = getAllClauses(unparsedClauses);
    
    // Replace 'and' clause with 'such that', 'pattern', etc. before processing
    modifyClauseList(clauseList);

    std::vector<SuchThatClause> suchThatClauses = PQLParser::parseSuchThatClauses(clauseList);
    std::vector<PatternClause> patternClauses = PQLParser::parsePatternClauses(clauseList);

    // ALL entities declared
    SynonymStore entities = PQLParser::parseQueryEntities(unparsedEntities);
    
    // Semantic Checking
    validateClauses(selectEntities, suchThatClauses, patternClauses, entities);
    return Query{selectEntities, suchThatClauses, patternClauses};
}

void PQLParser::modifyClauseList(std::vector<std::string>& clauseList) {
    std::string currClauseType;
    for (size_t i = 0; i < clauseList.size(); ++i) {
        if (std::regex_match(clauseList[i], QPSRegexes::SUCHTHAT_CLAUSE)) {
            currClauseType = QPSConstants::SUCH_THAT;
        } else if (std::regex_match(clauseList[i], QPSRegexes::PATTERN_CLAUSE)) {
            currClauseType = QPSConstants::PATTERN;
        } else if (std::regex_match(clauseList[i], QPSRegexes::AND_CLAUSE)) {
            clauseList[i].replace(0, 2, currClauseType);
        } else {
            throw QPSSyntaxError();
        }
    }
}

void PQLParser::validateClauses(std::vector<Synonym>& selectEntities, std::vector<SuchThatClause>& suchThatClauses,
                    std::vector<PatternClause>& patternClauses, SynonymStore& entities) {
    bool hasSemanticError = false;

    if (selectEntities.size() == 1 && isBoolean(selectEntities[0].getValue())) {
        if (!selectEntities[0].updateType(&entities)) {
            selectEntities.erase(selectEntities.begin());
        }
        // Update 'BOOLEAN' if it is Synonym, remove if cannot be found
        // Evaluator will know if BOOLEAN if vector is empty
        // Do not trigger a warning should it not be a Synonym type
    } else {
        for (Synonym& syn : selectEntities) {
            hasSemanticError = hasSemanticError || !syn.updateType(&entities);
        }
    }

    for (SuchThatClause& clause : suchThatClauses) {
        hasSemanticError = hasSemanticError || clause.validateArguments(&entities);
    }

    for (PatternClause& clause : patternClauses) {
        hasSemanticError = hasSemanticError || clause.validateArguments(&entities);
    }

    if (hasSemanticError) {
        throw QPSSemanticError();
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

        // Semantic checks begins here
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

        // if BOOLEAN, since we have no idea if it is a variable or not now, will
        // not perform any case on it. This will be done in semantic checking.
        // Capture it like a synonym, for now.

        // if single return, will return a vector with a single ClauseArgument.
        if (isElem(selectEntity)) {
            return {Synonym(DesignEntityType::UNKNOWN, selectEntity)};
        }

        // if multiple return, will return a vector with multiple ClauseArgument.
        else if (isTuple(selectEntity)) {
            
            std::smatch matches;
            if (std::regex_match(selectEntity, matches, QPSRegexes::TUPLE)) {
                std::string elems = matches[1];
                std::vector<std::string> splitResult = splitByDelimiter(elems, ",");
                // Push back the split results into a vector
                for (const auto& str : splitResult) {
                    result.push_back(Synonym(DesignEntityType::UNKNOWN, str));
                }
            } else {
                throw QPSSyntaxError();
            }
        } else {
            throw QPSSyntaxError();
        }
    }
    return result;
}

std::vector<SuchThatClause> PQLParser::parseSuchThatClauses(std::vector<std::string> clauseList) {
    std::vector<SuchThatClause> result = {};  // if there is none
    for (std::string clauseString : clauseList) {
        if (std::regex_match(clauseString, QPSRegexes::SUCHTHAT_CLAUSE)) {
            // replaced only the below line for strategy pattern refactor
            // SuchThatClause st = toSTClause(entities, clauseString);
            // UnparsedClause(std::vector<Synonym> entities,
            // std::string str, std::unique_ptr<ParsingStrategy> &&strategy);
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
    }
    return result;
}

std::vector<PatternClause> PQLParser::parsePatternClauses(std::vector<std::string> clauseList) {
    std::vector<PatternClause> result = {};  // if there is none
    for (std::string clauseString : clauseList) {
        if (std::regex_match(clauseString, QPSRegexes::PATTERN_CLAUSE)) {
            // replaced only the below line for strategy pattern refactor
            // PatternClause st = toPatternClause(entities, clauseString);
            UnparsedClause unparsedClause = UnparsedClause(clauseString, std::make_unique<PatternStrategy>());
            std::unique_ptr<QueryClause> qc = unparsedClause.execute();
            PatternClause* ptPtr = dynamic_cast<PatternClause*>(qc.get());
            if (ptPtr) {
                PatternClause ptClause = std::move(*ptPtr);
                result.push_back(ptClause);
            } else {
                throw Exception("Issues with Strategy Pattern for PatternClauses");
            }
        }
    }

    return result;
}
