#include <string>
#include <vector>
#include <unordered_map>

typedef std::vector<std::string> UnparsedQuery;
typedef std::string QueryEntity;

struct QueryDeclaration {
    std::string type;
    QueryEntity arg;
};

struct QueryClause {
    std::string type;
    QueryEntity firstArg;
    QueryEntity secondArg;
};

typedef std::vector<std::vector<QueryDeclaration>, std::vector<QueryClause>> ParsedQuery;

class PQLParser {
public:
    static ParsedQuery parse(UnparsedQuery);
private:
    static std::string getQueryDeclarations(UnparsedQuery);
    static std::string getQueryClauses(UnparsedQuery);
    static std::vector<QueryDeclaration> parseQueryDeclarations(std::string);
    static std::vector<QueryClause> parseQueryClauses(std::string);
    static ParsedQuery combineResult(std::vector<QueryDeclaration>, std::vector<QueryClause>);
};
