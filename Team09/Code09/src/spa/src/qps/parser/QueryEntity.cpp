#include <cstring>
#include "QueryEntity.h"
#include <iostream>
#include <algorithm>
#include <functional>

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
    
    const static std::unordered_map<std::string,int> string_to_case{
        {"variable", 1},
        {"constant", 2},
        {"procedure", 3},
        {"stmt", 4},
        {"read", 5},
        {"call", 6},
        {"while", 7},
        {"if", 8},
        {"assign", 9}
    };

    // Checks if in map, otherwise case 0
    switch(string_to_case.count(type) ? string_to_case.at(type) : 0) {
        case 1:
            entityType = EntityType::VARIABLE;
            break;
        case 2:
            entityType = EntityType::CONSTANT;
            break;
        case 3:
            entityType = EntityType::PROCEDURE;
            break;
        case 4:
            entityType = EntityType::STMT;
            break;
        case 5:
            entityType = EntityType::READ;
            break;
        case 6: 
            entityType = EntityType::CALL;
            break;
        case 7:
            entityType = EntityType::WHILE;
            break;
        case 8:
            entityType = EntityType::IF;
            break;
        case 9:
            entityType = EntityType::ASSIGN;
            break;
        case 0:
            std::cout << "EntityType is not found in valid types: -" << type << "-\n";
            exit(1);
            break;
    }
    return entityType;
}
