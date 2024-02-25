#pragma once

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

    void extract(ASTNode* root);

    void writePKB(PKBFacadeWriter* pkbWriter);

    void dfsVisit(ASTNode* node, AstVisitor* visitor);

private:
    // PKBFacadeWriter* PKBwriter;
    EntityExtractor* entityExtractor;
    FollowsExtractor* followsExtractor;
    ParentExtractor* parentExtractor;
    UsesExtractor* usesExtractor;
    ModifiesExtractor* modifiesExtractor;
    PatternExtractor* patternExtractor;
};
