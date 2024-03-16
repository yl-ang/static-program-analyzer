#pragma once

#include <string>
#include <unordered_map>

#include "qps/ParserUtils.h"
#include "qps/clauseArguments/Synonym.h"
#include "qps/exceptions/QPSSemanticError.h"

enum class DesignEntityType;
class Synonym;

class SynonymStore {
private:
    // Key: Name, Value: EntityType
    // This is because the Synonym name is suppose to be unique
    std::unordered_map<std::string, DesignEntityType> storage = {};
    bool hasSynonymName(const std::string& synName);

public:
    void storeSynonym(DesignEntityType type, const std::string& synName);

    bool containsSynonym(Synonym syn);
    Synonym getSynonym(const std::string& synName);
    DesignEntityType getDesignEntityType(const std::string& synName);
};
