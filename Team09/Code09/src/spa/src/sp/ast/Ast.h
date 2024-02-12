#pragma once
#include <string>
#include <vector>

#include "sp/Token.h"
#include "sp/ast/AstNode.h"

class AST {
 public:
  ASTNode buildAST(std::vector<Token> tokens);

 private:
  std::vector<std::vector<Token>> splitByProcedure(std::vector<Token> tokens);
  ASTNode buildProcedureAST(std::vector<Token> tokens);
  std::vector<std::vector<Token>> splitByStatements(std::vector<Token> tokens);
  ASTNode buildAssignmentAST(std::vector<Token> tokens);
  ASTNode buildVariableNameAST(std::vector<Token> tokens);
  ASTNode buildExpressionAST(std::vector<Token> tokens);
  ASTNode buildTermAST(std::vector<Token> tokens);
  ASTNode buildFactorAST(std::vector<Token> tokens);
  ASTNode buildConstAST(std::vector<Token> tokens);
  ASTNode buildBinaryExpressionAST(std::vector<Token> tokens,
                                   std::string ast_node_type,
                                   int operator_index);
  ASTNode buildBinaryTermAST(std::vector<Token> tokens,
                             std::string ast_node_type, int operator_index);
};
