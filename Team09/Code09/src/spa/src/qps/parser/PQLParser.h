#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "../ParserUtils.h"
#include "../clauses/QueryClause.h"
#include "../evaluator/Query.h"
#include "QueryEntity.h"

typedef std::vector<std::string> UnparsedQuery;

typedef std::tuple<std::vector<QueryEntity>, std::vector<QueryClause*>> ParsedQuery;

class PQLParser {
private:
    static SuchThatClause PQLParser::toSTClause(std::vector<QueryEntity>, std::string);
    static PatternClause PQLParser::toPatternClause(std::vector<QueryEntity>, std::string);
public:
    static Query parse(UnparsedQuery);

    // static std::vector<std::string> getQueryEntities(UnparsedQuery);
    static std::string getQueryClauses(UnparsedQuery);
    static std::vector<QueryEntity> parseQueryEntities(std::vector<std::string>);
    // static std::vector<QueryClause*> parseQueryClauses(std::string);
    static std::vector<SuchThatClause> findSuchThatClauses(std::vector<QueryEntity>, std::string);
    static std::vector<PatternClause> findPatternClauses(std::vector<QueryEntity>, std::string);
    // static std::unordered_map<ClauseType, std::vector<int>>
    // getClauseStarts(std::vector<std::string>);
    // static Query combineResult(const std::vector<QueryEntity>,
    //                            const std::vector<SuchThatClause>&,
    //                            const std::vector<PatternClause>&);
};
