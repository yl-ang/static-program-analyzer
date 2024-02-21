#pragma once

#include <string>

enum class DesignEntityType {
    STMT,
    READ,
    CALL,
    WHILE,
    IF,
    ASSIGN,
    VARIABLE,
    CONSTANT,
    PROCEDURE,
};

class QueryEntity {
private:
    DesignEntityType type;
    std::string name;

public:
    QueryEntity(const DesignEntityType& t, const std::string& n);

    static DesignEntityType determineType(const std::string);

    DesignEntityType getType() const;
    std::string getName() const;
    static std::string entityTypeToString(DesignEntityType);
    void print();
    bool operator==(const QueryEntity& other) const;
};
