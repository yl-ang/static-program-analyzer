#pragma once

#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

#include "qps/clauses/QueryClause.h"

using ID = unsigned int;
using SynonymName = std::string;
using EvaluationQueue = std::queue<ID>;
using OptionalQueryClause = std::optional<std::shared_ptr<QueryClause>>;
using AdjacencyList = std::unordered_map<ID, std::unordered_set<ID>>;

struct QueryDbEntry {
    ID id;
    std::shared_ptr<QueryClause> clause;
};

class QueryDb {
    static ID id_index;

    std::unordered_map<ID, QueryDbEntry> idToEntryMap;
    std::unordered_map<SynonymName, std::vector<ID>> synonymToClauseListMap;
    AdjacencyList adjList;
    std::unordered_set<ID> seen;
    std::unordered_set<SynonymName> evaluatedSynonyms;
    EvaluationQueue queue;

    void setTraversed(ID id);
    void addNeighboursToQueue(ID id);

public:
    QueryDb() = default;

    void insert(std::vector<std::shared_ptr<QueryClause>> clauses);
    void loadClausesWithEntities(std::vector<Synonym> synonyms);
    void loadRemainingClauses();
    OptionalQueryClause next();
};
