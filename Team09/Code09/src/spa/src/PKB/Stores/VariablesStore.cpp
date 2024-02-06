#include "VariablesStore.h"

const std::unordered_set<std::string>& VariablesStore::getVariablesStore() const {
    return variablesStore;
}

void VariablesStore::setVariables(const std::unordered_set<std::string>& inputVariables) {
    variablesStore = inputVariables;
}

bool VariablesStore::containsVariable(const std::string &variable) const {
    return variablesStore.count(variable) > 0;
}
