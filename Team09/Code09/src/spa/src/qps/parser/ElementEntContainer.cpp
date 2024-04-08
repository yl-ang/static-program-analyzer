#include "ElementEntContainer.h"

bool Elem::updateSynonyms(SynonymStore* store) {
    Synonym& syn = container[0];
    if (!syn.updateType(store)) {
        return isBoolean(syn.getValue());
    }
    return true;
}

std::vector<Synonym> Elem::getSynonyms() {
    if (isBoolean(container[0].getValue()) && container[0].getType() == DesignEntityType::UNKNOWN) {
        return {};
    }
    return container;
}
