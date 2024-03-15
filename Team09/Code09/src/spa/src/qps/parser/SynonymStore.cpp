#include "SynonymStore.h"

void SynonymStore::storeSynonym(DesignEntityType type, const std::string& synName) {
    if (hasSynonymName(synName)) {
        throw QPSSemanticError();
    }
    storage.insert({synName, type});
}

bool SynonymStore::containsSynonym(Synonym syn) {
    auto type = storage.find(syn.getValue());
    if (type != storage.end()) {
        return storage[syn.getValue()] == syn.getType();
    }
    return false;
}

bool SynonymStore::hasSynonymName(const std::string& synName) {
    auto type = storage.find(synName);
    return !(type == storage.end());
}

Synonym SynonymStore::getSynonym(const std::string& synName) {
    if (!hasSynonymName(synName)) {
        throw QPSSemanticError();
    }
    return Synonym(storage[synName], synName);
}

DesignEntityType SynonymStore::getDesignEntityType(const std::string& synName) {
    if (!hasSynonymName(synName)) {
        throw QPSSemanticError();
    }
    return storage[synName];
}
