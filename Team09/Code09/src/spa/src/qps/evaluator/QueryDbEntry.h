#pragma once

#include <string>
#include <vector>

#include "qps/clauses/QueryClause.h"

using ID = unsigned int;

struct QueryDbEntry {
    ID id;
    std::shared_ptr<QueryClause> clause;

    bool operator==(const QueryDbEntry& other) const {
        return clause->equals(*other.clause);
    }

    bool operator<(const QueryDbEntry& other) const {
        // Change to '>' for min heap, assuming smaller values are higher priority
        return clause->getSynonyms().size() > other.clause->getSynonyms().size();
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
