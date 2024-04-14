#include "SuchThatStrategy.h"

std::shared_ptr<QueryClause> SuchThatStrategy::execute(std::string str) const {
    std::smatch argMatch;
    if (std::regex_search(str, argMatch, std::regex(QPSConstants::SUCHTHAT_ARGS))) {
        if (argMatch.suffix().matched) {
            throw QPSSyntaxError();
        }
        std::string suchThatNot = argMatch.prefix();
        std::string type = argMatch[1];
        std::string parameters = argMatch[2];

        Validator::validateSuchThatSyntax(type, parameters);
        std::vector<std::string> parameterStringsToParse = splitByDelimiter(parameters, ",");
        std::vector<std::shared_ptr<ClauseArgument>> entityVector{buildSTParameters(parameterStringsToParse)};
        std::shared_ptr<SuchThatClause> suchThatClause = std::make_shared<SuchThatClause>(
            RelationshipBuilder::determineRelationshipType(type), entityVector[0], entityVector[1]);

        if (isNotRelation(suchThatNot)) {
            suchThatClause->setNegation(true);
        }

        return suchThatClause;
    } else {
        throw QPSSyntaxError();
    }
}

/**
 * Helper function to toSTClause
 *
 * Determine if parameters are:
 * literal, wildcard, integer, synonym
 */
std::vector<std::shared_ptr<ClauseArgument>> SuchThatStrategy::buildSTParameters(
    const std::vector<std::string>& strings) {
    std::vector<std::shared_ptr<ClauseArgument>> results{};
    for (const std::string& str : strings) {
        if (isQuotedIdent(str)) {
            results.push_back(std::make_shared<Literal>(cleanQuotedIdent(str)));
        } else if (isWildcard(str)) {
            results.push_back(std::make_shared<Wildcard>());
        } else if (isInteger(str)) {
            results.push_back(std::make_shared<Integer>(str));
        } else if (isSynonym(str)) {
            results.push_back(std::make_shared<Synonym>(DesignEntityType::UNKNOWN, str));
        } else {
            throw QPSSyntaxError();
        }
    }
    return results;
}
