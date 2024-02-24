#pragma once
#include <memory>
#include <queue>
#include <string>
#include <vector>

#include "sp/ast/AstNode.h"
#include "sp/ast/grammar_nodes/ConstantNode.h"
#include "sp/ast/grammar_nodes/ExpressionNode.h"
#include "sp/ast/grammar_nodes/FactorNode.h"
#include "sp/ast/grammar_nodes/IntegerNode.h"
#include "sp/ast/grammar_nodes/NameNode.h"
#include "sp/ast/grammar_nodes/ProcedureNode.h"
#include "sp/ast/grammar_nodes/ProgramNode.h"
#include "sp/ast/grammar_nodes/TermNode.h"
#include "sp/ast/grammar_nodes/statements/AssignmentNode.h"
#include "sp/ast/grammar_nodes/statements/PrintNode.h"
#include "sp/ast/grammar_nodes/statements/ReadNode.h"
#include "sp/ast/grammar_nodes/statements/StatementListNode.h"
#include "sp/ast/grammar_nodes/statements/StatementNode.h"
#include "sp/exceptions/LexicalSyntaxError.h"
#include "sp/exceptions/MissingTokenError.h"
#include "sp/exceptions/UnrecognisedTokenError.h"
#include "sp/tokenizer/Token.h"

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
    ExpressionNode buildSubExpressionAST(std::queue<Token>& tokens, ExpressionNode* node);
    ExpressionNode buildTermAST(std::queue<Token>& tokens);
    ExpressionNode buildSubTermAST(std::queue<Token>& tokens, ExpressionNode* node);
    TermNode buildBinaryTermAST(std::queue<Token>& tokens, std::queue<Token>& factor, LEXICAL_TOKEN_TYPE type);
    ExpressionNode buildFactorAST(std::queue<Token>& tokens);

    void checkSyntax(LEXICAL_TOKEN_TYPE expected, LEXICAL_TOKEN_TYPE received);
    void checkMissingToken(LEXICAL_TOKEN_TYPE expected, std::queue<Token>& tokens);
    NameNode buildVarNameAST(Token token);
    IntegerNode buildIntAST(Token token);
    ReadNode buildReadAST(std::queue<Token>& tokens);
    PrintNode buildPrintAST(std::queue<Token>& tokens);
    ExpressionNode buildExprFromFactorAST(std::queue<Token>& tokens);
};
