#pragma once
#include <memory>
#include <queue>
#include <unordered_set>
#include <vector>

#include "sp/ast/SemanticValidator.h"
#include "sp/ast/grammar_nodes/ConstantNode.h"
#include "sp/ast/grammar_nodes/ExpressionNode.h"
#include "sp/ast/grammar_nodes/ProcedureNode.h"
#include "sp/ast/grammar_nodes/ProgramNode.h"
#include "sp/ast/grammar_nodes/VariableNode.h"
#include "sp/ast/grammar_nodes/statements/AssignmentNode.h"
#include "sp/ast/grammar_nodes/statements/CallNode.h"
#include "sp/ast/grammar_nodes/statements/IfNode.h"
#include "sp/ast/grammar_nodes/statements/PrintNode.h"
#include "sp/ast/grammar_nodes/statements/ReadNode.h"
#include "sp/ast/grammar_nodes/statements/StatementListNode.h"
#include "sp/ast/grammar_nodes/statements/StatementNode.h"
#include "sp/ast/grammar_nodes/statements/WhileNode.h"
#include "sp/tokenizer/Token.h"

const std::unordered_set<LEXICAL_TOKEN_TYPE> RelationalOperators = {
    LEXICAL_TOKEN_TYPE::LESS_THAN,    LEXICAL_TOKEN_TYPE::LESS_THAN_OR_EQUAL_TO,
    LEXICAL_TOKEN_TYPE::GREATER_THAN, LEXICAL_TOKEN_TYPE::GREATER_THAN_OR_EQUAL_TO,
    LEXICAL_TOKEN_TYPE::EQUAL_CHECK,  LEXICAL_TOKEN_TYPE::NOT_EQUAL_CHECK};
class AST {
    SemanticValidator semanticValidator;

public:
    std::shared_ptr<VariableNode> buildVarNameAST(Token token);
    std::shared_ptr<ConstantNode> buildIntAST(Token token);
    std::shared_ptr<ExpressionNode> buildExprFromFactorAST(std::queue<Token>& tokens);
    // its weird that buildFactorAST returns an ExpressionNode when ideally it should return a FactorNode.
    // this is just how the grammar works...
    std::shared_ptr<ExpressionNode> buildFactorAST(std::queue<Token>& tokens);
    std::shared_ptr<ExpressionNode> buildSubTermAST(std::queue<Token>& tokens, std::shared_ptr<ExpressionNode> node);
    std::shared_ptr<ExpressionNode> buildTermAST(std::queue<Token>& tokens);
    std::shared_ptr<ExpressionNode> buildRelationalFactorAST(std::queue<Token>& tokens);
    std::shared_ptr<ExpressionNode> buildRelationalExpressionAST(std::queue<Token>& tokens);
    std::shared_ptr<ExpressionNode> buildSubExpressionAST(std::queue<Token>& tokens,
                                                          std::shared_ptr<ExpressionNode> node);
    std::shared_ptr<ExpressionNode> buildExpressionAST(std::queue<Token>& tokens);
    std::shared_ptr<AssignmentNode> buildAssignmentAST(std::queue<Token>& tokens);
    std::shared_ptr<PrintNode> buildPrintAST(std::queue<Token>& tokens);
    std::shared_ptr<ReadNode> buildReadAST(std::queue<Token>& tokens);
    std::shared_ptr<StatementNode> buildStatementAST(std::queue<Token>& tokens);
    std::shared_ptr<StatementListNode> buildStatementListAST(std::queue<Token>& tokens);
    std::shared_ptr<ProcedureNode> buildProcedureAST(std::queue<Token>& tokens);
    std::shared_ptr<ProgramNode> buildAST(std::vector<Token> tokens);
    std::shared_ptr<ExpressionNode> buildConditionalExpressionAST(std::queue<Token>& tokens);
    std::shared_ptr<ExpressionNode> buildBinaryConditionalExpressionAST(std::queue<Token>& tokens);
    std::shared_ptr<ExpressionNode> handleBracketedCondExpr(std::queue<Token>& tokens);
    std::shared_ptr<WhileNode> buildWhileAST(std::queue<Token>& tokens);
    std::shared_ptr<IfNode> buildIfAST(std::queue<Token>& tokens);
    std::shared_ptr<CallNode> buildCallAST(std::queue<Token>& tokens);
};
