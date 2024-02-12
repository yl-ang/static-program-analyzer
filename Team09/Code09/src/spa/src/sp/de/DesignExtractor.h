#pragma once

#include <unordered_set>
#include <string>
#include "sp/ast/AstNode.h"
#include "././PKB/PKBClient/PKBFacadeWriter.h"

class DesignExtractor {
 public:
    DesignExtractor() {}

    void extract(ASTNode root);

    void writePKB();

    std::unordered_set<std::string> getVariables() const { return variables; }

 private:
    PKBFacadeWriter* PKBwriter;
    std::unordered_set<std::string> variables;
};
