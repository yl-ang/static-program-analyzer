#include "DesignExtractor.h"

void DesignExtractor::extract(ASTNode root) {
  // DFS
  for (ASTNode child : root.getChildren()) {
    extract(child);
  }

  // Check if the current node is of type "var"
  if (root.getType() == "var") {
    variables.insert(root.getValue());
  }
}

void DesignExtractor::writePKB(PKBFacadeWriter writer) {
  writer.setVariables(variables);
}
