#include <string>
#include <vector>
#include <tuple>
#include <unordered_map>
#include "QueryEntity.h"
#include "QueryClause.cpp"
#include "../ParserUtils.h"

typedef std::vector<std::string> UnparsedQuery;

enum class ClauseType {
    SELECT,
    SUCH_THAT,
    PATTERN
};

typedef std::tuple<std::vector<QueryEntity>, std::vector<QueryClause>> ParsedQuery;

class PQLParser {
public:
    static ParsedQuery parse(UnparsedQuery);
private:
    static std::string getQueryEntities(UnparsedQuery);
    static std::string getQueryClauses(UnparsedQuery);
    static std::vector<QueryEntity> parseQueryEntities(std::string);
    static std::vector<QueryClause> parseQueryClauses(std::string);
    static std::unordered_map<ClauseType, std::vector<int>> getClauseStarts(std::vector<std::string> &wordList);
    static ParsedQuery combineResult(std::vector<QueryEntity>, std::vector<QueryClause>);
};
