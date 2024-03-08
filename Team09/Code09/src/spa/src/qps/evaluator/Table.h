#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "qps/clauseArguments/Synonym.h"
#include "qps/clauses/ClauseResult.h"

using SynonymValue = std::string;
using ColumnData = std::vector<std::string>;
using RowEntry = std::string;
using Row = std::unordered_map<SynonymValue, RowEntry>;

/**
 * Table class to store the results of a query and perform operations on the results.
 */
class Table {
private:
    /**
     * Sentinel table indicates that this is a dummy table that can be joined on.
     * It must NOT be confused with an empty table -- an empty table has headers but no rows.
     */
    bool isSentinel = false;
    std::vector<Synonym> headers;
    std::vector<Row> rows;

    /**
     * Returns the index of the header in the table.
     * @param qe the header to find
     * @return the index of the header
     */
    int getHeaderIndex(const Synonym&) const;

public:
    Table() : isSentinel{true} {}

    /**
     * Constructs a table with headers and columns.
     * @param headers the headers of the table
     * @param columns the columns of the table, data of each header
     */
    Table(std::vector<Synonym> headers, std::vector<ColumnData> columns);

    /**
     * Constructs a table with headers and rows.
     * @param headers the headers of the table
     * @param rows the rows of the table
     */
    Table(std::vector<Synonym>, std::vector<Row>);

    /**
     * Returns the headers of the table.
     * @return the headers of the table
     */
    std::vector<Synonym> getHeaders() const;

    /**
     * Returns true if the rows of the table is empty, but it has some header.
     * @return true if the table is empty
     */
    bool isEmpty() const;

    /**
     * \brief Returns rows
     * \return this rows
     */
    std::vector<Row> getRows() const;

    /**
     * \brief Synonyms are subset of headers
     * \param synonyms Set of synonyms
     * \return True if synonyms are subset of headers
     */
    bool headersIsSupersetOf(const std::vector<Synonym>& synonyms) const;

    /**
     * Returns true if the table contains the header.
     * @param qe the header to find
     * @return true if the table contains the header
     */
    bool containsHeader(const Synonym&) const;

    bool isSentinelTable() const;
    bool operator==(const Table&) const;
};
