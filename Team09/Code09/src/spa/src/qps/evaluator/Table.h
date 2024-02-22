#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "qps/clauseArguments/Synonym.h"

using SynonymValue = std::string;
using ColumnData = std::vector<std::string>;
using RowEntry = std::string;
using Row = std::unordered_map<SynonymValue, RowEntry>;

/**
 * Table class to store the results of a query and perform operations on the results.
 */
class Table {
private:
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
     * Returns the cross product of this table and another table.
     * Used when joining tables with no common headers.
     * @param other the other table to cross product with
     * @return the cross product of the tables
     */
    Table crossProduct(const Table&);

public:
    Table();

    /**
     * Constructs a table with headers and columns.
     * @param headers the headers of the table
     * @param columns the columns of the table, data of each header
     */
    Table(std::vector<Synonym>, std::vector<ColumnData>);

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
    std::vector<std::string> extractResults(const std::vector<Synonym>&);

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
    Table join(const Table&);

    /**
     * Returns true if the rows of the table is empty.
     * @return true if the table is empty
     */
    bool isEmpty() const;
};
