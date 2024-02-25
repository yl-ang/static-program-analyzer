#pragma once

#include <string>
#include <unordered_set>

#include "././PKB/PKBClient/PKBFacadeWriter.h"
#include "EntityExtractor.h"
#include "FollowsExtractor.h"
#include "ModifiesExtractor.h"
#include "ParentExtractor.h"
#include "UsesExtractor.h"

class DesignExtractor {
public:
    DesignExtractor() {}

    void extract(ASTNode root);

    void writePKB(PKBFacadeWriter);

    std::unordered_set<std::string> getVariables() const {
        return variables;
    }

private:
    PKBFacadeWriter* PKBwriter;
    EntityExtractor* entityExtractor;
    FollowsExtractor* followsExtractor;
    ParentExtractor* parentExtractor;
    UsesExtractor* usesExtractor;
    ModifiesExtractor* modifiesExtractor;

    std::unordered_set<std::string> variables;
};
