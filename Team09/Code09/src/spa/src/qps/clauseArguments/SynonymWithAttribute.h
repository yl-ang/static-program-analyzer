#include "Synonym.h"

class SynonymWithAttribute : public Synonym {
private:
    const SynonymAttributeType& attrType;

public:
    SynonymWithAttribute(const DesignEntityType& t, const std::string& n, const SynonymAttributeType& attrType)
        : Synonym(t, n), attrType(attrType);

    std::optional<SynonymAttributeType> getAttr() override const {
        return this->attrType;
    };
};
