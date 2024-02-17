#pragma once
#include <queue>
#include <string>
#include <vector>

#include "ProgramNode.h"
#include "StatementListNode.h"
#include "ast/grammar_nodes/NameNode.h"
#include "ast/grammar_nodes/ProcedureNode.h"
#include "ast/grammar_nodes/statements/AssignmentNode.h"
#include "ast/grammar_nodes/statements/StatementNode.h"
#include "sp/ast/AstNode.h"
#include "sp/exceptions/LexicalSyntaxError.h"
#include "sp/exceptions/MissingTokenError.h"
#include "sp/exceptions/UnrecognisedTokenError.h"
#include "sp/tokenizer/Token.h"

class AST {
public:
    ProgramNode buildAST(std::vector<Token> tokens);
    ProcedureNode buildProcedureAST(std::queue<Token> tokens);
    StatementListNode buildStatementListAST(std::queue<Token> tokens);
    StatementNode buildStatementAST(std::queue<Token> tokens);
    AssignmentNode buildAssignmentAST(std::queue<Token> tokens);
    void checkSyntax(LEXICAL_TOKEN_TYPE expected, LEXICAL_TOKEN_TYPE received);
    void checkMissingToken(LEXICAL_TOKEN_TYPE expected,
                           std::queue<Token> tokens);
};
