#pragma once

#include "qps/clauses/relationships/Relationship.h"

using StmtSet = std::unordered_set<StmtNum>;

class BaseParent : public Relationship {
protected:
    std::shared_ptr<ClauseArgument> parent;
    std::shared_ptr<ClauseArgument> child;

private:
    bool isSimpleResult() const;

    ClauseResult evaluateSynonymWildcard(PKBFacadeReader&);
    ClauseResult evaluateSynonymInteger(PKBFacadeReader&);
    ClauseResult evaluateBothSynonyms(PKBFacadeReader&);

    virtual bool hasParentRelationship(PKBFacadeReader&) = 0;
    virtual StmtSet getParents(PKBFacadeReader&, const StmtNum&) = 0;
    virtual StmtSet getChildren(PKBFacadeReader&, const StmtNum&) = 0;

public:
    BaseParent(std::shared_ptr<ClauseArgument>, std::shared_ptr<ClauseArgument>);

    ClauseResult evaluate(PKBFacadeReader&) override;
    ClauseResult evaluate(PKBFacadeReader&, const std::shared_ptr<EvaluationDb>&) override;
    bool validateArguments() override;
};
