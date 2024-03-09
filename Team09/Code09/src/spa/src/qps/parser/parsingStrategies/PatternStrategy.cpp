#include "PatternStrategy.h"

std::unique_ptr<QueryClause> PatternStrategy::execute(std::vector<Synonym> entities, std::string str) const {
    std::smatch argMatch;
    if (std::regex_search(str, argMatch, QPSRegexes::PATTERN_ARGS)) {
        std::string assignSynonym = argMatch[1];
        std::string parameters = argMatch[2];

        std::vector<std::string> parameterStringsToParse{assignSynonym};
        std::vector<std::string> cleanedParameters{cleanParameters(parameters)};

        parameterStringsToParse.insert(parameterStringsToParse.end(), cleanedParameters.begin(),
                                       cleanedParameters.end());
        std::vector<ClauseArgument*> entityVector{buildPatternParameters(entities, parameterStringsToParse)};
        std::unique_ptr<PatternClause> patternClause{std::make_unique<PatternClause>(
                                                            entityVector[0], entityVector[1], entityVector[2])};

        return patternClause;
    } else {
        throw Exception("Cannot convert string to PatternClause: " + str);
    }
}

/**
 * Helper function to toPatternClause
 *
 * For pattern parameters:
 * first parameter is always synonym
 * second parameter is always entRef
 * third parameter is always expressionSpec
 */
std::vector<ClauseArgument*> PatternStrategy::buildPatternParameters(const std::vector<Synonym>& entities,
                                                               const std::vector<std::string>& strings) {
    std::vector<ClauseArgument*> results{};

    std::string ptSynonym = strings[0];
    std::string ptEntRef = strings[1];
    std::string ptExpressionSpec = strings[2];

    // first argument is synonym
    results.push_back(buildSynonym(entities, ptSynonym));

    // second argument is ent-ref
    if (isQuotedIdent(ptEntRef)) {
        results.push_back(new Literal(removeAllQuotations(removeAllWhitespaces(ptEntRef))));
    } else if (isWildcard(ptEntRef)) {
        results.push_back(new Wildcard());
    } else if (isSynonym(ptEntRef)) {
        results.push_back(buildSynonym(entities, ptEntRef));
    } else {
        throw Exception("Issues determining if Pattern EntRef is literal, wildcard, or synonym: " + ptEntRef);
    }

    // third argument is expression-spec OR wildcard
    if (isExpressionSpec(ptExpressionSpec)) {
        if (isWildcard(ptExpressionSpec)) {
            results.push_back(new Wildcard());
        } else {
            results.push_back(new ExpressionSpec(removeAllWhitespaces(ptExpressionSpec)));
        }
    } else {
        throw Exception("Pattern Expression-spec is not expression-spec or wildcard: " + ptEntRef);
    }

    return results;
}
