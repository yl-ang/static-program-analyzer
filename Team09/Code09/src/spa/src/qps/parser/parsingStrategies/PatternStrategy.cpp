#include "PatternStrategy.h"

std::unique_ptr<QueryClause> PatternStrategy::execute(std::string str) const {
    std::smatch argMatch;
    if (std::regex_search(str, argMatch, QPSRegexes::PATTERN_ARGS)) {
        std::string assignSynonym = argMatch[1];
        std::string parameters = argMatch[2];

        std::vector<std::string> parameterStringsToParse{assignSynonym};
        std::vector<std::string> cleanedParameters{cleanParameters(parameters)};
        Validator::validatePatternSyntax(assignSynonym, parameters);

        parameterStringsToParse.insert(parameterStringsToParse.end(), cleanedParameters.begin(),
                                       cleanedParameters.end());
        std::vector<ClauseArgument*> entityVector{buildPatternParameters(parameterStringsToParse)};
        std::unique_ptr<PatternClause> patternClause{
            std::make_unique<PatternClause>(entityVector[0], entityVector[1], entityVector[2])};

        return patternClause;
    } else {
        throw QPSSyntaxError();
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
std::vector<ClauseArgument*> PatternStrategy::buildPatternParameters(const std::vector<std::string>& strings) {
    std::vector<ClauseArgument*> results{};

    std::string ptSynonym = strings[0];
    std::string ptEntRef = strings[1];
    std::string ptExpressionSpec = strings[2];

    // first argument is synonym
    results.push_back(new Synonym(DesignEntityType::UNKNOWN, ptSynonym));

    // second argument is ent-ref
    if (isQuotedIdent(ptEntRef)) {
        results.push_back(new Literal(removeAllQuotations(removeAllWhitespaces(ptEntRef))));
    } else if (isWildcard(ptEntRef)) {
        results.push_back(new Wildcard());
    } else if (isSynonym(ptEntRef)) {
        results.push_back(new Synonym(DesignEntityType::UNKNOWN, ptEntRef));
    } else {
        throw QPSSyntaxError();
    }

    // third argument is expression-spec OR wildcard
    if (isExpressionSpec(ptExpressionSpec)) {
        if (isWildcard(ptExpressionSpec)) {
            results.push_back(new Wildcard());
        } else {
            results.push_back(new ExpressionSpec(removeAllWhitespaces(ptExpressionSpec)));
        }
    } else {
        throw QPSSyntaxError();
    }

    return results;
}
