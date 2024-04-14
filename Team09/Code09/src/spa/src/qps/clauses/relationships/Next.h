#pragma once

#include "Relationship.h"
#include "baseClasses/BaseNext.h"
#include "pkb/PKBClient/PKBFacadeReader.h"
#include "qps/clauseArguments/ClauseArgument.h"
#include "qps/clauseArguments/Integer.h"
#include "qps/clauses/ClauseResult.h"

class Next : public BaseNext {
public:
    Next(std::shared_ptr<ClauseArgument> currentStmt, std::shared_ptr<ClauseArgument> nextStmt)
        : BaseNext(currentStmt, nextStmt) {}

private:
    bool hasNextRelationship(PKBFacadeReader& reader) override {
        return reader.hasNextRelationship(*currentStmt, *nextStmt);
    }

    bool hasNextRelationship(PKBFacadeReader& reader, const StmtNum& curr, const StmtNum& next) override {
        return reader.hasNextRelationship(curr, next);
    }

    StmtSet getNexters(PKBFacadeReader& reader, const StmtNum& nextee) override {
        return reader.getNexter(nextee);
    }

    StmtSet getNextees(PKBFacadeReader& reader, const StmtNum& nexter) override {
        return reader.getNextee(nexter);
    }

    ClauseResult evaluateBothSynonyms(PKBFacadeReader& reader, EvaluationDb& evalDb) override {
        Synonym currentSyn = *std::dynamic_pointer_cast<Synonym>(currentStmt);
        Synonym nextSyn = *std::dynamic_pointer_cast<Synonym>(nextStmt);

        std::vector<Row> results{};

        const std::unordered_set existingCurrentSynStmtNums{evalDb.getStmts(currentSyn)};
        const std::unordered_set existingNexterStmtNums{evalDb.getStmts(nextSyn)};

        results.reserve(existingCurrentSynStmtNums.size() * existingNexterStmtNums.size());

        for (const StmtNum& nextee : existingCurrentSynStmtNums) {
            for (const StmtNum& nexter : existingNexterStmtNums) {
                if (currentSyn == nextSyn && nextee != nexter) {
                    continue;
                }

                if (hasNextRelationship(reader, nextee, nexter)) {
                    results.push_back(Row{{currentSyn.getName(), std::to_string(nextee)},
                                          {nextSyn.getName(), std::to_string(nexter)}});
                }
            }
        }

        std::vector<Synonym> synonyms =
            currentSyn == nextSyn ? std::vector{currentSyn} : std::vector{currentSyn, nextSyn};

        return {synonyms, results};
    }
};
