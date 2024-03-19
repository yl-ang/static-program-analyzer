#include "Synonym.h"

class SynonymWithAttribute : public Synonym {
private:
    const SynonymAttributeType& attrType;

public:
    SynonymWithAttribute(const DesignEntityType& t, const std::string& n, const SynonymAttributeType& attrType);

    std::optional<SynonymAttributeType> getAttr() const override;
};
