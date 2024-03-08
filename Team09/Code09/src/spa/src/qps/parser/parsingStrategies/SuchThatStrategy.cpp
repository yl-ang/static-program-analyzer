#include "SuchThatStrategy.h"

std::unique_ptr<QueryClause> SuchThatStrategy::execute(std::vector<Synonym> entities, std::string str) const {
    std::smatch argMatch;
    if (std::regex_search(str, argMatch, QPSRegexes::SUCHTHAT_ARGS)) {
        std::string type = argMatch[1];
        std::string parameters = argMatch[2];

        std::vector<std::string> parameterStringsToParse{cleanParameters(parameters)};
        std::vector<ClauseArgument*> entityVector{buildSTParameters(entities, parameterStringsToParse)};
        return std::make_unique<SuchThatClause>(RelationshipBuilder::determineRelationshipType(type),
                                                entityVector[0], entityVector[1]);
    } else {
        throw Exception("Cannot convert string to SuchThatClause: " + str);
    }
}

/**
 * Helper function to toSTClause
 *
 * Determine if parameters are:
 * literal, wildcard, integer, synonym
 */
std::vector<ClauseArgument*> SuchThatStrategy::buildSTParameters(const std::vector<Synonym>& entities,
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
