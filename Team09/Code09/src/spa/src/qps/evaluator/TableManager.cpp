#include "TableManager.h"

#include <algorithm>
#include <unordered_set>

#include "qps/exceptions/evaluator/QPSTableManagerError.h"

void TableManager::join(const ClauseResult& cr, const std::unordered_set<SynonymValue>& synonymsToRetain) const {
    auto table = clauseResultToTable(cr);
    this->join(table, synonymsToRetain);
}

void TableManager::joinAll(const std::vector<Table>& tables,
                           const std::unordered_set<SynonymValue>& synonymsToRetain) const {
    for (Table table : tables) {
        this->join(table, synonymsToRetain);
    }
}

void TableManager::join(Table& other, const std::unordered_set<SynonymValue>& synonymsToRetain) const {
    if (this->result.isSentinelTable() || other.isSentinelTable()) {
        return this->joinSentinelTable(other);
    }

    if (this->isEmpty() || other.isEmpty()) {
        return this->joinEmptyTable(other);
    }

    std::vector newHeaders{mergeHeaders(other, synonymsToRetain)};
    std::unordered_set<SynonymValue> newHeadersNames{};
    for (const auto& header : newHeaders) {
        newHeadersNames.insert(header.getName());
    }

    const std::vector commonHeaders{getCommonHeaders(other)};
    const std::unordered_map commonValueStringToRowMap{getCommonValueStringToRowMap(commonHeaders, other.getRows())};

    std::vector<Row> newRows{};
    newRows.reserve(commonValueStringToRowMap.size() * other.getRows().size());

    Row newRow{};
    for (Row& row : this->result.getRows()) {
        std::string commonValueString{buildTuple(commonHeaders, row)};
        newRow = row;

        if (auto itMap = commonValueStringToRowMap.find(commonValueString);
            // If common headers are empty, there is no inner join to do. just do a cartesian product.
            commonHeaders.empty() || itMap != commonValueStringToRowMap.end()) {
            for (const Row& otherRow : itMap->second) {
                combineRows(newHeadersNames, newRow, otherRow);
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
    std::vector newHeaders{mergeHeaders(other, {})};
    std::vector<Row> emptyRows{};
    this->result = Table{newHeaders, emptyRows};
}

std::unordered_map<std::string, std::vector<Row>> TableManager::getCommonValueStringToRowMap(
    const std::vector<Synonym>& commonHeaders, const std::vector<Row>& rows) const {
    std::unordered_map<std::string, std::vector<Row>> commonValueStringToRowMap{};
    commonValueStringToRowMap.reserve(rows.size());
    for (const Row& row : rows) {
        std::string commonValueString{buildTuple(commonHeaders, row)};
        commonValueStringToRowMap[std::move(commonValueString)].push_back(row);
    }

    return commonValueStringToRowMap;
}

void TableManager::combineRows(const std::unordered_set<SynonymValue>& headers, Row& firstRow, const Row& secondRow) {
    // delete from the original row whose headers are not in the set of wanted headers
    std::vector<SynonymValue> synonymColumnsToDelete{};
    for (const auto& pair : firstRow) {
        if (headers.find(pair.first) == headers.end()) {
            synonymColumnsToDelete.push_back(pair.first);
        }
    }

    for (const auto& synValue : synonymColumnsToDelete) {
        firstRow.erase(synValue);
    }

    // Insert into the original row from the second row whose headers are wanted
    for (const auto& pair : secondRow) {
        if (headers.find(pair.first) != headers.end()) {
            firstRow[pair.first] = pair.second;
        }
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

std::vector<Synonym> TableManager::mergeHeaders(const Table& other,
                                                const std::unordered_set<SynonymValue>& synonymsToRetain) const {
    std::vector<Synonym> newHeaders{};
    std::unordered_set<SynonymValue> seen{};

    for (Synonym header : this->result.getHeaders()) {
        const bool shouldRetain = synonymsToRetain.find(header.getName()) != synonymsToRetain.end();
        const bool isNotSeen = seen.find(header.getValue()) == seen.end();
        if (isNotSeen && shouldRetain) {
            newHeaders.push_back(header);
            seen.insert(header.getValue());
        }
    }

    for (Synonym header : other.getHeaders()) {
        const bool shouldRetain = synonymsToRetain.find(header.getName()) != synonymsToRetain.end();
        const bool isNotSeen = seen.find(header.getValue()) == seen.end();
        if (isNotSeen && shouldRetain) {
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
