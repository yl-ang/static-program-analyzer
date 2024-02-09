#include <string>
#include <vector>
#include <unordered_map>

typedef std::vector<std::string> UnparsedQuery;
typedef std::unordered_map<std::string, std::vector<std::string>> Query;
typedef std::string QueryEntity;
struct Clause {
    std::string type;
    QueryEntity firstArg;
    QueryEntity secondArg;
};

class PQLParser {
public:
    static Query parse(UnparsedQuery);
private:
    static std::vector<QueryEntity> parseQueryEntities(UnparsedQuery);
    static std::vector<Clause> parseQueryClauses(UnparsedQuery);
    static Query combineResult(std::vector<QueryEntity>, std::vector<Clause>);
};
