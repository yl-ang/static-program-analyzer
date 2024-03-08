#include "Table.h"
#include "qps/clauses/ClauseResult.h"
#include "qps/exceptions/Exception.h"

class TableManager {
public:
    inline static std::string CONVERTING_BOOLEAN_RESULT_TO_TABLE_EXCEPTION =
        "Illegal attempt at converting boolean clause result to table.";

    TableManager() = default;

    TableManager(const Table& table) : result{table} {}

    void join(const ClauseResult& result) const;
    void join(const Table& result) const;
    void joinAll(const std::vector<Table>& tables) const;
    bool isEmpty() const;
    Table getTable() const;
    std::vector<std::string> extractResults(const std::vector<Synonym>& synonyms) const;

private:
    mutable Table result{};
    Table clauseResultToTable(const ClauseResult& res) const;
};
