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
};

static inline const std::unordered_map<std::string, SynonymAttributeType> STRING_TO_ATTRIBUTE_MAP = {
    {"procName", SynonymAttributeType::PROCNAME},
    {"varName", SynonymAttributeType::VARNAME},
    {"value", SynonymAttributeType::VALUE},
    {"stmt#", SynonymAttributeType::STMTNUM}};

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

    virtual std::optional<SynonymAttributeType> getAttr() const {
        return std::nullopt;
    }
};
