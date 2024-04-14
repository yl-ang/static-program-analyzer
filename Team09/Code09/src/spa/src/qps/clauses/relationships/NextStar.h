#pragma once

#include <queue>

#include "Relationship.h"
#include "baseClasses/BaseNext.h"
#include "pkb/PKBClient/PKBFacadeReader.h"
#include "qps/clauseArguments/ClauseArgument.h"
#include "qps/clauseArguments/Integer.h"
#include "qps/clauses/ClauseResult.h"

class NextStar : public BaseNext {
public:
    NextStar(std::shared_ptr<ClauseArgument> currentStmt, std::shared_ptr<ClauseArgument> nextStmt)
        : BaseNext(currentStmt, nextStmt) {}

private:
    bool hasNextRelationship(PKBFacadeReader& reader) override {
        return reader.hasNextStarRelationship(*currentStmt, *nextStmt);
    }

    bool hasNextRelationship(PKBFacadeReader& reader, const StmtNum& curr, const StmtNum& next) override {
        return reader.hasNextStarRelationship(curr, next);
    }

    StmtSet getNexters(PKBFacadeReader& reader, const StmtNum& nextee) override {
        auto nexters = reader.getNexter(nextee);
        StmtSet nexterStars{};
        std::stack<StmtNum> stack{};

        for (const auto& nexter : nexters) {
            stack.push(nexter);
        }

        while (!stack.empty()) {
            auto currNexter = stack.top();
            stack.pop();
            if (nexterStars.find(currNexter) != nexterStars.end()) {
                continue;
            }

            nexterStars.insert(currNexter);
            for (const auto& nexter : reader.getNexter(currNexter)) {
                stack.push(nexter);
            }
        }

        return nexters;
    }

    StmtSet getNextees(PKBFacadeReader& reader, const StmtNum& nexter) override {
        auto nextees = reader.getNextee(nexter);
        StmtSet nexteeStars{};
        std::stack<StmtNum> stack{};

        for (const auto& nextee : nextees) {
            stack.push(nextee);
        }

        while (!stack.empty()) {
            auto currNextee = stack.top();
            stack.pop();
            if (nexteeStars.find(currNextee) != nexteeStars.end()) {
                continue;
            }

            nexteeStars.insert(currNextee);
            for (const auto& nexter : reader.getNexter(currNextee)) {
                stack.push(nexter);
            }
        }

        return nextees;
    }

    std::unordered_map<StmtNum, std::unordered_set<StmtNum>> getNextStarMap(PKBFacadeReader& reader,
                                                                            std::unordered_set<StmtNum> nexteeStmts,
                                                                            std::unordered_set<StmtNum> nexterStmts) {
        std::unordered_map<StmtNum, std::unordered_set<StmtNum>> adjList{};

        for (const auto& nextee : reader.getStmts()) {
            for (const auto& nexter : reader.getNexter(nextee.stmtNum)) {
                adjList[nextee.stmtNum].insert(nexter);
            }
        }

        std::unordered_map<StmtNum, std::unordered_set<StmtNum>> nextStarMap{};
        for (const auto& pair : adjList) {
            // If this stmt num is not a potential nextee stmt, we ignore it.
            if (nexteeStmts.find(pair.first) == nexteeStmts.end()) {
                continue;
            }

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

                // Only add this nexter star if it is a potential nexter stmt
                if (nexterStmts.find(currNexterStar) != nexterStmts.end()) {
                    nexterStars.insert(currNexterStar);
                }

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

    ClauseResult evaluateBothSynonyms(PKBFacadeReader& reader, EvaluationDb& evalDb) override {
        Synonym currentSyn = *std::dynamic_pointer_cast<Synonym>(currentStmt);
        Synonym nextSyn = *std::dynamic_pointer_cast<Synonym>(nextStmt);

        std::vector<Row> results{};

        const std::unordered_set existingCurrentSynStmtNums{evalDb.getStmts(currentSyn)};
        const std::unordered_set existingNexterStmtNums{evalDb.getStmts(nextSyn)};

        results.reserve(existingCurrentSynStmtNums.size() * existingNexterStmtNums.size());

        for (const auto& pair : getNextStarMap(reader, existingCurrentSynStmtNums, existingNexterStmtNums)) {
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

        std::vector<Synonym> synonyms =
            currentSyn == nextSyn ? std::vector{currentSyn} : std::vector{currentSyn, nextSyn};

        return {synonyms, results};
    }
};
