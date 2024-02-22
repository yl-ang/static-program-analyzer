#pragma once

#include <string>
#include <vector>

#include "qps/clauseArguments/Synonym.h"

using Row = std::vector<std::string>;

/**
 * Table class is a data structure that stores the results of a query.
 * It is a 2D array with headers and rows.
 * The rows are stored in column-major order. Row i contains the data of the
 *i-th header/column.
 **/
class Table {
private:
    std::vector<Synonym> headers;
    std::vector<std::vector<std::string>> rows;

    int getHeaderIndex(const Synonym&);

public:
    Table();
    Table(std::vector<Synonym>, std::vector<Row>);
    std::vector<std::string> extractResults(const std::vector<Synonym>&);
    bool isEmpty() const;
};
