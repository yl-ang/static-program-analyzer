#include "Synonym.h"

#include <algorithm>
#include <cstring>
#include <functional>
#include <iostream>
#include <unordered_map>

#include "qps/exceptions/QPSSyntaxError.h"

Synonym::Synonym(const DesignEntityType& t, const std::string& n) : type(t), name(n) {}

DesignEntityType Synonym::getType() const {
    return type;
}

std::string Synonym::getValue() const {
    return name;
}

bool Synonym::operator==(const ClauseArgument& other) const {
    const Synonym* synonym = dynamic_cast<const Synonym*>(&other);
    if (!synonym) {
        return false;
    }

    return this->getType() == (*synonym).getType() &&
        this->getValue() == (*synonym).getValue();
}

void Synonym::print() {
    std::cout << "type, name: " << Synonym::entityTypeToString(type) << ", " << name << "\n";
}

bool Synonym::isSynonym() const {
    return true;
}

std::string Synonym::getClauseType() const {
    return "Synonym";
}

bool Synonym::updateType(SynonymStore* store) {
    type = store->getDesignEntityType(name);
    return type != DesignEntityType::UNKNOWN;
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
