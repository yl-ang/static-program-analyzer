#pragma once
#include <memory>
#include <queue>
#include <string>
#include <unordered_set>
#include <vector>

#include "sp/ast/AstNode.h"
#include "sp/ast/grammar_nodes/ConstantNode.h"
#include "sp/ast/grammar_nodes/ExpressionNode.h"
#include "sp/ast/grammar_nodes/FactorNode.h"
#include "sp/ast/grammar_nodes/ProcedureNode.h"
#include "sp/ast/grammar_nodes/ProgramNode.h"
#include "sp/ast/grammar_nodes/TermNode.h"
#include "sp/ast/grammar_nodes/VariableNode.h"
#include "sp/ast/grammar_nodes/statements/AssignmentNode.h"
#include "sp/ast/grammar_nodes/statements/PrintNode.h"
#include "sp/ast/grammar_nodes/statements/ReadNode.h"
#include "sp/ast/grammar_nodes/statements/StatementListNode.h"
#include "sp/ast/grammar_nodes/statements/StatementNode.h"
#include "sp/exceptions/UnrecognisedTokenError.h"
#include "sp/tokenizer/Token.h"

const std::unordered_set<LEXICAL_TOKEN_TYPE> RelationalOperators = {
    LEXICAL_TOKEN_TYPE::LESS_THAN,    LEXICAL_TOKEN_TYPE::LESS_THAN_OR_EQUAL_TO,
    LEXICAL_TOKEN_TYPE::GREATER_THAN, LEXICAL_TOKEN_TYPE::GREATER_THAN_OR_EQUAL_TO,
    LEXICAL_TOKEN_TYPE::EQUAL_CHECK,  LEXICAL_TOKEN_TYPE::NOT_EQUAL_CHECK};
class AST {
public:
    ProgramNode buildAST(std::vector<Token> tokens);
    ProcedureNode buildProcedureAST(std::queue<Token>& tokens);
    StatementListNode buildStatementListAST(std::queue<Token>& tokens);
    StatementNode buildStatementAST(std::queue<Token>& tokens);
    AssignmentNode buildAssignmentAST(std::queue<Token>& tokens);
    ExpressionNode buildExpressionAST(std::queue<Token>& tokens);
    ExpressionNode buildBinaryExpressionAST(std::queue<Token>& tokens, std::queue<Token>& term,
                                            LEXICAL_TOKEN_TYPE type);
    ExpressionNode buildSubExpressionAST(std::queue<Token>& tokens, ExpressionNode& node);
    ExpressionNode buildTermAST(std::queue<Token>& tokens);
    ExpressionNode buildSubTermAST(std::queue<Token>& tokens, ExpressionNode& node);
    TermNode buildBinaryTermAST(std::queue<Token>& tokens, std::queue<Token>& factor, LEXICAL_TOKEN_TYPE type);
    ExpressionNode buildFactorAST(std::queue<Token>& tokens);
    VariableNode buildVarNameAST(Token token);
    ConstantNode buildIntAST(Token token);
    ReadNode buildReadAST(std::queue<Token>& tokens);
    PrintNode buildPrintAST(std::queue<Token>& tokens);
    ExpressionNode buildExprFromFactorAST(std::queue<Token>& tokens);
    ExpressionNode buildRelationalExpressionAST(std::queue<Token>& tokens);
    ExpressionNode buildRelationalFactorAST(std::queue<Token>& tokens);
};
