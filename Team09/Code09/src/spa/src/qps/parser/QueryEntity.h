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
    QueryEntity(const EntityType&, const std::string&);

    static EntityType determineType(const std::string);
    static QueryEntity createVariable(const std::string&);
    static QueryEntity createConstant(const std::string&);
    static QueryEntity createProcedure(const std::string&);

    EntityType getType() const;
    std::string getName() const;
    bool operator==(const QueryEntity& other) const;
};
