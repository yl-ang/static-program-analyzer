#include "TableManager.h"

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

Table TableManager::clauseResultToTable(const ClauseResult& result) const {
    if (result.isBoolean()) {
        throw Exception(CONVERTING_BOOLEAN_RESULT_TO_TABLE_EXCEPTION);
    }
    return Table{result.getSynonyms(), result.getAllSynonymValues()};
}

bool TableManager::isEmpty() const {
    return this->result.isEmpty();
}

std::vector<std::string> TableManager::extractResults(const std::vector<Synonym>& synonyms) const {
    return this->result.extractResults(synonyms);
}
