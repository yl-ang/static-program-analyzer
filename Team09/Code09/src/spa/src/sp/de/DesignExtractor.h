#pragma once

#include <memory>
#include <string>
#include <unordered_set>
#include <utility>

#include "././PKB/PKBClient/PKBFacadeWriter.h"
#include "EntityExtractor.h"
#include "FollowsExtractor.h"
#include "ModifiesExtractor.h"
#include "ParentExtractor.h"
#include "PatternExtractor.h"
#include "UsesExtractor.h"

class DesignExtractor {
public:
    DesignExtractor() {}

    void extract(std::unique_ptr<ASTNode> root);

    void writePKB(PKBFacadeWriter* pkbWriter);

    void dfsVisit(std::unique_ptr<ASTNode> node, AstVisitor* visitor);

private:
    EntityExtractor* entityExtractor;
    FollowsExtractor* followsExtractor;
    ParentExtractor* parentExtractor;
    UsesExtractor* usesExtractor;
    ModifiesExtractor* modifiesExtractor;
    PatternExtractor* patternExtractor;
};
