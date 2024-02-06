#pragma once

#include <unordered_set>
#include <string>
#include "PKB/PKB.h"
#include "PKB/Utils/HashSpecialization.h"

class PKBFacadeReader {
public:
    explicit PKBFacadeReader(PKB &pkbReference);

    // Getters for entities
    [[nodiscard]] std::unordered_set<std::string> getVariables() const;
    [[nodiscard]] std::unordered_set<int> getStmts() const;
    [[nodiscard]] std::unordered_set<int> getConstants() const;
    [[nodiscard]] std::unordered_set<std::string> getProcedures() const;

private:
    PKB* pkbReference = nullptr;
};
