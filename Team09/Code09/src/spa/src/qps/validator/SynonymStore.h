#include <string>
#include <unordered_map>

#include "../exceptions/QPSSemanticError.h"

class SynonymStore {
    std::unordered_map<std::string, std::string> storage = {};

public:
    void storeSynonym(const std::string& synName, const std::string& synType);
    bool containsSynonym(const std::string& synName, const std::string& synType);
    bool containsSynonymName(const std::string& synName);
};
