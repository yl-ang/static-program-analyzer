#pragma once

#include <qps/parser/SynonymStore.h>

#include <string>

#include "ClauseArgument.h"

class SynonymStore;

enum class DesignEntityType {
    STMT,
    READ,
    PRINT,
    CALL,
    WHILE,
    IF,
    ASSIGN,
    VARIABLE,
    CONSTANT,
    PROCEDURE,
    UNKNOWN,
};

class Synonym : public ClauseArgument {
private:
    DesignEntityType type;
    std::string name;

public:
    Synonym(const DesignEntityType& t, const std::string& n);

    static DesignEntityType determineType(const std::string);

    bool isSynonym() const override;
    std::string getClauseType() const override;
    DesignEntityType getType() const;
    std::string getValue() const override;
    static std::string entityTypeToString(DesignEntityType);
    bool updateType(SynonymStore* synonyms);
    void print();
    bool operator==(const ClauseArgument& other) const override;
};
