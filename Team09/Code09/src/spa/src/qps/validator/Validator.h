#include <string>
#include <unordered_map>
#include <vector>

#include "../GrammarUtils.h"
#include "../ParserUtils.h"
#include "../exceptions/QPSSemanticError.h"
#include "../exceptions/QPSSyntaxError.h"

class VariableStore {
    std::unordered_map<std::string, std::string> storage = {};

public:
    void storeSynonym(const std::string& synName, const std::string& synType);
    bool containsSynonym(const std::string& synName, const std::string& synType);
    bool containsSynonymName(const std::string& synName);
};

class Validator {
public:
    void validate(std::vector<std::string>);

private:
    // tuple<variable_type, variable_name>
    VariableStore variableStore = {};

    bool isValidSelectStatement(std::string);
    bool isValidDeclarationStatement(std::string);

    bool isValidSuchThatClause(std::string);
    bool isValidPatternClause(std::string);
};
