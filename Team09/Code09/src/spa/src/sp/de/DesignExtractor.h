#pragma once

#include <unordered_set>
#include <string>
#include "./ast/ASTNode.h"
#include "././PKB/PKBClient/PKBFacadeWriter.h"

class DesignExtractor {
 public:
    DesignExtractor() {}

    void extract(ASTNode* root);

    void writePKB();

 private:
    PKBFacadeWriter* PKBwriter;
    std::unordered_set<std::string> variables;
};
