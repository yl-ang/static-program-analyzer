#pragma once

#include <string>
#include <unordered_set>

#include "././PKB/PKBClient/PKBFacadeWriter.h"
#include "sp/ast/AstNode.h"

class DesignExtractor {
 public:
  DesignExtractor() {}

  void extract(ASTNode root);

  void writePKB(PKBFacadeWriter);

  std::unordered_set<std::string> getVariables() const { return variables; }

 private:
  //  PKBFacadeWriter* PKBwriter;
  std::unordered_set<std::string> variables;
};
