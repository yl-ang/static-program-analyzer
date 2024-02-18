#pragma once
#include <deque>
#include <memory>
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
#include "sp/ast/grammar_nodes/statements/StatementListNode.h"
#include "sp/ast/grammar_nodes/statements/StatementNode.h"
#include "sp/exceptions/LexicalSyntaxError.h"
#include "sp/exceptions/MissingTokenError.h"
#include "sp/exceptions/UnrecognisedTokenError.h"
#include "sp/tokenizer/Token.h"

class AST {
public:
    ProgramNode buildAST(std::vector<Token> tokens);
    ProcedureNode buildProcedureAST(std::deque<Token>& tokens);
    StatementListNode buildStatementListAST(std::deque<Token>& tokens);
    StatementNode buildStatementAST(std::deque<Token>& tokens);
    AssignmentNode buildAssignmentAST(std::deque<Token>& tokens);
    ExpressionNode buildExpressionAST(std::deque<Token>& tokens);
    ExpressionNode buildBinaryExpressionAST(std::deque<Token>& tokens,
                                            std::deque<Token>& term,
                                            LEXICAL_TOKEN_TYPE type);
    ExpressionNode buildSubExpressionAST(std::deque<Token>& tokens,
                                         ExpressionNode* node);
    TermNode buildTermAST(std::deque<Token>& tokens);
    TermNode buildSubTermAST(std::deque<Token>& tokens, TermNode* node);
    TermNode buildBinaryTermAST(std::deque<Token>& tokens,
                                std::deque<Token>& factor,
                                LEXICAL_TOKEN_TYPE type);
    FactorNode buildFactorAST(std::deque<Token>& tokens);

    void checkSyntax(LEXICAL_TOKEN_TYPE expected, LEXICAL_TOKEN_TYPE received);
    void checkMissingToken(LEXICAL_TOKEN_TYPE expected,
                           std::deque<Token>& tokens);
};
