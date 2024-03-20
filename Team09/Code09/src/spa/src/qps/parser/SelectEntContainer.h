#pragma once

#include <string>
#include <vector>

#include "qps/clauseArguments/Synonym.h"

class SelectEntContainer {
private:
    Synonym buildSynonym(const std::string& syn) {
        if (isAttrRef(syn)) {
            std::vector<std::string> attrs = splitByDelimiter(syn, ".");
            return {DesignEntityType::UNKNOWN, attrs[0], Synonym::stringToAttributeEnum(attrs[1])};
        }
        return {DesignEntityType::UNKNOWN, syn};
    }

protected:
    std::vector<Synonym> container = {};

public:
    void add(const std::string& syn) {
        container.push_back(buildSynonym(syn));
    }

    virtual bool updateSynonyms(SynonymStore* store) = 0;

    virtual std::vector<Synonym> getSynonyms() = 0;
};

class Tuple : public SelectEntContainer {
    bool updateSynonyms(SynonymStore* store) override {
        for (Synonym& syn : container) {
            if (!syn.updateType(store)) {
                return false;
            }
        }
        return true;
    }

    std::vector<Synonym> getSynonyms() override {
        return container;
    }
};

class Elem : public SelectEntContainer {
    bool updateSynonyms(SynonymStore* store) override {
        Synonym& syn = container[0];
        if (!syn.updateType(store)) {
            return isBoolean(syn.getValue());
        }
        return true;
    }

    std::vector<Synonym> getSynonyms() override {
        if (isBoolean(container[0].getValue())) {
            return {};
        }
        return container;
    }
};
