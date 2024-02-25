#include "ClauseResult.h"

ClauseResult::ClauseResult(bool booleanResult) : result(booleanResult) {}
ClauseResult::ClauseResult(std::vector<Synonym> synonyms, std::vector<SynonymValues> synonymValues)
    : result(std::make_pair(synonyms, synonymValues)) {}
ClauseResult::ClauseResult(Synonym synonym, SynonymValues values) {
    std::vector<Synonym> synonyms = {synonym};
    std::vector<SynonymValues> synonymValues = {values};
    result = std::make_pair(synonyms, synonymValues);
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

std::vector<SynonymValues> ClauseResult::getAllSynonymValues() const {
    return std::get<ComplexResult>(result).second;
}
