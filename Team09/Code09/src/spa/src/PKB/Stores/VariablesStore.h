#pragma once

#include <unordered_set>
#include <string>
#include <utility>
#include "PKB/Utils/HashSpecialization.h"

class VariablesStore {
private:
    std::unordered_set<std::string> variablesStore;

public:
    const std::unordered_set<std::string>& getVariablesStore() const;
    void setVariables(const std::unordered_set<std::string>& inputVariables);
    bool containsVariable(const std::string &variable) const;
};
