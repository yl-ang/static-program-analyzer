#include "Table.h"

Table::Table(std::vector<QueryEntity> headers, std::vector<Row> rows) : headers(headers), rows(rows) {
    if (headers.size() != rows.size()) {
        throw "Number of columns in header and rows are not equal.";
    }
}

std::vector<std::string> Table::extractResults(const QueryEntity& qe) {
    if (isEmpty()) {
        return {};
    }

    int columnIndex{ getHeaderIndex(qe) };
    if (columnIndex == -1) {
        return {};
    }

    return rows[columnIndex];
}

bool Table::isEmpty() const {
    return rows.size() == 0;
}

int Table::getHeaderIndex(const QueryEntity& qe) {
    for (int i = 0; i < headers.size(); i++) {
        if (headers[i] == qe) {
            return i;
        }
    }
    return -1;
}
