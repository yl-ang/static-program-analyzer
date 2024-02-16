#include <string>
#include <vector>
#include <tuple>
#include <unordered_map>
#include "QueryEntity.h"
#include "QueryClause.h"
#include "../evaluator/Query.h"
#include "../ParserUtils.h"

typedef std::vector<std::string> UnparsedQuery;

typedef std::tuple<std::vector<QueryEntity>, std::vector<QueryClause*>> ParsedQuery;

class PQLParser {
public:
    static Query parse(UnparsedQuery);

    static std::vector<std::string> getQueryEntities(UnparsedQuery);
    static std::string getQueryClauses(UnparsedQuery);
    static std::vector<QueryEntity> parseQueryEntities(std::vector<std::string>);
    static std::vector<QueryClause*> parseQueryClauses(std::string);
    // static std::unordered_map<ClauseType, std::vector<int>> getClauseStarts(std::vector<std::string>);
    static Query combineResult(std::vector<QueryEntity>, std::vector<QueryClause*>);
};