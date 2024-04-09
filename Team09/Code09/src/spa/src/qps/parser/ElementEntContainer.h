#pragma once
#include "SelectEntContainer.h"

class Elem : public SelectEntContainer {
    bool updateSynonyms(SynonymStore* store) override;

    std::vector<Synonym> getSynonyms() override;
};
