#include "Synonym.h"

#include <algorithm>
#include <cstring>
#include <functional>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

#include "qps/exceptions/QPSSyntaxError.h"

namespace {
std::unordered_map<DesignEntityType, std::unordered_set<SynonymAttributeType>> DESIGN_ENTITY_TYPE_VALID_ATTRIBUTES = {
    {DesignEntityType::IF, {SynonymAttributeType::STMTNUM}},
    {DesignEntityType::STMT, {SynonymAttributeType::STMTNUM}},
    {DesignEntityType::WHILE, {SynonymAttributeType::STMTNUM}},
    {DesignEntityType::ASSIGN, {SynonymAttributeType::STMTNUM}},

    {DesignEntityType::READ, {SynonymAttributeType::STMTNUM, SynonymAttributeType::VARNAME}},
    {DesignEntityType::PRINT, {SynonymAttributeType::STMTNUM, SynonymAttributeType::VARNAME}},

    {DesignEntityType::PROCEDURE, {SynonymAttributeType::PROCNAME}},
    {DesignEntityType::CALL, {SynonymAttributeType::STMTNUM, SynonymAttributeType::PROCNAME}},

    {DesignEntityType::CONSTANT, {SynonymAttributeType::VALUE}},
    {DesignEntityType::VARIABLE, {SynonymAttributeType::VARNAME}},

    {DesignEntityType::UNKNOWN, {}},
};

const std::unordered_map<std::string, SynonymAttributeType> STRING_TO_ATTRIBUTE_MAP = {
    {QPSConstants::ATTR_PROC_NAME, SynonymAttributeType::PROCNAME},
    {QPSConstants::ATTR_VAR_NAME, SynonymAttributeType::VARNAME},
    {QPSConstants::ATTR_VALUE, SynonymAttributeType::VALUE},
    {QPSConstants::ATTR_STMT_NO, SynonymAttributeType::STMTNUM},
};
}  // namespace

Synonym::Synonym(const DesignEntityType& t, const std::string& n) : type(t), name(n) {}

Synonym::Synonym(const DesignEntityType& t, const std::string& n, const SynonymAttributeType& a)
    : type(t), name(n), attrType(a) {}

DesignEntityType Synonym::getType() const {
    return type;
}

std::string Synonym::getName() const {
    return name;
}

std::string Synonym::getValue() const {
    if (getAttr().has_value()) {
        const auto it = std::find_if(STRING_TO_ATTRIBUTE_MAP.begin(), STRING_TO_ATTRIBUTE_MAP.end(),
                                     [this](const std::pair<std::string, SynonymAttributeType>& pair) {
                                         return pair.second == this->getAttr().value();
                                     });

        return name + "." + it->first;
    }
    return name;
}

bool Synonym::operator==(const ClauseArgument& other) const {
    const Synonym* synonym = dynamic_cast<const Synonym*>(&other);
    if (!synonym) {
        return false;
    }

    return this->getType() == synonym->getType() && this->getValue() == synonym->getValue() &&
           this->getAttr() == synonym->getAttr();
}

bool Synonym::equalSynonymValue(const Synonym& other) const {
    return this->getType() == other.getType() && this->getName() == other.getName();
}

bool Synonym::isSynonym() const {
    return true;
}

std::string Synonym::getClauseType() const {
    return "Synonym";
}

SynonymAttributeType Synonym::stringToAttributeEnum(const std::string& str) {
    return STRING_TO_ATTRIBUTE_MAP.at(str);
}

std::optional<SynonymAttributeType> Synonym::getAttr() const {
    if (this->attrType == SynonymAttributeType::NONE) {
        return std::nullopt;
    }
    return this->attrType;
}

bool Synonym::validateAttribute() const {
    auto validAttributes = DESIGN_ENTITY_TYPE_VALID_ATTRIBUTES[type];

    if (!this->getAttr().has_value()) {
        return true;
    }

    return validAttributes.find(this->getAttr().value()) != validAttributes.end();
}

bool Synonym::updateType(SynonymStore* store) {
    type = store->getDesignEntityType(name);
    return validateAttribute() && type != DesignEntityType::UNKNOWN;
}

Synonym Synonym::getWithoutAttribute() const {
    return Synonym{getType(), getName()};
}

std::string Synonym::entityTypeToString(DesignEntityType type) {
    switch (type) {
    case DesignEntityType::STMT:
        return "STMT";
    case DesignEntityType::READ:
        return "READ";
    case DesignEntityType::CALL:
        return "CALL";
    case DesignEntityType::WHILE:
        return "WHILE";
    case DesignEntityType::IF:
        return "IF";
    case DesignEntityType::ASSIGN:
        return "ASSIGN";
    case DesignEntityType::VARIABLE:
        return "VARIABLE";
    case DesignEntityType::CONSTANT:
        return "CONSTANT";
    case DesignEntityType::PROCEDURE:
        return "PROCEDURE";
    case DesignEntityType::PRINT:
        return "PRINT";
    default:
        throw Exception("EntityType to string is not in valid EntityTypes");
    }
}

DesignEntityType Synonym::determineType(const std::string type) {
    DesignEntityType entityType;
    if (type == "variable") {
        entityType = DesignEntityType::VARIABLE;
    } else if (type == "constant") {
        entityType = DesignEntityType::CONSTANT;
    } else if (type == "procedure") {
        entityType = DesignEntityType::PROCEDURE;
    } else if (type == "stmt") {
        entityType = DesignEntityType::STMT;
    } else if (type == "read") {
        entityType = DesignEntityType::READ;
    } else if (type == "call") {
        entityType = DesignEntityType::CALL;
    } else if (type == "while") {
        entityType = DesignEntityType::WHILE;
    } else if (type == "if") {
        entityType = DesignEntityType::IF;
    } else if (type == "assign") {
        entityType = DesignEntityType::ASSIGN;
    } else if (type == "print") {
        entityType = DesignEntityType::PRINT;
    } else {
        throw QPSSyntaxError();
    }
    return entityType;
}
