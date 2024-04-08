#include "SelectEntContainer.h"

#include "qps/clauseArguments/Synonym.h"

Synonym SelectEntContainer::buildSynonym(const std::string& syn) {
    if (isAttrRef(syn)) {
        std::vector<std::string> attrs = splitByDelimiter(syn, ".");
        return {DesignEntityType::UNKNOWN, attrs[0], Synonym::stringToAttributeEnum(attrs[1])};
    }
    return {DesignEntityType::UNKNOWN, syn};
}

void SelectEntContainer::add(const std::string& syn) {
    container.push_back(buildSynonym(syn));
}
