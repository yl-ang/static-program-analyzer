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
    bool isSemanticallyCorrect = true;

public:
    SynonymStore() = default;
    SynonymStore(std::unordered_map<std::string, DesignEntityType>);

    void storeSynonym(DesignEntityType type, const std::string& synName);
    bool isValidStore();

    bool containsSynonym(Synonym syn);
    Synonym getSynonym(const std::string& synName);
    DesignEntityType getDesignEntityType(const std::string& synName);

    bool operator==(const SynonymStore& other) const;
};
