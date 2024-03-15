#pragma once

#include <string>
#include <unordered_map>

#include "qps/ParserUtils.h"
#include "qps/exceptions/QPSSemanticError.h"

class ValidatorSynonymStore {
    std::unordered_map<std::string, std::string> storage = {};

public:
    void storeSynonymWithStatement(const std::string& declarationStatement);
    bool containsSynonym(const std::string& synName, const std::string& synType);
    bool containsSynonymName(const std::string& synName);
};
