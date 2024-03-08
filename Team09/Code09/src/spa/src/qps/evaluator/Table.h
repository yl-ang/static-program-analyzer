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
     * Merges 2 headers. Duplicates are ignored.
     * @param firstHeaders first set of headers to merge
     * @param secondHeaders second set of headers to merge
     * @return the merged headers
     */
    static std::vector<Synonym> mergeHeaders(const std::vector<Synonym>&, const std::vector<Synonym>&);

    /**
     * Returns true if the rows are joinable.
     * They are joinable if the values of the rows are the same for their common headers.
     * @param row1 the first row
     * @param row2 the second row
     * @param headers the synonyms to compare
     * @return true if the rows are joinable
     */
    static bool areJoinableRows(const Row&, const Row&, const std::vector<Synonym>&);

    /**
     * Combines 2 rows into 1 row.
     * @param row1 the first row
     * @param row2 the second row
     * @return the combined row
     */
    static Row combineRows(const Row&, const Row&, const std::vector<Synonym>&);

    /**
     * Returns the common headers between this table and another table.
     * @param other the other table to compare with
     * @return the common headers
     */
    std::vector<Synonym> getCommonHeaders(const Table& other) const;

    /**
     * Returns the index of the header in the table.
     * @param qe the header to find
     * @return the index of the header
     */
    int getHeaderIndex(const Synonym&) const;

    /**
     * Returns true if the table contains the header.
     * @param qe the header to find
     * @return true if the table contains the header
     */
    bool containsHeader(const Synonym&) const;

    /**
     * Returns the cartesian product of this table and another table.
     * Every item in this table is paired with every item in the other table.
     * Used when joining tables with no common headers.
     * @param other the other table to cross product with
     * @return the cross product of the tables
     */
    Table cartesianProduct(const Table&) const;

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
     * Returns the results of the query.
     * @param synonyms the synonyms to extract
     * @return the results of the query
     */
    std::vector<std::string> extractResults(const std::vector<Synonym>&) const;

    /**
     * Returns the headers of the table.
     * @return the headers of the table
     */
    std::vector<Synonym> getHeaders() const;

    /**
     * Joins this table with another table on common headers, and rows
     * with the same values for the common headers.
     * @return the joined table
     */
    virtual Table join(const Table&) const;

    /**
     * Returns true if the rows of the table is empty, but it has some header.
     * @return true if the table is empty
     */
    bool isEmpty() const;
};
