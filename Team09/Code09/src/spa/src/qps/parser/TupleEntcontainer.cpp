#include "TupleEntContainer.h"

bool Tuple::updateSynonyms(SynonymStore* store) {
    for (Synonym& syn : container) {
        if (!syn.updateType(store)) {
            return false;
        }
    }
    return true;
}

std::vector<Synonym> Tuple::getSynonyms() {
    return container;
}
