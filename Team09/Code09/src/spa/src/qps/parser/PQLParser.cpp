#include "../ParserUtils.h"
#include "PQLParser.h"

ParsedQuery PQLParser::parse(UnparsedQuery unparsedQuery) {
    std::string unparsedDeclarations = getQueryDeclarations(unparsedQuery);
    std::string unparsedClauses = getQueryClauses(unparsedQuery);
    std::vector<QueryDeclaration> declarations = parseQueryDeclarations(unparsedDeclarations);
    std::vector<QueryClause> clauses = parseQueryClauses(unparsedClauses);
    ParsedQuery query = combineResult(declarations, clauses);
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
std::vector<QueryDeclaration> PQLParser::parseQueryDeclarations(std::string unparsedDeclarations) {
    std::vector<QueryDeclaration> queryDeclarations = {};

    // Split up synonyms by types
    std::vector<std::string> synonymTypesList = splitByDelimiter(unparsedDeclarations, ";");
    

    for (std::string synonymTypeList : synonymTypesList) {
        // synonymTypeList should look something like "call cl, c2"

        // splitting up synonyms individually after splitting by type
        std::vector<std::string> typeAndSynonyms = splitByDelimiter(synonymTypeList, ", ");
        std::vector<std::string> typeAndFirstSynonym = splitByDelimiter(typeAndSynonyms[0], " ");

        // first synonym and type
        std::string type = typeAndFirstSynonym[0];
        QueryEntity firstArg = typeAndFirstSynonym[1];
        QueryDeclaration firstQueryDeclaration = {type, firstArg};
        queryDeclarations.push_back(firstQueryDeclaration);

        // skip first element for other synonyms
        std::vector<std::string> sublist(typeAndSynonyms.begin() + 1, typeAndSynonyms.end());

        for (std::string synonym : sublist) {
            QueryEntity currArg = synonym;
            QueryDeclaration currQueryDeclaration = {type, currArg};
            queryDeclarations.push_back(currQueryDeclaration);
        }
    }
    return queryDeclarations;
}

std::vector<QueryClause> PQLParser::parseQueryClauses(std::string unparsedClauses) {
    // Parse clauses from UnparsedQuery (std::vector<std::string>)
    // Input should look something like ""
    // Output should look something like ""
    return {};
}

ParsedQuery PQLParser::combineResult(
    std::vector<QueryDeclaration> queryDeclarations, std::vector<QueryClause> queryClauses) {
    // Just combines the two
    // into a vector {variables, clauses}
    return {};
}
