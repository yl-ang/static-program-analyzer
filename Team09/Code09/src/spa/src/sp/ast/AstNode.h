#pragma once

#include <string>
#include <vector>

#include "Token.h"

class ASTNode {
  std::vector<ASTNode> children;
  std::string value;
  std::string type;

 public:
  explicit ASTNode(std::string value, std::string type,
                   std::vector<ASTNode> children = {})
      : value(value), type(type), children(children) {}

    void add_child(ASTNode child) { children.push_back(child); }
};
