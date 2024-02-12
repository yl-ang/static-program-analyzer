#pragma once

#include <string>

enum class EntityType {
    STMT,
    READ,
    CALL, 
    WHILE, 
    IF, 
    ASSIGN, 
    VARIABLE, 
    CONSTANT, 
    PROCEDURE
};

class QueryEntity {
private:
    EntityType type;
    std::string name;

public:
    QueryEntity(const EntityType& t, const std::string& n);

    static EntityType determineType(const std::string);

    EntityType getType() const;
    std::string getName() const;
    static std::string entityTypeToString(EntityType);
    void print();
    bool operator==(const QueryEntity& other) const;
};
