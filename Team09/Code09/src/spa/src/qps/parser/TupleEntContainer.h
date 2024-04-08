#pragma once
#include "SelectEntContainer.h"

class Tuple : public SelectEntContainer {
    bool updateSynonyms(SynonymStore* store) override;

    std::vector<Synonym> getSynonyms() override;
};
