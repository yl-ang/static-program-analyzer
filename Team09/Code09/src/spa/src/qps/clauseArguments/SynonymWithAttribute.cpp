#include "SynonymWithAttribute.h"

#include <unordered_set>

namespace {
std::unordered_map<DesignEntityType, std::unordered_set<SynonymAttributeType>> DESIGN_ENTITY_TYPE_VALID_ATTRIBUTES = {
    {DesignEntityType::IF, {SynonymAttributeType::STMTNUM}},
    {DesignEntityType::STMT, {SynonymAttributeType::STMTNUM}},
    {DesignEntityType::WHILE, {SynonymAttributeType::STMTNUM}},
    {DesignEntityType::ASSIGN, {SynonymAttributeType::STMTNUM}},

    {DesignEntityType::READ, {SynonymAttributeType::STMTNUM, SynonymAttributeType::VARNAME}},
    {DesignEntityType::PRINT, {{SynonymAttributeType::STMTNUM}, SynonymAttributeType::VARNAME}},

    {DesignEntityType::PROCEDURE, {SynonymAttributeType::PROCNAME}},
    {DesignEntityType::CALL, {SynonymAttributeType::STMTNUM, SynonymAttributeType::PROCNAME}},

    {DesignEntityType::CONSTANT, {SynonymAttributeType::VALUE}},
    {DesignEntityType::VARIABLE, {SynonymAttributeType::VARNAME}},

    {DesignEntityType::UNKNOWN, {}},
};
}  // namespace

SynonymWithAttribute::SynonymWithAttribute(const DesignEntityType& t, const std::string& n,
                                           const SynonymAttributeType& attrType)
    : Synonym(t, n), attrType(attrType) {
    if (const auto& validAttributes = DESIGN_ENTITY_TYPE_VALID_ATTRIBUTES[t];
        validAttributes.find(attrType) == validAttributes.end()) {
        throw QPSSemanticError();
    }
}

std::optional<SynonymAttributeType> SynonymWithAttribute::getAttr() const {
    return this->attrType;
}
