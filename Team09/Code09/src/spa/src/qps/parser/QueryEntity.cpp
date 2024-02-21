#include "QueryEntity.h"

#include <algorithm>
#include <cstring>
#include <functional>
#include <iostream>
#include <unordered_map>

QueryEntity::QueryEntity(const DesignEntityType& t, const std::string& n) : type(t), name(n) {}

DesignEntityType QueryEntity::getType() const {
    return type;
}

std::string QueryEntity::getName() const {
    return name;
}

bool QueryEntity::operator==(const QueryEntity& other) const {
    return type == other.type && name == other.name;
}

void QueryEntity::print() {
    std::cout << "type, name: " << QueryEntity::entityTypeToString(type) << ", " << name << "\n";
}

std::string QueryEntity::entityTypeToString(DesignEntityType type) {
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
    default:
        return "UNKNOWN";
    }
}

DesignEntityType QueryEntity::determineType(const std::string type) {
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
    } else {
        std::cout << "EntityType is not found in valid types: " << type << "\n";
        exit(1);
    }
    return entityType;
}
