#include "TableManager.h"

#include <algorithm>
#include <unordered_set>

void TableManager::join(const ClauseResult& cr) const {
    this->join(clauseResultToTable(cr));
}

void TableManager::joinAll(const std::vector<Table>& tables) const {
    for (const Table& table : tables) {
        this->join(table);
    }
}

void TableManager::join(const Table& other) const {
    if (this->result.isSentinelTable() || other.isSentinelTable()) {
        return this->joinSentinelTable(other);
    }

    if (this->isEmpty() || other.isEmpty()) {
        return this->joinEmptyTable(other);
    }

    const std::vector newHeaders{mergeHeaders(other)};
    const std::vector<Synonym> commonHeaders{getCommonHeaders(other)};

    std::vector<Row> newRows{};
    for (const Row& row : this->result.getRows()) {
        for (const Row& otherRow : other.getRows()) {
            if (areJoinableRows(row, otherRow, commonHeaders)) {
                Row newRow{combineRows(row, otherRow, other.getHeaders())};
                newRows.push_back(newRow);
            }
        }
    }

    this->result = Table{newHeaders, newRows};
}

void TableManager::joinSentinelTable(const Table& other) const {
    if (this->result.isSentinelTable()) {
        this->result = other;
    }
}

void TableManager::joinEmptyTable(const Table& other) const {
    const std::vector newHeaders{mergeHeaders(other)};
    const std::vector<Row> emptyRows{};
    this->result = Table{newHeaders, emptyRows};
}

bool TableManager::areJoinableRows(const Row& row, const Row& otherRow, const std::vector<Synonym>& commonHeaders) {
    if (commonHeaders.empty()) {
        return true;
    }

    return std::all_of(commonHeaders.begin(), commonHeaders.end(), [&row, &otherRow](const Synonym& header) {
        return row.at(header.getValue()) == otherRow.at(header.getValue());
    });
}

Row TableManager::combineRows(const Row& row, const Row& otherRow, const std::vector<Synonym>& otherHeaders) {
    Row newRow{row};
    for (Synonym header : otherHeaders) {
        SynonymValue headerValue = header.getValue();
        newRow[headerValue] = otherRow.at(headerValue);
    }
    return newRow;
}

std::vector<Synonym> TableManager::getCommonHeaders(const Table& other) const {
    std::vector<Synonym> commonHeaders{};
    std::unordered_set<SynonymValue> seen{};

    for (Synonym header : this->result.getHeaders()) {
        if (seen.find(header.getValue()) == seen.end() && other.containsHeader(header)) {
            commonHeaders.push_back(header);
            seen.insert(header.getValue());
        }
    }
    return commonHeaders;
}

std::vector<Synonym> TableManager::mergeHeaders(const Table& other) const {
    std::vector<Synonym> newHeaders{};
    std::unordered_set<SynonymValue> seen{};

    for (Synonym header : this->result.getHeaders()) {
        if (seen.find(header.getValue()) == seen.end()) {
            newHeaders.push_back(header);
            seen.insert(header.getValue());
        }
    }

    for (Synonym header : other.getHeaders()) {
        if (seen.find(header.getValue()) == seen.end()) {
            newHeaders.push_back(header);
            seen.insert(header.getValue());
        }
    }
    return newHeaders;
}

Table TableManager::getTable() const {
    return this->result;
}

Table TableManager::clauseResultToTable(const ClauseResult& result) {
    if (result.isBoolean()) {
        throw Exception(CONVERTING_BOOLEAN_RESULT_TO_TABLE_EXCEPTION);
    }
    return Table{result.getSynonyms(), result.getAllSynonymValues()};
}

bool TableManager::isEmpty() const {
    return this->result.isEmpty();
}

std::vector<std::string> TableManager::extractResults(const std::vector<Synonym>& synonyms) const {
    if (this->isEmpty() || synonyms.empty() || !this->result.headersIsSupersetOf(synonyms)) {
        return {};
    }

    std::vector<std::string> results{};
    std::unordered_set<std::string> seen{};

    for (const Row& row : this->result.getRows()) {
        if (auto resultTuple{buildTuple(synonyms, row)}; seen.find(resultTuple) == seen.end()) {
            seen.insert(resultTuple);
            results.push_back(resultTuple);
        }
    }

    return results;
}

std::string TableManager::buildTuple(const std::vector<Synonym>& synonyms, const Row& row) {
    std::string resultTuple{};
    for (const Synonym& synonym : synonyms) {
        SynonymValue synonymVal = synonym.getValue();
        RowEntry entry = row.at(synonymVal);
        resultTuple += entry + " ";
    }
    resultTuple.erase(resultTuple.size() - 1);  // remove trailing whitespace
    return resultTuple;
}
