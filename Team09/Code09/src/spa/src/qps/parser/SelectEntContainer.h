#pragma once

#include <qps/clauseArguments/Synonym.h>

#include <string>
#include <vector>

class SelectEntContainer {
protected:
    std::vector<Synonym> container = {};

public:
    void add(const std::string& syn) {
        container.push_back(Synonym(DesignEntityType::UNKNOWN, syn));
    }

    virtual bool updateSynonyms(SynonymStore* store) = 0;

    virtual std::vector<Synonym> getSynonyms() = 0;
};

class Tuple : public SelectEntContainer {
    bool updateSynonyms(SynonymStore* store) {
        for (Synonym& syn : container) {
            if (!syn.updateType(store)) {
                return false;
            }
        }
        return true;
    }

    std::vector<Synonym> getSynonyms() {
        return container;
    }
};

class Elem : public SelectEntContainer {
    bool updateSynonyms(SynonymStore* store) {
        Synonym& syn = container[0];
        if (!syn.updateType(store)) {
            return isBoolean(syn.getValue());
        }
        return true;
    }

    std::vector<Synonym> getSynonyms() {
        if (isBoolean(container[0].getValue())) {
            return {};
        }
        return container;
    }
};
