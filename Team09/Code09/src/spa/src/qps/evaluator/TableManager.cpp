#include "TableManager.h"

#include <unordered_set>

void TableManager::join(const ClauseResult& result) const {
    this->join(clauseResultToTable(result));
}

void TableManager::join(const Table& newTable) const {
    this->result = this->result.join(newTable);
}

void TableManager::joinAll(const std::vector<Table>& tables) const {
    for (const Table& table : tables) {
        this->join(table);
    }
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
        std::string resultTuple{};
        for (const Synonym& synonym : synonyms) {
            SynonymValue synonymVal = synonym.getValue();
            RowEntry entry = row.at(synonymVal);
            resultTuple += entry + " ";
        }
        resultTuple.erase(resultTuple.size() - 1);  // remove trailing whitespace

        if (seen.find(resultTuple) == seen.end()) {
            seen.insert(resultTuple);
            results.push_back(resultTuple);
        }
    }

    return results;
}
