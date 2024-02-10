#include <string>
#include <vector>
#include <unordered_map>
#include "QueryEntity.h"

typedef std::vector<std::string> UnparsedQuery;

struct QueryClause {
    std::string type;
    QueryEntity firstArg;
    QueryEntity secondArg;
};

typedef std::vector<std::vector<QueryEntity>, std::vector<QueryClause>> ParsedQuery;

class PQLParser {
public:
    static ParsedQuery parse(UnparsedQuery);
private:
    static std::string getQueryEntities(UnparsedQuery);
    static std::string getQueryClauses(UnparsedQuery);
    static std::vector<QueryEntity> parseQueryEntities(std::string);
    static std::vector<QueryClause> parseQueryClauses(std::string);
    static ParsedQuery combineResult(std::vector<QueryEntity>, std::vector<QueryClause>);
};
