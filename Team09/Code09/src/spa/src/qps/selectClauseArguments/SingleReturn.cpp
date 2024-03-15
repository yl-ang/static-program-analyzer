#include "SingleReturn.h"

SingleReturn::SingleReturn(const Synonym& s) : synonyms({s}) {}

bool SingleReturn::isSingleReturn() const {
    return true;
}

std::vector<Synonym> SingleReturn::getValue() const {
    return synonyms;
}

std::string SingleReturn::getSelectClauseType() const {
    return "SingleReturn";
}

bool SingleReturn::operator==(const SelectClauseArgument& other) const {
    if (!other.isSingleReturn()) {
        return false;
    }

    return this->getValue() == other.getValue();
}
