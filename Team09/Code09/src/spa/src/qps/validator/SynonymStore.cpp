#include "SynonymStore.h"

void SynonymStore::storeSynonym(const std::string& synName, const std::string& synType) {
    auto variable = storage.find(synName);

    // Semantic Error: QPS Grammar other rules 1
    if (variable == storage.end()) {
        storage.insert({synName, synType});
    } else {
        throw QPSSemanticError();
    }
}

bool SynonymStore::containsSynonym(const std::string& synName, const std::string& synType) {
    auto variable = storage.find(synName);
    if (variable == storage.end()) {
        return false;
    }

    return variable->second == synType;
}

bool SynonymStore::containsSynonymName(const std::string& synName) {
    auto variable = storage.find(synName);
    return !(variable == storage.end());
}
