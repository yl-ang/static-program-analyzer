#include "PatternStrategy.h"

std::shared_ptr<QueryClause> PatternStrategy::execute(std::string str) const {
    std::string removedPattern = trim(str.substr(QPSConstants::PATTERN.size()));
    std::string assignSynonym;
    std::string parameters;
    std::tie(assignSynonym, parameters) = substringUntilDelimiter(removedPattern, "(");
    if (parameters.back() != ')') {
        throw QPSSyntaxError();
    }
    parameters.pop_back();
    assignSynonym = trim(assignSynonym);
    parameters = trim(parameters);

    std::vector<std::string> parameterStringsToParse{assignSynonym};
    std::vector<std::string> cleanedParameters = splitByDelimiter(parameters, ",");
    Validator::validatePatternSyntax(assignSynonym, parameters);

    parameterStringsToParse.insert(parameterStringsToParse.end(), cleanedParameters.begin(), cleanedParameters.end());
    std::vector<std::shared_ptr<ClauseArgument>> entityVector{buildPatternParameters(parameterStringsToParse)};
    std::shared_ptr<PatternClause> patternClause{
        std::make_shared<PatternClause>(entityVector[0], std::vector(entityVector.begin() + 1, entityVector.end()))};
    return patternClause;
}

std::vector<std::shared_ptr<ClauseArgument>> PatternStrategy::buildPatternParameters(
    const std::vector<std::string>& strings) {
    if (strings.size() == 3) {
        return buildTwoParameters(strings);
    } else if (strings.size() == 4) {
        return buildThreeParameters(strings);
    } else {
        throw QPSSyntaxError();
    }
}

/**
 * For pattern parameters:
 * first parameter is always synonym
 * second parameter is always entRef
 * third parameter is either wildcard or expression spec
 */
std::vector<std::shared_ptr<ClauseArgument>> PatternStrategy::buildTwoParameters(
    const std::vector<std::string>& strings) {
    std::vector<std::shared_ptr<ClauseArgument>> results{};

    std::string ptSynonym = strings[0];
    std::string ptEntRef = strings[1];
    std::string ptExpressionSpec = strings[2];

    // first argument is synonym
    results.push_back(std::make_shared<Synonym>(DesignEntityType::UNKNOWN, ptSynonym));

    // second argument is ent-ref
    if (isWildcard(ptEntRef)) {
        results.push_back(std::make_shared<Wildcard>());
    } else if (isQuotedIdent(ptEntRef)) {
        results.push_back(std::make_shared<Literal>(cleanQuotedIdent(ptEntRef)));
    } else if (isSynonym(ptEntRef)) {
        results.push_back(std::make_shared<Synonym>(DesignEntityType::UNKNOWN, ptEntRef));
    } else {
        throw QPSSyntaxError();
    }

    // third argument is expression-spec OR wildcard
    if (isWildcard(ptExpressionSpec)) {
        results.push_back(std::make_shared<Wildcard>());
    } else if (isExpressionSpec(ptExpressionSpec)) {
        results.push_back(std::make_shared<ExpressionSpec>(removeCharsFrom(ptExpressionSpec, QPSConstants::SPACE)));
    } else {
        throw QPSSyntaxError();
    }
    return results;
}

std::vector<std::shared_ptr<ClauseArgument>> PatternStrategy::buildThreeParameters(
    const std::vector<std::string>& strings) {
    std::vector<std::shared_ptr<ClauseArgument>> results{};

    std::string ptSynonym = strings[0];
    std::string ptEntRef = strings[1];
    std::string wildcard1 = strings[2];
    std::string wildcard2 = strings[3];

    // first argument is synonym
    results.push_back(std::make_shared<Synonym>(DesignEntityType::UNKNOWN, ptSynonym));

    // second argument is ent-ref
    if (isWildcard(ptEntRef)) {
        results.push_back(std::make_shared<Wildcard>());
    } else if (isQuotedIdent(ptEntRef)) {
        results.push_back(std::make_shared<Literal>(cleanQuotedIdent(ptEntRef)));
    } else if (isSynonym(ptEntRef)) {
        results.push_back(std::make_shared<Synonym>(DesignEntityType::UNKNOWN, ptEntRef));
    } else {
        throw QPSSyntaxError();
    }

    // third argument is wildcard
    if (isWildcard(wildcard1)) {
        results.push_back(std::make_shared<Wildcard>());
    } else {
        throw QPSSyntaxError();
    }

    // last argument is wildcard
    if (isWildcard(wildcard2)) {
        results.push_back(std::make_shared<Wildcard>());
    } else {
        throw QPSSyntaxError();
    }

    return results;
}
