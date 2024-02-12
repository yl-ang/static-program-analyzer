#include <sp/ast/Ast.h>
#include <sp/ast/AstNode.h>

#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>

/*
root node is fixed so we create that at the top first before traversing the
token vector. now we process the procedures each procedure.  procedure
<identifier> { stmtLIst+ } So the first token should be the procedure keyword


Assumptions:
- First token is always procedure
- Program is syntatically correct
*/
ASTNode AST::buildAST(std::vector<Token> tokens) {
  ASTNode root = ASTNode("main", "program");
  std::vector<std::vector<Token>> procedures = splitByProcedure(tokens);
  for (auto procedure : procedures) {
    ASTNode procedureAST = buildProcedureAST(procedure);
    root.add_child(procedureAST);
  }

  return root;
}

/*
Finds a procedure by comparing closing curly braces.
The algorithm uses a stack to do so. Open curly braces are always pushed onto
the stack. If there are closes curly braces, we pop from the stack if the last
element is an open curly brace. Once the stack is empty, we are done with the
currnt procedure. So we add it into our resulting array.
*/
std::vector<std::vector<Token>> AST::splitByProcedure(
    std::vector<Token> tokens) {
  std::vector<std::vector<Token>> procedures = {};
  std::vector<Token> current_procedure = {};
  std::stack<Token> stack = {};
  for (auto token : tokens) {
    if (token.type == LEXICAL_TOKEN_TYPE::OPEN_CURLY_BRACE) {
      stack.push(token);

    } else if (token.type == LEXICAL_TOKEN_TYPE::CLOSE_CURLY_BRACE &&
               stack.top().type == LEXICAL_TOKEN_TYPE::OPEN_CURLY_BRACE) {
      stack.pop();
    }
    current_procedure.push_back(token);
    // if the current token we pushed is a proc type, or the prev token before
    // this one is, we dont pop from the stack
    // TODO(ben): handle nested procedures
    if (token.type == PROC || current_procedure.end()[-2].type == PROC) {
      continue;
    }
    // if the stack is empty, means that we have reached the end of the current
    // procedure. so we add the current procedure to the procedures vect and
    // start again.
    if (stack.empty()) {
      procedures.push_back(current_procedure);
      current_procedure = {};
    }
  }

  return procedures;
}

/*
Takes in some tokens from a given procedure. Get all the statements by comparing
on semicolon (this is only correct for some statements)
TODO: split by other types of statements
*/
std::vector<std::vector<Token>> AST::splitByStatements(
    std::vector<Token> tokens) {
  // need to remove the first 3 tokens (procedure XXXX {), and the last
  // TODO(ben): find a better way to do this
  std::vector<Token> statementTokens =
      std::vector<Token>(tokens.begin() + 3, tokens.end() - 1);
  std::vector<std::vector<Token>> statements = {};
  std::vector<Token> current_statement = {};
  for (auto token : statementTokens) {
    if (token.type == LEXICAL_TOKEN_TYPE::SEMICOLON) {
      statements.push_back(current_statement);
      current_statement = {};
    } else {
      current_statement.push_back(token);
    }
  }

  return statements;
}

/*
Since procedures are made of statements, we need to find all the statements
first. Then we build the statement ast for each statement and add it to the
statement list. Currently we assume that every statement is an assignment.
*/
ASTNode AST::buildProcedureAST(std::vector<Token> tokens) {
  ASTNode root = ASTNode("", "stmtList");
  std::vector<std::vector<Token>> statements = splitByStatements(tokens);
  for (auto statement : statements) {
    ASTNode statementAST = buildAssignmentAST(statement);
    root.add_child(statementAST);
  }
  return root;
}

/*
Once we reach the equal sign, we build the LHS AST. Then we continue using the
same vector for the RHS. At the end, we build RHS and then we add them as
children. Grammar for assignment is: var_name '=' expr ';'

So before the equal, we build the var name ast
*/
ASTNode AST::buildAssignmentAST(std::vector<Token> tokens) {
  ASTNode root = ASTNode("", "assign");
  std::vector<Token> token_vector = {};
  for (auto token : tokens) {
    if (token.type == LEXICAL_TOKEN_TYPE::EQUAL) {
      ASTNode identifierAST = buildVariableNameAST(token_vector);
      root.add_child(identifierAST);
      token_vector = {};
    } else {
      token_vector.push_back(token);
    }
  }
  ASTNode expressionAST = buildExpressionAST(token_vector);
  root.add_child(expressionAST);
  return root;
}

/*
Variable name AST are trivial. Assume that the length of the vector here is 1
for now.
*/
ASTNode AST::buildVariableNameAST(std::vector<Token> tokens) {
  Token variable_name_token = tokens[0];
  return ASTNode(variable_name_token.value, "var");
}

/*
attempt to find a '+' or '-' otherwise, we know that its just a term. If its
just a term, we build the term ast instead.
*/
ASTNode AST::buildExpressionAST(std::vector<Token> tokens) {
  for (int i = 0; i < tokens.size(); i++) {
    Token current_token = tokens[i];
    if (current_token.type == ADD) {
      return buildBinaryExpressionAST(tokens, "add", i);
    } else if (current_token.type == SUB) {
      return buildBinaryExpressionAST(tokens, "sub", i);
    }
  }

  return buildTermAST(tokens);
}

ASTNode AST::buildBinaryExpressionAST(std::vector<Token> tokens,
                                      std::string ast_node_type,
                                      int operator_index) {
  ASTNode root = ASTNode("", ast_node_type);
  std::vector<Token> expression_tokens =
      std::vector<Token>(tokens.begin(), tokens.begin() + operator_index);
  std::vector<Token> term_tokens =
      std::vector<Token>(tokens.begin() + operator_index, tokens.end());

  ASTNode expression = buildExpressionAST(expression_tokens);
  ASTNode term = buildTermAST(term_tokens);
  root.add_child(expression);
  root.add_child(term);
  return root;
}

/*
Similar idea to build expression, except now we split by different operators
instead
*/
ASTNode AST::buildTermAST(std::vector<Token> tokens) {
  for (int i = 0; i < tokens.size(); i++) {
    Token current_token = tokens[i];
    if (current_token.type == MUL) {
      return buildBinaryTermAST(tokens, "mul", i);
    } else if (current_token.type == DIV) {
      return buildBinaryTermAST(tokens, "div", i);
    } else if (current_token.type == MOD) {
      return buildBinaryTermAST(tokens, "mod", i);
    }
  }

  return buildFactorAST(tokens);
}

ASTNode AST::buildBinaryTermAST(std::vector<Token> tokens,
                                std::string ast_node_type, int operator_index) {
  ASTNode root = ASTNode("", ast_node_type);
  std::vector<Token> term_tokens =
      std::vector<Token>(tokens.begin(), tokens.begin() + operator_index);
  std::vector<Token> factor_tokens =
      std::vector<Token>(tokens.begin() + operator_index, tokens.end());

  ASTNode term = buildTermAST(term_tokens);
  ASTNode factor = buildFactorAST(factor_tokens);
  root.add_child(term);
  root.add_child(factor);
  return root;
}

/*
For now we assume that factors are only identifiers.
*/
ASTNode AST::buildFactorAST(std::vector<Token> tokens) {
  Token identifier = tokens[0];
  if (identifier.type == NAME) {
    return buildVariableNameAST(tokens);
  }

  return buildConstAST(tokens);
}

ASTNode AST::buildConstAST(std::vector<Token> tokens) {
  Token const_token = tokens[0];
  return ASTNode(const_token.value, "const");
}
