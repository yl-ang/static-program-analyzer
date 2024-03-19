#include "qps/parser/PQLParser.h"

class PQLParserTester: public PQLParser {
public:
    static std::string getQueryClauses(UnparsedQueries);
    static SynonymStore parseQueryEntities(std::vector<std::string>);
    static std::tuple<std::shared_ptr<SelectEntContainer>,std::vector<SuchThatClause>,
        std::vector<PatternClause>> parseClauses(const std::vector<std::string>&);
    static std::shared_ptr<SelectEntContainer> parseSelectClause(std::string);
    static SuchThatClause parseSuchThatClauses(std::string);
    static PatternClause parsePatternClauses(std::string);
    static void modifyClauseList(std::vector<std::string>&);
};