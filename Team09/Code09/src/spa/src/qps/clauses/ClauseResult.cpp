#include "ClauseResult.h"

ClauseResult::ClauseResult(bool booleanResult) : result(booleanResult) {}
ClauseResult::ClauseResult(const std::vector<Synonym>& synonyms, const std::vector<Row>& synonymValues)
    : result(std::make_pair(synonyms, synonymValues)) {}
ClauseResult::ClauseResult(const Synonym& synonym, const std::vector<Row>& rows) {
    std::vector<Synonym> synonyms = {synonym};
    result = std::make_pair(synonyms, rows);
}

bool ClauseResult::isBoolean() const {
    return std::holds_alternative<bool>(result);
}

bool ClauseResult::getBoolean() const {
    return std::get<bool>(result);
}

std::vector<Synonym> ClauseResult::getSynonyms() const {
    return std::get<ComplexResult>(result).first;
}

std::vector<Row> ClauseResult::getAllSynonymValues() const {
    return std::get<ComplexResult>(result).second;
}

bool ClauseResult::isEmpty() const {
    return !isBoolean() && getAllSynonymValues().empty();
}
