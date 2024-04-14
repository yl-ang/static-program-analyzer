#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "qps/clauseArguments/Synonym.h"

using SynonymValue = std::string;
using ColumnData = std::vector<std::string>;
using RowEntry = std::string;
using Row = std::unordered_map<SynonymValue, RowEntry>;

using ValueTransformer = std::function<SynonymValue(Synonym, SynonymValue)>;
using HeaderMatcher = std::function<Synonym(std::vector<Synonym>, Synonym)>;

/**
 * Table class to store the results of a query and perform operations on the results.
 */
class Table {
private:
    /**
     * Sentinel table indicates that this is a dummy table that can be joined on.
     * It must NOT be confused with an empty table -- an empty table has headers but no rows.
     * Joining to an EMPTY TABLE will result in an empty table, while joining to a SENTINEL TABLE will result in the
     * other table.
     */
    bool isSentinel = false;
    mutable std::vector<Synonym> headers;
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
    Table(std::vector<Synonym>& headers, std::vector<Row>& rows);

    /**
     * Returns the headers of the table.
     * @return the headers of the table
     */
    std::vector<Synonym> getHeaders() const;

    void setHeaders(const std::vector<Synonym>& newHeaders) const;

    /**
     * Returns true if the rows of the table is empty, but it has some header.
     * @return true if the table is empty
     */
    bool isEmpty() const;

    /**
     * \brief Returns rows
     * \return this rows
     */
    std::vector<Row>& getRows();

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

    /**
     * \brief Transforms the values of the columns given by the synonym headers using the transformer function
     * \param synonyms The columns whose values must be transformed
     * \param transformer The transformer function
     */
    void projectNewColumn(std::vector<Synonym>, const HeaderMatcher&, const ValueTransformer&);

    std::unordered_set<SynonymValue> getColumn(const Synonym& synonym);

    bool isSentinelTable() const;
    bool operator==(const Table&) const;
};
