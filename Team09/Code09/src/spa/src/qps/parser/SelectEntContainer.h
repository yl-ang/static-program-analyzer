#pragma once

#include <string>
#include <vector>

#include "qps/GrammarUtils.h"
#include "qps/clauseArguments/Synonym.h"

class SelectEntContainer {
private:
    Synonym buildSynonym(const std::string& syn);

protected:
    std::vector<Synonym> container = {};

public:
    void add(const std::string& syn);

    virtual bool updateSynonyms(SynonymStore* store) = 0;

    virtual std::vector<Synonym> getSynonyms() = 0;
};
