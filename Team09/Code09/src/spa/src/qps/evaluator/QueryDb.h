#pragma once

#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

#include "QueryDbEntry.h"
#include "qps/clauses/QueryClause.h"

struct QueryDbEntry;

using SynonymName = std::string;
using EvaluationQueue = std::queue<QueryDbEntry>;
using OptionalQueryClause = std::optional<std::shared_ptr<QueryClause>>;
using AdjacencyList = std::unordered_map<ID, std::unordered_set<ID>>;
using ClauseCache = std::unordered_set<QueryDbEntry, QueryDbEntry::HashFunction>;

class QueryDb {
    std::unordered_map<ID, QueryDbEntry> idToEntryMap;
    std::unordered_map<SynonymName, std::vector<ID>> synonymToClauseListMap;
    AdjacencyList adjList;
    std::unordered_set<ID> seen;
    std::unordered_set<SynonymName> synonymsToEvaluate;
    EvaluationQueue queue;

public:
    QueryDb(const std::vector<std::shared_ptr<QueryClause>>& clauses);

    void loadClausesWithEntity(SynonymName syn);
    bool loadNextGroup();
    OptionalQueryClause next();
};
