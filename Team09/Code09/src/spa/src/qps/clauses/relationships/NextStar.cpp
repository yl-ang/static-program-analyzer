#include "NextStar.h"

bool NextStar::hasNextRelationship(PKBFacadeReader& reader) {
    return reader.hasNextStarRelationship(*currentStmt, *nextStmt);
}

bool NextStar::hasNextRelationship(PKBFacadeReader& reader, const StmtNum& curr, const StmtNum& next) {
    return reader.hasNextStarRelationship(curr, next);
}

StmtSet NextStar::findStars(PKBFacadeReader& reader, const StmtNum& sourceStmt, bool isGetNexter) {
    StmtSet starsSet{};
    std::stack<StmtNum> stack{};

    for (const auto& nexter : isGetNexter ? reader.getNexter(sourceStmt) : reader.getNextee(sourceStmt)) {
        stack.push(nexter);
    }

    while (!stack.empty()) {
        auto currStmt = stack.top();
        stack.pop();
        if (starsSet.find(currStmt) != starsSet.end()) {
            continue;
        }

        starsSet.insert(currStmt);
        for (const auto& nexter : isGetNexter ? reader.getNexter(currStmt) : reader.getNextee(currStmt)) {
            stack.push(nexter);
        }
    }

    return starsSet;
}

StmtSet NextStar::getNexters(PKBFacadeReader& reader, const StmtNum& nextee) {
    return findStars(reader, nextee, true);
}

StmtSet NextStar::getNextees(PKBFacadeReader& reader, const StmtNum& nexter) {
    return findStars(reader, nexter, false);
}

std::unordered_map<StmtNum, std::unordered_set<StmtNum>> NextStar::getNextStarMap(PKBFacadeReader& reader) {
    std::unordered_map<StmtNum, std::unordered_set<StmtNum>> adjList{};

    for (const auto& nextee : reader.getStmts()) {
        for (const auto& nexter : reader.getNexter(nextee.stmtNum)) {
            adjList[nextee.stmtNum].insert(nexter);
        }
    }

    std::unordered_map<StmtNum, std::unordered_set<StmtNum>> nextStarMap{};
    for (const auto& pair : adjList) {
        std::queue<StmtNum> q{};
        for (const auto& nexter : pair.second) {
            q.push(nexter);
        }

        std::unordered_set<StmtNum> nexterStars{};
        std::unordered_set<StmtNum> seen{};
        while (!q.empty()) {
            auto currNexterStar = q.front();
            q.pop();
            if (seen.find(currNexterStar) != seen.end()) {
                continue;
            }
            seen.insert(currNexterStar);

            nexterStars.insert(currNexterStar);

            // Add all its values to the queue, regardless of whether this is a valid next star.
            // We know for a fact that the current stmtnum is a next star, just that it might not be a result we
            // need. We need to add all its nexters, as they are also potential next stars of the current nextee
            for (const auto& nexter : adjList[currNexterStar]) {
                q.push(nexter);
            }
        }
        nextStarMap[pair.first] = nexterStars;
    }
    return nextStarMap;
}

ClauseResult NextStar::evaluateBothSynonyms(PKBFacadeReader& reader, EvaluationDb& evalDb) {
    Synonym currentSyn = *std::dynamic_pointer_cast<Synonym>(currentStmt);
    Synonym nextSyn = *std::dynamic_pointer_cast<Synonym>(nextStmt);

    std::vector<Row> results{};

    const std::unordered_set existingCurrentSynStmtNums{evalDb.getStmts(currentSyn)};
    const std::unordered_set existingNexterStmtNums{evalDb.getStmts(nextSyn)};

    results.reserve(existingCurrentSynStmtNums.size() * existingNexterStmtNums.size());

    for (const auto& pair : getNextStarMap(reader)) {
        for (const StmtNum& nexter : pair.second) {
            if (currentSyn == nextSyn && pair.first != nexter) {
                continue;
            }

            if (existingCurrentSynStmtNums.find(pair.first) != existingCurrentSynStmtNums.end() &&
                existingNexterStmtNums.find(nexter) != existingNexterStmtNums.end()) {
                results.push_back(Row{{currentSyn.getName(), std::to_string(pair.first)},
                                      {nextSyn.getName(), std::to_string(nexter)}});
            }
        }
    }

    std::vector<Synonym> synonyms = currentSyn == nextSyn ? std::vector{currentSyn} : std::vector{currentSyn, nextSyn};

    return {synonyms, results};
}
