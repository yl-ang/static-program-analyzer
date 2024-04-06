#include "WithStrategy.h"

std::shared_ptr<QueryClause> WithStrategy::execute(std::string str) const {
    std::string removedWith = trim(str.substr(QPSConstants::WITH.size()));

    bool hasNot = isNotRelation(removedWith);

    std::string mainClause = hasNot ? trim(removedWith.substr(QPSConstants::NOT.size())) : removedWith;

    Validator::validateWithSyntax(mainClause);
    std::vector<std::string> parameterStringsToParse = splitByDelimiter(mainClause, "=");
    std::vector<std::shared_ptr<ClauseArgument>> entityVector{buildWithParameters(parameterStringsToParse)};
    std::shared_ptr<WithClause> withClause{std::make_shared<WithClause>(entityVector[0], entityVector[1])};

    if (hasNot) {
        withClause->setNegation(true);
    }

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
