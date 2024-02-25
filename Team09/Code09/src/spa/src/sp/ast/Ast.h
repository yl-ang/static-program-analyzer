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
    std::unique_ptr<VariableNode> buildVarNameAST(Token token);
    std::unique_ptr<ConstantNode> buildIntAST(Token token);
    std::unique_ptr<ExpressionNode> buildExprFromFactorAST(std::queue<Token>& tokens);
    std::unique_ptr<ExpressionNode> buildFactorAST(std::queue<Token>& tokens);
    std::unique_ptr<ExpressionNode> buildSubTermAST(std::queue<Token>& tokens, std::unique_ptr<ExpressionNode> node);
    std::unique_ptr<ExpressionNode> buildTermAST(std::queue<Token>& tokens);
    std::unique_ptr<ExpressionNode> buildRelationalFactorAST(std::queue<Token>& tokens);
    std::unique_ptr<ExpressionNode> buildRelationalExpressionAST(std::queue<Token>& tokens);
    std::unique_ptr<ExpressionNode> buildSubExpressionAST(std::queue<Token>& tokens,
                                                          std::unique_ptr<ExpressionNode> node);
    std::unique_ptr<ExpressionNode> buildExpressionAST(std::queue<Token>& tokens);
    std::unique_ptr<AssignmentNode> buildAssignmentAST(std::queue<Token>& tokens);
    std::unique_ptr<PrintNode> buildPrintAST(std::queue<Token>& tokens);
    std::unique_ptr<ReadNode> buildReadAST(std::queue<Token>& tokens);
    std::unique_ptr<StatementNode> buildStatementAST(std::queue<Token>& tokens);
    std::unique_ptr<StatementListNode> buildStatementListAST(std::queue<Token>& tokens);
    std::unique_ptr<ProcedureNode> buildProcedureAST(std::queue<Token>& tokens);
    std::unique_ptr<ProgramNode> buildAST(std::vector<Token> tokens);
};
