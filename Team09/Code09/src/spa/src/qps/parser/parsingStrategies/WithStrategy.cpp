#include "WithStrategy.h"

std::unique_ptr<QueryClause> WithStrategy::execute(std::string str) const {
    std::string prefix;
    std::string mainClause;
    std::tie(prefix, mainClause) = substringUntilDelimiter(str, QPSConstants::SPACE);

    Validator::validateWithSyntax(mainClause);
    std::vector<std::string> parameterStringsToParse = splitByDelimiter(mainClause, "=");
    std::vector<std::shared_ptr<ClauseArgument>> entityVector{buildWithParameters(parameterStringsToParse)};
    std::unique_ptr<WithClause> withClause{std::make_unique<WithClause>(entityVector[0], entityVector[1])};

    return withClause;
}

/**
 * Determine if parameters are:
 * literal, integer, synonym with attribute
 */
std::vector<std::shared_ptr<ClauseArgument>> WithStrategy::buildWithParameters(
    const std::vector<std::string>& strings) {
    std::vector<std::shared_ptr<ClauseArgument>> results{};
    for (const std::string& str : strings) {
        if (isQuotedIdent(str)) {
            results.push_back(std::make_shared<Literal>(cleanQuotedIdent(str)));
        } else if (isInteger(str)) {
            results.push_back(std::make_shared<Integer>(str));
        } else if (isAttrRef(str)) {
            std::vector<std::string> attrs = splitByDelimiter(str, ".");
            results.push_back(std::make_shared<Synonym>(DesignEntityType::UNKNOWN, attrs[0],
                                                        Synonym::stringToAttributeEnum(attrs[0])));
        } else {
            throw QPSSyntaxError();
        }
    }
    return results;
}
