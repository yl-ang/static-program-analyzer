#pragma once

#include <string>
#include <unordered_set>

#include "PKB/PKB.h"

class PKBFacadeWriter {
 public:
  explicit PKBFacadeWriter(PKB &pkb);
  PKBFacadeWriter() = default;

  // Setters for common entities
  void setVariables(const std::unordered_set<Variable> &inputVariables) const;
  void setConstants(const std::unordered_set<Constant> &inputConstants) const;
  void setProcedures(
      const std::unordered_set<Procedure> &inputProcedures) const;

  // Setter for statement entity
  void setStmts(const std::unordered_set<Stmt> &inputStmts) const;

  // TODO: Setter for Design Abstractions

 private:
  PKB *pkbReference = nullptr;
};
