#include "PatternStrategy.h"

std::unique_ptr<QueryClause> PatternStrategy::execute(std::string str) const {
    std::smatch argMatch;
    if (std::regex_search(str, argMatch, QPSRegexes::PATTERN_ARGS)) {
        if (argMatch.suffix().matched) {
            throw QPSSyntaxError();
        }
        std::string assignSynonym = argMatch[1];
        std::string parameters = argMatch[2];

        std::vector<std::string> parameterStringsToParse{assignSynonym};
        std::vector<std::string> cleanedParameters = splitByDelimiter(parameters, ",");
        Validator::validatePatternSyntax(assignSynonym, parameters);

        parameterStringsToParse.insert(parameterStringsToParse.end(), cleanedParameters.begin(),
                                       cleanedParameters.end());
        std::vector<ClauseArgument*> entityVector{buildPatternParameters(parameterStringsToParse)};
        std::unique_ptr<PatternClause> patternClause{std::make_unique<PatternClause>(
            entityVector[0], std::vector(entityVector.begin() + 1, entityVector.end()))};
        return patternClause;
    } else {
        throw QPSSyntaxError();
    }
}

std::vector<ClauseArgument*> PatternStrategy::buildPatternParameters(const std::vector<std::string>& strings) {
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
std::vector<ClauseArgument*> PatternStrategy::buildTwoParameters(const std::vector<std::string>& strings) {
    std::vector<ClauseArgument*> results{};

    std::string ptSynonym = strings[0];
    std::string ptEntRef = strings[1];
    std::string ptExpressionSpec = strings[2];

    // first argument is synonym
    results.push_back(new Synonym(DesignEntityType::UNKNOWN, ptSynonym));

    // second argument is ent-ref
    if (isWildcard(ptEntRef)) {
        results.push_back(new Wildcard());
    } else if (isQuotedIdent(ptEntRef)) {
        results.push_back(new Literal(cleanQuotedIdent(ptEntRef)));
    } else if (isSynonym(ptEntRef)) {
        results.push_back(new Synonym(DesignEntityType::UNKNOWN, ptEntRef));
    } else {
        throw QPSSyntaxError();
    }

    // third argument is expression-spec OR wildcard
    if (isWildcard(ptExpressionSpec)) {
        results.push_back(new Wildcard());
    } else if (isExpressionSpec(ptExpressionSpec)) {
        results.push_back(new ExpressionSpec(removeCharsFrom(ptExpressionSpec, QPSConstants::SPACE)));
    } else {
        throw QPSSyntaxError();
    }
    return results;
}

std::vector<ClauseArgument*> PatternStrategy::buildThreeParameters(const std::vector<std::string>& strings) {
    std::vector<ClauseArgument*> results{};

    std::string ptSynonym = strings[0];
    std::string ptEntRef = strings[1];
    std::string wildcard1 = strings[2];
    std::string wildcard2 = strings[3];

    // first argument is synonym
    results.push_back(new Synonym(DesignEntityType::UNKNOWN, ptSynonym));

    // second argument is ent-ref
    if (isWildcard(ptEntRef)) {
        results.push_back(new Wildcard());
    } else if (isQuotedIdent(ptEntRef)) {
        results.push_back(new Literal(cleanQuotedIdent(ptEntRef)));
    } else if (isSynonym(ptEntRef)) {
        results.push_back(new Synonym(DesignEntityType::UNKNOWN, ptEntRef));
    } else {
        throw QPSSyntaxError();
    }

    // third argument is wildcard
    if (isWildcard(wildcard1)) {
        results.push_back(new Wildcard());
    } else {
        throw QPSSyntaxError();
    }

    // third argument is wildcard
    if (isWildcard(wildcard2)) {
        results.push_back(new Wildcard());
    } else {
        throw QPSSyntaxError();
    }

    return results;
}
