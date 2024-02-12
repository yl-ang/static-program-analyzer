#include <cstring>
#include "QueryEntity.h"
#include <iostream>
#include <algorithm>
#include <functional>
#include <unordered_map>

QueryEntity::QueryEntity(const EntityType& t, const std::string& n) : type(t), name(n) {}

EntityType QueryEntity::getType() const {
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

std::string QueryEntity::entityTypeToString(EntityType type) {
    switch (type) {
        case EntityType::STMT:      return "STMT";
        case EntityType::READ:      return "READ";
        case EntityType::CALL:      return "CALL";
        case EntityType::WHILE:     return "WHILE";
        case EntityType::IF:        return "IF";
        case EntityType::ASSIGN:    return "ASSIGN";
        case EntityType::VARIABLE:  return "VARIABLE";
        case EntityType::CONSTANT:  return "CONSTANT";
        case EntityType::PROCEDURE: return "PROCEDURE";
        default:                    return "UNKNOWN";
    }
}

EntityType QueryEntity::determineType(const std::string type) {
    EntityType entityType;
    if (type == "variable") {
        entityType = EntityType::VARIABLE;
    } else if (type == "constant") {
        entityType = EntityType::CONSTANT;
    } else if (type == "procedure") {
        entityType = EntityType::PROCEDURE;
    } else if (type == "stmt") {
        entityType = EntityType::STMT;
    } else if (type == "read") {
        entityType = EntityType::READ;
    } else if (type == "call") {
        entityType = EntityType::CALL;
    } else if (type == "while") {
        entityType = EntityType::WHILE;
    } else if (type == "if") {
        entityType = EntityType::IF;
    } else if (type == "assign") {
        entityType = EntityType::ASSIGN;
    } else {
        std::cout << "EntityType is not found in valid types: " << type << "\n";
        exit(1);
    }
    return entityType;
}
