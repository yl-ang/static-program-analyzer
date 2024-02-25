#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include "qps/clauseArguments/Synonym.h"

using SynonymKey = std::string;
using SynonymValues = std::vector<std::string>;
using ComplexResult = std::pair<std::vector<Synonym>, std::vector<SynonymValues>>;

class ClauseResult {
private:
    std::variant<bool, ComplexResult> result;

public:
    ClauseResult(bool);
    ClauseResult(std::vector<Synonym>, std::vector<SynonymValues>);
    ClauseResult(Synonym, SynonymValues);

    bool isBoolean() const;
    bool getBoolean() const;
    std::vector<Synonym> getSynonyms() const;
    std::vector<SynonymValues> getAllSynonymValues() const;
};
