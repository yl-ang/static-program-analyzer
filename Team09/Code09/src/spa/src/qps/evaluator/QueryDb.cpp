#include "QueryDb.h"

#include <iostream>

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
            synonymsEvaluationCount[synonym.getName()] += 1;
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

void QueryDb::loadClausesWithEntity(SynonymValue synName) {
    std::unordered_set<ID> idsToLoad{};
    std::stack<ID> toTraverse{};
    for (ID id : synonymToClauseListMap[synName]) {
        toTraverse.push(id);
    }

    while (!toTraverse.empty()) {
        auto currId = toTraverse.top();
        toTraverse.pop();
        if (seen.find(currId) != seen.end()) {
            continue;
        }

        seen.insert(currId);
        idsToLoad.insert(currId);
        for (auto neighbour : adjList[currId]) {
            toTraverse.push(neighbour);
        }
        adjList[currId].clear();
    }

    for (ID id : idsToLoad) {
        queue.push(idToEntryMap[id]);
    }
}

bool QueryDb::loadNextGroup() {
    for (const auto& pair : synonymsEvaluationCount) {
        if (pair.second <= 0) {
            continue;
        }

        loadClausesWithEntity(pair.first);
        if (!queue.empty()) {
            return true;
        }
    }
    return false;
}

OptionalQueryClause QueryDb::next() {
    if (queue.empty()) {
        return std::nullopt;
    }

    auto nextEntry = queue.front();
    queue.pop();

    std::cout << "New clause being nexted" << std::endl;
    for (const auto& syn : nextEntry.clause->getSynonyms()) {
        std::cout << "header: " << syn.getName() << std::endl;
        synonymsEvaluationCount[syn.getName()] -= 1;
    }

    return nextEntry.clause;
}

std::unordered_map<SynonymValue, int> QueryDb::getSynonymsEvaluationCount() {
    return this->synonymsEvaluationCount;
}
