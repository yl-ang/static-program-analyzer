#include "ValidatorSynonymStore.h"

void ValidatorSynonymStore::storeSynonymWithStatement(const std::string& declarationStatement) {
    std::string synType;
    std::string remainingString;
    std::tie(synType, remainingString) = substringUntilDelimiter(declarationStatement, " ");
    remainingString = remainingString.substr(0, remainingString.size() - 1);  // Remove ending ';'

    std::vector<std::string> synNames = splitByDelimiter(remainingString, ",");
    for (std::string synName : synNames) {
        auto variable = storage.find(synName);

        // Semantic Error: QPS Grammar other rules 1
        if (variable == storage.end()) {
            storage.insert({synName, synType});
        } else {
            throw QPSSemanticError();
        }
    }
}

bool ValidatorSynonymStore::containsSynonym(const std::string& synName, const std::string& synType) {
    auto variable = storage.find(synName);
    if (variable == storage.end()) {
        return false;
    }

    return variable->second == synType;
}

bool ValidatorSynonymStore::containsSynonymName(const std::string& synName) {
    auto variable = storage.find(synName);
    return !(variable == storage.end());
}
