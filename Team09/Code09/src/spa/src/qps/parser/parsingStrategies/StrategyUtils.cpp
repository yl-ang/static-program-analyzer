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
