#pragma once

#include <optional>
#include <string>

#include "ClauseArgument.h"
#include "qps/parser/SynonymStore.h"

class SynonymStore;

enum class SynonymAttributeType {
    PROCNAME,
    VARNAME,
    VALUE,
    STMTNUM,
    NONE,
};

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
    SynonymAttributeType attrType{SynonymAttributeType::NONE};

    inline static const std::unordered_map<std::string, DesignEntityType> STRING_TO_TYPE_MAP = {
        {QPSConstants::VARIABLE, DesignEntityType::VARIABLE},   {QPSConstants::CONSTANT, DesignEntityType::CONSTANT},
        {QPSConstants::PROCEDURE, DesignEntityType::PROCEDURE}, {QPSConstants::STMT, DesignEntityType::STMT},
        {QPSConstants::READ, DesignEntityType::READ},           {QPSConstants::CALL, DesignEntityType::CALL},
        {QPSConstants::WHILE, DesignEntityType::WHILE},         {QPSConstants::IF, DesignEntityType::IF},
        {QPSConstants::ASSIGN, DesignEntityType::ASSIGN},       {QPSConstants::PRINT, DesignEntityType::PRINT}};

public:
    Synonym(const DesignEntityType& t, const std::string& n);
    Synonym(const DesignEntityType& t, const std::string& n, const SynonymAttributeType& a);

    static DesignEntityType determineType(const std::string);
    static SynonymAttributeType stringToAttributeEnum(const std::string& str);

    bool isSynonym() const override;
    std::string getClauseType() const override;
    DesignEntityType getType() const;
    std::string getName() const;
    std::string getValue() const override;
    std::optional<SynonymAttributeType> getAttr() const;
    bool updateType(SynonymStore* synonyms);
    bool operator==(const ClauseArgument& other) const override;
    bool validateAttribute() const;

    // Checks that the other Synonym has the same type and value, regardless of attribute.
    bool equalSynonymValue(const Synonym& other) const;
    Synonym getWithoutAttribute() const;
};
