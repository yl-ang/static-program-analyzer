#include "TableManager.h"

#include <algorithm>
#include <unordered_set>

#include "qps/exceptions/evaluator/QPSTableManagerError.h"

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
    const std::vector commonHeaders{getCommonHeaders(other)};
    const std::unordered_map commonValueStringToRowMap{getCommonValueStringToRowMap(commonHeaders)};

    std::vector<Row> newRows{};
    newRows.reserve(commonValueStringToRowMap.size() * other.getRows().size());
    for (Row& row : other.getRows()) {
        std::string commonValueString{buildTuple(commonHeaders, row)};

        if (auto it = commonValueStringToRowMap.find(commonValueString); it != commonValueStringToRowMap.end()) {
            for (const Row& otherRow : it->second) {
                Row newRow{row};
                combineRows(newRow, otherRow);
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

std::unordered_map<std::string, std::vector<Row>> TableManager::getCommonValueStringToRowMap(
    const std::vector<Synonym>& commonHeaders) const {
    std::unordered_map<std::string, std::vector<Row>> commonValueStringToRowMap{};
    commonValueStringToRowMap.reserve(this->result.getRows().size());
    for (Row row : this->result.getRows()) {
        std::string commonValueString{buildTuple(commonHeaders, row)};
        commonValueStringToRowMap[commonValueString].push_back(std::move(row));
    }
    return commonValueStringToRowMap;
}

void TableManager::combineRows(Row& targetRow, const Row& sourceRow) {
    for (const auto& pair : sourceRow) {
        targetRow[pair.first] = pair.second;
    }
}

std::vector<Synonym> TableManager::getCommonHeaders(const Table& other) const {
    std::vector<Synonym> commonHeaders{};
    std::unordered_set<SynonymValue> seen{};

    for (Synonym header : this->result.getHeaders()) {
        if (seen.find(header.getValue()) == seen.end() && other.containsHeader(header)) {
            seen.insert(header.getValue());
            commonHeaders.push_back(header);
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
        throw QPSTableManagerError();
    }
    return Table{result.getSynonyms(), result.getAllSynonymValues()};
}

bool TableManager::isEmpty() const {
    return this->result.isEmpty();
}

bool TableManager::containsHeader(const Synonym& header) const {
    return this->result.containsHeader(header);
}

std::vector<std::string> TableManager::extractResults(const std::vector<Synonym>& synonyms) const {
    if (this->isEmpty() || synonyms.empty() || !this->result.headersIsSupersetOf(synonyms)) {
        return {};
    }

    std::unordered_set<std::string> uniqueTuples{};
    uniqueTuples.reserve(this->result.getRows().size());
    for (const Row& row : this->result.getRows()) {
        uniqueTuples.insert(buildTuple(synonyms, row));
    }

    std::vector<std::string> results{std::make_move_iterator(uniqueTuples.begin()),
                                     std::make_move_iterator(uniqueTuples.end())};
    return results;
}

void TableManager::projectAttributes(const std::vector<Synonym>& synonymsWithAttributes,
                                     const HeaderMatcher& headerMatcher,
                                     const ValueTransformer& attributeProjector) const {
    this->result.projectNewColumn(synonymsWithAttributes, headerMatcher, attributeProjector);
}

std::string TableManager::buildTuple(const std::vector<Synonym>& synonyms, const Row& row) {
    std::string resultTuple{};

    for (const Synonym& synonym : synonyms) {
        SynonymValue synonymVal = synonym.getValue();
        RowEntry entry = row.at(synonymVal);
        resultTuple.append(entry);
        resultTuple.append(" ");
    }

    if (resultTuple.length() > 0) {
        resultTuple.erase(resultTuple.size() - 1);  // remove trailing whitespace
    }

    return resultTuple;
}

bool TableManager::hasHeader(const Synonym& synonym) const {
    return this->result.containsHeader(synonym);
}

std::unordered_set<std::string> TableManager::getColumn(const Synonym& synonym) const {
    return this->result.getColumn(synonym);
}
