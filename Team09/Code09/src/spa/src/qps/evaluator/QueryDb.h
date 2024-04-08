#pragma once

#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

#include "qps/clauses/QueryClause.h"

struct QueryDbEntry;

using ID = unsigned int;
using SynonymName = std::string;
using EvaluationQueue = std::queue<ID>;
using OptionalQueryClause = std::optional<std::shared_ptr<QueryClause>>;
using AdjacencyList = std::unordered_map<ID, std::unordered_set<ID>>;

struct QueryDbEntry {
    ID id;
    std::shared_ptr<QueryClause> clause;

    bool operator==(const QueryDbEntry& other) const {
        return clause->equals(*other.clause);
    }

    struct HashFunction {
        std::size_t operator()(const QueryDbEntry& other) const {
            std::vector<std::string> synonymNames{};
            for (Synonym syn : other.clause->getSynonyms()) {
                synonymNames.push_back(syn.getName());
            }

            // ai-gen start(0, copilot, e)
            // prompt: https://platform.openai.com/playground/p/W9SKQhcH5N9CxBLIs4m6S3Ve?mode=chat
            std::hash<std::string> hasher;
            std::size_t synonymHash = 0;
            for (const auto& name : synonymNames) {
                // hashing individual names and combining them (xor operation)
                synonymHash ^= hasher(name) + 0x9e3779b9 + (synonymHash << 6) + (synonymHash >> 2);
            }
            return synonymHash;
            // ai-gen end
        }
    };
};

using ClauseCache = std::unordered_set<QueryDbEntry, QueryDbEntry::HashFunction>;

class QueryDb {
    static ID id_index;

    std::unordered_map<ID, QueryDbEntry> idToEntryMap;
    std::unordered_map<SynonymName, std::vector<ID>> synonymToClauseListMap;
    AdjacencyList adjList;
    std::unordered_set<ID> seen;
    std::unordered_set<SynonymName> evaluatedSynonyms;
    EvaluationQueue queue;
    ClauseCache clauseCache;

    void setTraversed(ID id);
    void addNeighboursToQueue(ID id);

public:
    QueryDb(const std::vector<std::shared_ptr<QueryClause>>& clauses);

    void loadClausesWithEntities(std::vector<Synonym> synonyms);
    bool loadConnectedClauses();
    OptionalQueryClause next();
};
