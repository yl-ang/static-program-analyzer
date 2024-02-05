#include <string>
#include <vector>
#include <unordered_map>

// TODO: Settle on the type of Query and QueryResult.
typedef std::unordered_map<std::string, std::vector<std::string>> Query;
typedef std::string QueryEntity;
typedef int QueryResult;

struct Clause
{
	std::string type;
	QueryEntity firstArg;
	QueryEntity secondArg;
};

class PQLEvaluator
{
public:
	static QueryResult evaluate(Query);

private:
	static std::vector<QueryEntity> getQueryEntities(Query);
	static std::vector<Clause> getQueryClauses(Query);
	static QueryResult projectClausesOntoEntities(std::vector<QueryEntity>, std::vector<Clause>);
};