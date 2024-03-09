#include "StrategyUtils.h"
#include "../../ParserUtils.h"

#include "../../exceptions/Exception.h"

/**
 * Helper function to toSTClause, toPatternClause
 */
std::vector<std::string> cleanParameters(const std::string& parametersString) {
    std::vector<std::string> result{};
    std::istringstream iss(parametersString);
    std::string parameter;
    while (std::getline(iss, parameter, ',')) {
        parameter = trim(parameter);
        result.push_back(parameter);
    }
    return result;
}

/**
 * Helper function to buildSTParameters, buildPatternParameters
 */
Synonym* buildSynonym(const std::vector<Synonym>& entities, const std::string& str) {
    for (const Synonym& entity : entities) {
        if (entity.getValue() == str) {
            return new Synonym(entity.getType(), entity.getValue());
            break;  // We have already matched an entity, no need to continue searching
        }
    }
    throw Exception("Could not find Synonym when parsing clauses, amongst declarations: " + str);
}
