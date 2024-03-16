#include "SynonymStore.h"

SynonymStore::SynonymStore(std::unordered_map<std::string, DesignEntityType> store) : storage(store) {}

void SynonymStore::storeSynonym(DesignEntityType type, const std::string& synName) {
    if (hasSynonymName(synName)) {
        isSemanticallyCorrect = false;
    }
    storage.insert({synName, type});
}

bool SynonymStore::isValidStore() {
    return isSemanticallyCorrect;
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
        return Synonym(DesignEntityType::UNKNOWN, synName);
    }
    return Synonym(storage[synName], synName);
}

DesignEntityType SynonymStore::getDesignEntityType(const std::string& synName) {
    if (!hasSynonymName(synName)) {
        return DesignEntityType::UNKNOWN;
    }
    return storage[synName];
}

bool SynonymStore::operator==(const SynonymStore& other) const {
    return storage == other.storage;
}
