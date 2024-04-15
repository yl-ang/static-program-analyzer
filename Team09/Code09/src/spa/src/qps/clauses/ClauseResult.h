#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include "qps/clauseArguments/Synonym.h"
#include "qps/evaluator/Table.h"

using SynonymKey = std::string;
using SynonymValues = std::vector<std::string>;
using ComplexResult = std::pair<std::vector<Synonym>, std::vector<Row>>;

class ClauseResult {
private:
    std::variant<bool, ComplexResult> result;

public:
    ClauseResult(bool);
    ClauseResult(const std::vector<Synonym>&, const std::vector<Row>&);
    ClauseResult(const Synonym&, const std::vector<Row>&);

    bool isBoolean() const;
    bool getBoolean() const;
    std::vector<Synonym> getSynonyms() const;
    std::vector<Row> getAllSynonymValues() const;
    bool isEmpty() const;
};
