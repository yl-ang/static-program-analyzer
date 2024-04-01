#pragma once

#include "PKB/PKBClient/PKBFacadeReader.h"
#include "Relationship.h"
#include "baseClasses/BaseParent.h"
#include "qps/clauseArguments/ClauseArgument.h"

class ParentStar : public BaseParent {
public:
    ParentStar(std::shared_ptr<ClauseArgument> parent, std::shared_ptr<ClauseArgument> child)
        : BaseParent(parent, child) {}

private:
    bool hasParentRelationship(PKBFacadeReader& reader) override {
        return reader.hasParentStarRelationship(*parent, *child);
    }

    StmtSet getParents(PKBFacadeReader& reader, const StmtNum& child) override {
        return reader.getParentsStar(child);
    }

    StmtSet getChildren(PKBFacadeReader& reader, const StmtNum& parent) override {
        return reader.getChildrenStar(parent);
    }
};
