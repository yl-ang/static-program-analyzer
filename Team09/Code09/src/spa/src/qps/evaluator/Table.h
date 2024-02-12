#pragma once

#include <vector>
#include <string>
#include "qps/parser/QueryEntity.h"

using Row = std::vector<std::string>;

/**
  * Table class is a data structure that stores the results of a query.
  * It is a 2D array with headers and rows.
  * The rows are stored in column-major order. Row i contains the data of the i-th header/column.
  **/
class Table {
private:
    std::vector<QueryEntity> headers;
    std::vector<std::vector<std::string>> rows;

    int getHeaderIndex(const QueryEntity&);

public:
    Table(std::vector<QueryEntity>, std::vector<Row>);
    std::vector<std::string> extractResults(const std::vector<QueryEntity>&);
    bool isEmpty() const;
};
