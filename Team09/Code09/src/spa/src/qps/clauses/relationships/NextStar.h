#pragma once

#include <queue>

#include "PKB/PKBClient/PKBFacadeReader.h"
#include "Relationship.h"
#include "baseClasses/BaseNext.h"
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
        return reader.getNexterStar(nextee);
    }

    StmtSet getNextees(PKBFacadeReader& reader, const StmtNum& nexter) override {
        return reader.getNexteeStar(nexter);
    }

    ClauseResult BaseNext::evaluateBothSynonyms(PKBFacadeReader& reader,
                                                const std::shared_ptr<EvaluationDb>& evalDb) override {
        Synonym currentSyn = *std::dynamic_pointer_cast<Synonym>(currentStmt);
        Synonym nextSyn = *std::dynamic_pointer_cast<Synonym>(nextStmt);

        std::vector<Synonym> synonyms{currentSyn, nextSyn};

        SynonymValues currentSynValues{}, nextSynValues{};

        const std::unordered_set existingCurrentSynStmtNums{evalDb->getStmts(currentSyn)};
        const std::unordered_set existingNexterStmtNums{evalDb->getStmts(nextSyn)};

        currentSynValues.reserve(existingCurrentSynStmtNums.size() * existingNexterStmtNums.size());
        nextSynValues.reserve(existingCurrentSynStmtNums.size() * existingNexterStmtNums.size());

        for (const auto& pair : reader.getNextStarMap()) {
            for (const StmtNum& nexter : pair.second) {
                if (currentSyn == nextSyn && pair.first != nexter) {
                    continue;
                }

                if (existingCurrentSynStmtNums.find(pair.first) != existingCurrentSynStmtNums.end() &&
                    existingNexterStmtNums.find(nexter) != existingNexterStmtNums.end()) {
                    currentSynValues.push_back(std::to_string(pair.first));
                    nextSynValues.push_back(std::to_string(nexter));
                }
            }
        }

        std::vector<SynonymValues> synonymValues{currentSynValues, nextSynValues};
        return {synonyms, synonymValues};
    }
};
