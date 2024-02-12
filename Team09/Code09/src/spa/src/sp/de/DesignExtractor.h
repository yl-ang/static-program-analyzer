#pragma once

#include <unordered_set>
#include <string>
#include "./ast/ASTNode.h"
#include "././PKB/PKBClient/PKBFacadeWriter.h"

class DesignExtractor {
 public:
    DesignExtractor() {}

    std::unordered_set<ASTNode> extract(ASTNode* root);

    void writePKB(const std::unordered_set<ASTNode>& nodes);

 private:
    PKBFacadeWriter* PKBwriter;
    std::unordered_set<std::string> variables;
};
