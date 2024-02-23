#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "../ParserUtils.h"
#include "../clauses/QueryClause.h"
#include "../evaluator/Query.h"
#include "qps/clauseArguments/Synonym.h"

typedef std::vector<std::string> UnparsedQuery;

typedef std::tuple<std::vector<Synonym>, std::vector<QueryClause*>> ParsedQuery;

class PQLParser {
private:
    static SuchThatClause toSTClause(std::vector<Synonym>, std::string);
    static PatternClause toPatternClause(std::vector<Synonym>, std::string);

    static std::vector<std::string> searchClause(const std::regex&, const std::string&);
    static std::vector<ClauseArgument> buildSTParameters(const std::vector<Synonym>&,
                                                            const std::vector<std::string>&);
    static std::vector<ClauseArgument> buildPatternParameters(const std::vector<Synonym>&,
                                                            const std::vector<std::string>&);                                                          
    static std::vector<std::string> cleanParameters(const std::string&);
    static ClauseArgument* buildEntRef(const std::vector<Synonym>& entities,const std::string& str);
    static Synonym* buildSynonym(const std::vector<Synonym>& entities,const std::string& str);
    static Synonym buildExpression(const std::string& str);
public:
    static Query parse(UnparsedQuery);
    static std::string getQueryClauses(UnparsedQuery);
    static std::vector<Synonym> parseQueryEntities(std::vector<std::string>);
    static std::vector<Synonym> findSelectClauses(std::vector<Synonym>, std::string);
    static std::vector<SuchThatClause> findSuchThatClauses(std::vector<Synonym>, std::string);
    static std::vector<PatternClause> findPatternClauses(std::vector<Synonym>, std::string);
};
