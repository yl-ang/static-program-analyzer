#pragma once

#include <unordered_map>

#include "PKB/PKBClient/PKBFacadeReader.h"
#include "pkb/Utils/DataTypes.h"
#include "qps/clauseArguments/Synonym.h"
#include "qps/clauses/ClauseResult.h"
#include "qps/exceptions/Exception.h"

enum class RelationshipType { FOLLOWS, FOLLOWS_STAR, PARENT, PARENT_STAR, MODIFIES, USES, NEXT };  // update here

static std::unordered_map<DesignEntityType, StatementType> DESIGN_ENTITY_TYPE_TO_STMT_TYPE_MAP = {
    {DesignEntityType::ASSIGN, StatementType::ASSIGN}, {DesignEntityType::WHILE, StatementType::WHILE},
    {DesignEntityType::IF, StatementType::IF},         {DesignEntityType::CALL, StatementType::CALL},
    {DesignEntityType::READ, StatementType::READ},     {DesignEntityType::PRINT, StatementType::PRINT},
};

static const std::unordered_map<std::string, RelationshipType> RELATIONSHIP_TYPE_MAP = {
    {"Follows", RelationshipType::FOLLOWS}, {"Follows*", RelationshipType::FOLLOWS_STAR},
    {"Parent", RelationshipType::PARENT},   {"Parent*", RelationshipType::PARENT_STAR},
    {"Uses", RelationshipType::USES},       {"Modifies", RelationshipType::MODIFIES},
    {"Next", RelationshipType::NEXT}};  // update here

class Relationship {
public:
    virtual ~Relationship(){};
    virtual ClauseResult evaluate(PKBFacadeReader& reader) = 0;

    static RelationshipType determineRelationshipType(const std::string& type) {
        auto it = RELATIONSHIP_TYPE_MAP.find(type);
        if (it != RELATIONSHIP_TYPE_MAP.end()) {
            return it->second;
        }
        throw Exception("RelationshipType is not found in valid types: " + type);
    }
};
