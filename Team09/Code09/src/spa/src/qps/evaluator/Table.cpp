#include "Table.h"

#include <algorithm>

#include "qps/clauses/ClauseResult.h"
#include "qps/exceptions/evaluator/QPSUnequalColHeaderError.h"
#include "qps/exceptions/evaluator/QPSUnequalRowColError.h"

Table::Table(std::vector<Synonym> headers, std::vector<ColumnData> columns) : headers(headers) {
    if (columns.empty()) {
        return;
    }

    if (headers.size() != columns.size()) {
        throw QPSUnequalColHeaderError();
    }

    for (size_t i = 0; i < columns.size(); i++) {
        if (columns[i].size() != columns[0].size()) {
            throw QPSUnequalRowColError();
        }
    }

    for (size_t rowNo = 0; rowNo < columns[0].size(); rowNo++) {
        Row row;
        for (size_t colNo = 0; colNo < columns.size(); colNo++) {
            SynonymValue columnHeaderValue = headers[colNo].getValue();
            RowEntry rowEntryForColumn = columns[colNo][rowNo];
            row[columnHeaderValue] = rowEntryForColumn;
        }
        rows.push_back(row);
    }
}

Table::Table(std::vector<Synonym>& headers, std::vector<Row>& rows)
    : headers(std::move(headers)), rows(std::move(rows)) {
    /*
    for (const Row& row : rows) {
        if (row.size() != headers.size()) {
            throw QPSUnequalColHeaderError();
        }
    }
    */
}

bool Table::containsHeader(const Synonym& qe) const {
    return std::any_of(headers.begin(), headers.end(), [&qe](const Synonym& header) { return header == qe; });
}

bool Table::headersIsSupersetOf(const std::vector<Synonym>& synonyms) const {
    return std::all_of(synonyms.begin(), synonyms.end(),
                       [this](const Synonym& synonym) { return containsHeader(synonym); });
}

bool Table::isEmpty() const {
    return !headers.empty() && rows.empty();
}

bool Table::isSentinelTable() const {
    return isSentinel;
}

int Table::getHeaderIndex(const Synonym& qe) const {
    for (int i = 0; i < headers.size(); i++) {
        if (headers[i] == qe) {
            return i;
        }
    }
    return -1;
}

std::vector<Row>& Table::getRows() {
    return rows;
}

std::vector<Synonym> Table::getHeaders() const {
    return headers;
}

void Table::setHeaders(const std::vector<Synonym>& newHeaders) const {
    this->headers = newHeaders;
}

void Table::projectNewColumn(std::vector<Synonym> newHeaders, const HeaderMatcher& matchTargetHeader,
                             const ValueTransformer& valueTransform) {
    for (const Synonym& newHeader : newHeaders) {
        this->headers.push_back(newHeader);

        Synonym projectionTargetHeader = matchTargetHeader(headers, newHeader);
        for (size_t i = 0; i < rows.size(); i++) {
            const SynonymValue valueToTransform = rows[i][projectionTargetHeader.getValue()];
            const SynonymValue valueToInsert{valueTransform(newHeader, valueToTransform)};
            rows[i][newHeader.getValue()] = valueToInsert;
        }
    }
}

bool Table::operator==(const Table& other) const {
    if (this->headers.size() != other.headers.size() || this->rows.size() != other.rows.size()) {
        return false;
    }

    // Since we established header size must be the same, then this is checking equality of headers.
    if (!headersIsSupersetOf(other.getHeaders())) {
        return false;
    }

    // For each row in this table, build its string representation and store it in a set
    std::unordered_set<std::string> rowStrings{};
    for (const Row& row : rows) {
        std::string rowString{};
        for (const Synonym& header : headers) {
            rowString += row.at(header.getValue()) + ",";
        }
        rowStrings.insert(rowString);
    }

    // For each row in the other table, build its string representation and check if it exists in the set
    for (const Row& row : other.rows) {
        std::string rowString{};
        // We use this' headers because we want to ensure the order of the columns is the same.
        for (const Synonym& header : headers) {
            rowString += row.at(header.getValue()) + ",";
        }

        if (auto it = rowStrings.find(rowString); it != rowStrings.end()) {
            rowStrings.erase(it);
        } else {
            return false;
        }
    }

    return true;
}

std::unordered_set<SynonymValue> Table::getColumn(const Synonym& synonym) {
    std::unordered_set<SynonymValue> result{};

    for (const Row& row : getRows()) {
        result.insert(row.at(synonym.getValue()));
    }

    return result;
}
