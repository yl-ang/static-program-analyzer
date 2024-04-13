#include "QueryDb.h"

QueryDb::QueryDb(const std::vector<std::shared_ptr<QueryClause>>& clauses) {
    adjList.reserve(clauses.size());
    synonymToClauseListMap.reserve(clauses.size());
    idToEntryMap.reserve(clauses.size());

    ID idIndex = 0;

    ClauseCache entries{};
    entries.reserve(clauses.size());
    for (const std::shared_ptr<QueryClause>& clause : clauses) {
        ID thisId = idIndex++;

        QueryDbEntry newEntry{thisId, clause};

        if (entries.find(newEntry) != entries.end()) {
            continue;
        }
        entries.insert(newEntry);
        idToEntryMap[thisId] = newEntry;

        for (const Synonym& synonym : clause->getSynonyms()) {
            synonymToClauseListMap[synonym.getName()].push_back(thisId);
        }
    }

    for (const QueryDbEntry& entry : entries) {
        for (Synonym syn : entry.clause->getSynonyms()) {
            synonymsToEvaluate.insert(syn.getName());

            for (ID id : synonymToClauseListMap[syn.getName()]) {
                adjList[entry.id].insert(id);
                adjList[id].insert(entry.id);
            }
        }
    }
}

void QueryDb::loadClausesWithEntity(Synonym syn) {
    for (ID id : synonymToClauseListMap[syn.getName()]) {
        queue.push(idToEntryMap[id]);
    }
}

bool QueryDb::loadNextGroup() {
    for (const SynonymName& synName : synonymsToEvaluate) {
        auto clauseIds = synonymToClauseListMap[synName];
        if (clauseIds.empty()) {
            continue;
        }

        for (ID id : clauseIds) {
            queue.push(idToEntryMap[id]);
        }
        return true;
    }
    return false;
}

OptionalQueryClause QueryDb::next() {
    if (queue.empty()) {
        return std::nullopt;
    }

    QueryDbEntry nextEntry{};
    do {
        nextEntry = queue.top();
        queue.pop();
    } while (seen.find(nextEntry.id) != seen.end() && !queue.empty());

    if (seen.find(nextEntry.id) != seen.end() && queue.empty()) {
        return std::nullopt;
    }

    setTraversed(nextEntry.id);
    addNeighboursToQueue(nextEntry.id);
    return nextEntry.clause;
}

void QueryDb::setTraversed(ID id) {
    seen.insert(id);
    for (Synonym syn : idToEntryMap[id].clause->getSynonyms()) {
        synonymsToEvaluate.erase(syn.getName());
    }
}

void QueryDb::addNeighboursToQueue(ID id) {
    for (ID connectedId : adjList[id]) {
        queue.push(idToEntryMap[connectedId]);
    }
    // Prevent adding clauses multiple times into the evaluate queue.
    adjList[id].clear();
}
