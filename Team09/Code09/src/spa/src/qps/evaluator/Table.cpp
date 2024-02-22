#include "Table.h"

#include <unordered_set>

Table::Table() {}

Table::Table(std::vector<Synonym> headers, std::vector<ColumnData> columns) : headers(headers) {
    if (headers.size() != columns.size()) {
        throw "Number of columns and headers are not equal.";
    }

    for (int i = 0; i < columns.size(); i++) {
        if (columns[i].size() != columns[0].size()) {
            throw "Number of rows in columns are not equal.";
        }
    }

    for (int rowNo = 0; rowNo < columns[0].size(); rowNo++) {
        Row row;
        for (int colNo = 0; colNo < columns.size(); colNo++) {
            SynonymValue columnHeaderValue = headers[colNo].getValue();
            RowEntry rowEntryForColumn = columns[colNo][rowNo];
            row[columnHeaderValue] = rowEntryForColumn;
        }
        rows.push_back(row);
    }
}

Table::Table(std::vector<Synonym> headers, std::vector<Row> rows) : headers(headers), rows(rows) {
    for (int i = 0; i < rows.size(); i++) {
        if (rows[i].size() != headers.size()) {
            throw "Number of columns and headers are not equal.";
        }
    }
}

/**
 * currently assumes there is only 1 entity.
 * @TODO(Ezekiel): handle multiple entities
 */
std::vector<std::string> Table::extractResults(const std::vector<Synonym>& synonyms) {
    if (isEmpty() || synonyms.size() == 0 || !containsHeader(synonyms[0])) {
        return {};
    }

    std::vector<std::string> result{};
    std::unordered_set<std::string> seen{};

    for (Row row : rows) {
        SynonymValue synonymVal = synonyms[0].getValue();
        RowEntry entry = row.at(synonymVal);
        if (seen.find(entry) == seen.end()) {
            seen.insert(entry);
            result.push_back(entry);
        }
    }

    return result;
}

// ai-gen start(copilot, 0, e)
// prompt: create cross product of two tables
Table Table::crossProduct(const Table& other) {
    std::vector<Synonym> newHeaders{headers};
    for (Synonym header : other.headers) {
        if (!containsHeader(header)) {
            newHeaders.push_back(header);
        }
    }

    std::vector<Row> newRows{};
    for (Row row : rows) {
        for (Row otherRow : other.rows) {
            Row newRow{row};
            for (Synonym header : other.headers) {
                SynonymValue headerValue = header.getValue();
                newRow[headerValue] = otherRow.at(headerValue);
            }
            newRows.push_back(newRow);
        }
    }

    return Table{newHeaders, newRows};
}
// ai-gen end

Table Table::join(const Table& other) {
    // ai-gen start(copilot, 0, e)
    // prompt: get common headers the new table should be the union of the headers
    std::vector<Synonym> commonHeaders{getCommonHeaders(other)};

    if (commonHeaders.size() == 0) {
        return crossProduct(other);
    }

    std::vector<Synonym> newHeaders{headers};
    for (Synonym header : other.headers) {
        if (!containsHeader(header)) {
            newHeaders.push_back(header);
        }
    }
    // ai-gen end

    // ai-gen start(copilot, 1, e)
    // prompt: find the rows that have the same values for the common headers
    std::vector<Row> newRows{};
    for (Row row : rows) {
        for (Row otherRow : other.rows) {
            bool isSame = true;
            for (Synonym header : commonHeaders) {
                if (row.at(header.getValue()) != otherRow.at(header.getValue())) {
                    isSame = false;
                    break;
                }
            }
            if (isSame) {
                Row newRow{row};
                for (Synonym header : other.headers) {
                    SynonymValue headerValue = header.getValue();
                    newRow[headerValue] = otherRow.at(headerValue);
                }
                newRows.push_back(newRow);
            }
        }
    }
    // ai-gen end

    return Table{newHeaders, newRows};
}

// ai-gen start(copilot, 0, e)
// prompt: using copilot
std::vector<Synonym> Table::getCommonHeaders(const Table& other) const {
    std::vector<Synonym> commonHeaders{};
    for (Synonym header : headers) {
        if (other.containsHeader(header)) {
            commonHeaders.push_back(header);
        }
    }
    return commonHeaders;
}

bool Table::containsHeader(const Synonym& qe) const {
    for (Synonym header : headers) {
        if (header == qe) {
            return true;
        }
    }
    return false;
}
// ai-gen end

bool Table::isEmpty() const {
    return rows.size() == 0;
}

int Table::getHeaderIndex(const Synonym& qe) const {
    for (int i = 0; i < headers.size(); i++) {
        if (headers[i] == qe) {
            return i;
        }
    }
    return -1;
}

std::vector<Synonym> Table::getHeaders() const {
    return headers;
}
