#pragma once

#include "Table.h"
#include "qps/clauses/ClauseResult.h"
#include "qps/exceptions/Exception.h"

class TableManager {
public:
    inline static std::string CONVERTING_BOOLEAN_RESULT_TO_TABLE_EXCEPTION =
        "Illegal attempt at converting boolean clause result to table.";

    TableManager() = default;
    TableManager(const Table& table) : result{table} {}

    bool isEmpty() const;
    Table getTable() const;
    void join(const Table& result) const;
    void join(const ClauseResult& result) const;
    void joinAll(const std::vector<Table>& tables) const;
    void projectAttributes(const ValueTransformer& attributeProjector) const;
    std::vector<std::string> extractResults(const std::vector<Synonym>& synonyms) const;

private:
    static Table clauseResultToTable(const ClauseResult& res);
    static std::string buildTuple(const std::vector<Synonym>& synonyms, const Row& row);
    static Row combineRows(const Row& row, const Row& otherRow, const std::vector<Synonym>& otherHeaders);
    static bool areJoinableRows(const Row& row, const Row& otherRow, const std::vector<Synonym>& commonHeaders);

    mutable Table result{};

    void joinEmptyTable(const Table& other) const;
    void joinSentinelTable(const Table& other) const;
    std::vector<Synonym> mergeHeaders(const Table& other) const;
    std::vector<Synonym> getCommonHeaders(const Table& other) const;
};
