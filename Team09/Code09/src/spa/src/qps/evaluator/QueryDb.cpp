#include "QueryDb.h"

ID QueryDb::id_index = 0;

QueryDb::QueryDb(const std::vector<std::shared_ptr<QueryClause>>& clauses) {
    std::vector<QueryDbEntry> entries{};
    entries.reserve(clauses.size());
    adjList.reserve(clauses.size());
    clauseCache.reserve(clauses.size());
    synonymToClauseListMap.reserve(clauses.size());
    idToEntryMap.reserve(clauses.size());

    for (std::shared_ptr<QueryClause> clause : clauses) {
        ID thisId = id_index++;

        QueryDbEntry newEntry{thisId, clause};

        if (clauseCache.find(newEntry) != clauseCache.end()) {
            continue;
        }
        clauseCache.insert(newEntry);
        entries.push_back(newEntry);
        idToEntryMap[thisId] = newEntry;

        for (const Synonym& synonym : clause->getSynonyms()) {
            synonymToClauseListMap[synonym.getName()].push_back(thisId);
        }
    }

    for (const QueryDbEntry& entry : entries) {
        for (Synonym syn : entry.clause->getSynonyms()) {
            for (ID id : synonymToClauseListMap[syn.getName()]) {
                adjList[entry.id].insert(id);
                adjList[id].insert(entry.id);
            }
        }
    }
}

void QueryDb::loadClausesWithEntities(std::vector<Synonym> synonyms) {
    for (const Synonym& syn : synonyms) {
        for (ID id : synonymToClauseListMap[syn.getName()]) {
            queue.push(id);
        }
    }
}

bool QueryDb::loadNewGroup() {
    bool loadSuccessful = false;
    for (const auto& pair : synonymToClauseListMap) {
        if (evaluatedSynonyms.find(pair.first) != evaluatedSynonyms.end() || pair.second.empty()) {
            // already evaluated
            continue;
        }

        for (ID id : pair.second) {
            queue.push(id);
        }
        loadSuccessful = true;
        break;
    }
    return loadSuccessful;
}

OptionalQueryClause QueryDb::next() {
    if (queue.empty()) {
        return std::nullopt;
    }

    ID nextEntryId{};
    do {
        nextEntryId = queue.front();
        queue.pop();
    } while (seen.find(nextEntryId) != seen.end() && !queue.empty());

    if (seen.find(nextEntryId) != seen.end() && queue.empty()) {
        return std::nullopt;
    }

    setTraversed(nextEntryId);
    addNeighboursToQueue(nextEntryId);
    return idToEntryMap[nextEntryId].clause;
}

void QueryDb::setTraversed(ID id) {
    seen.insert(id);
    for (Synonym syn : idToEntryMap[id].clause->getSynonyms()) {
        evaluatedSynonyms.insert(syn.getName());
    }
}

void QueryDb::addNeighboursToQueue(ID id) {
    for (ID connectedId : adjList[id]) {
        queue.push(connectedId);
    }
}
