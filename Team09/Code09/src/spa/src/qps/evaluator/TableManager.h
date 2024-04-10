#pragma once

#include "Table.h"
#include "qps/clauses/ClauseResult.h"
#include "qps/exceptions/Exception.h"

class TableManager {
public:
    inline static std::string CONVERTING_BOOLEAN_RESULT_TO_TABLE_EXCEPTION =
        "Illegal attempt at converting boolean clause result to table.";

    TableManager() = default;
    TableManager(Table table) : result{std::move(table)} {}

    bool isEmpty() const;
    bool containsHeader(const Synonym& header) const;
    Table getTable() const;
    void join(const Table& result) const;
    void join(const ClauseResult& result) const;
    void joinAll(const std::vector<Table>& tables) const;
    void projectAttributes(const std::vector<Synonym>&, const HeaderMatcher&, const ValueTransformer&) const;
    std::vector<std::string> extractResults(const std::vector<Synonym>& synonyms) const;

private:
    static Table clauseResultToTable(const ClauseResult& res);
    static std::string buildTuple(const std::vector<Synonym>& synonyms, const Row& row);
    static void combineRows(Row&, const Row&);

    mutable Table result{};

    void joinEmptyTable(const Table& other) const;
    void joinSentinelTable(const Table& other) const;
    std::vector<Synonym> mergeHeaders(const Table& other) const;
    std::vector<Synonym> getCommonHeaders(const Table& other) const;
    std::unordered_map<std::string, std::vector<Row>> getCommonValueStringToRowMap(
        const std::vector<Synonym>& commonHeaders) const;
};
