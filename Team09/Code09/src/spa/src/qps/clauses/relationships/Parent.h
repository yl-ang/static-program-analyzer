#pragma once

#include "PKB/PKBClient/PKBFacadeReader.h"
#include "Relationship.h"
#include "baseClasses/BaseParent.h"
#include "qps/clauseArguments/ClauseArgument.h"
#include "qps/clauses/ClauseResult.h"

class Parent : public BaseParent {
public:
    Parent(ClauseArgument& parent, ClauseArgument& child) : BaseParent(parent, child) {}

private:
    bool hasParentRelationship(PKBFacadeReader& reader) override {
        return reader.hasParentRelationship(parent, child);
    }
    StmtSet getParents(PKBFacadeReader& reader, const StmtNum& child) override {
        const std::optional parentOpt = reader.getParent(child);
        return parentOpt.has_value() ? StmtSet{parentOpt.value()} : StmtSet{};
    }

    StmtSet getChildren(PKBFacadeReader& reader, const StmtNum& parent) override {
        return reader.getChildren(parent);
    }
};
