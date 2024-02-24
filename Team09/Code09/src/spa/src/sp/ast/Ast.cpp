#include <sp/ast/Ast.h>

#include <iostream>
#include <memory>
#include <queue>
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

ProgramNode AST::buildAST(std::vector<Token> tokens) {
    ProgramNode root = ProgramNode();
    std::queue<Token> token_queue = {};
    for (auto token : tokens) {
        token_queue.push(token);
    }

    while (token_queue.size()) {
        ProcedureNode procedure = buildProcedureAST(token_queue);
        root.add_child(procedure);
    }

    return root;
}
/*
Grammar: 'procedure' proc_name { stmtList }
The first word should be procedure, so we check for that by looking at the first
element of the queue. Otherwise, we throw a syntax error. Then we look at the
next token to see if its a name or not then finally we process statement lists,
which is enclosed in {}
*/
ProcedureNode AST::buildProcedureAST(std::queue<Token>& tokens) {
    tokens.pop();
    Token procName = tokens.front();
    ProcedureNode node = ProcedureNode(procName.value);
    tokens.pop();

    StatementListNode statementList = buildStatementListAST(tokens);
    node.add_child(statementList);
    return node;
}

/*
stmt list passes here includes the curly braces, so we can check for that as
well.

*/
StatementListNode AST::buildStatementListAST(std::queue<Token>& tokens) {
    tokens.pop();

    StatementListNode statementList = StatementListNode();
    std::vector<StatementNode> statements = {};
    while (tokens.size() && tokens.front().type != CLOSE_CURLY_BRACE) {
        StatementNode statement = buildStatementAST(tokens);
        statementList.add_child(statement);
    }

    tokens.pop();
    return statementList;
}

StatementNode AST::buildStatementAST(std::queue<Token>& tokens) {
    Token first_token = tokens.front();
    if (first_token.type == NAME) {
        return buildAssignmentAST(tokens);
    } else if (first_token.type == READ) {
        return buildReadAST(tokens);
    } else if (first_token.type == PRINT) {
        return buildPrintAST(tokens);
    }
    throw UnrecognisedTokenError(first_token.type);
}

ReadNode AST::buildReadAST(std::queue<Token>& tokens) {
    ReadNode readNode = ReadNode();
    tokens.pop();
    Token varName = tokens.front();
    VarNode nameNode = buildVarNameAST(varName);
    tokens.pop();
    readNode.add_child(nameNode);
    tokens.pop();
    return readNode;
}

PrintNode AST::buildPrintAST(std::queue<Token>& tokens) {
    PrintNode printNode = PrintNode();
    tokens.pop();
    Token varName = tokens.front();
    VarNode nameNode = buildVarNameAST(varName);
    tokens.pop();
    printNode.add_child(nameNode);
    tokens.pop();
    return printNode;
}

AssignmentNode AST::buildAssignmentAST(std::queue<Token>& tokens) {
    Token varName = tokens.front();
    VarNode nameNode = VarNode(varName.value);
    tokens.pop();
    AssignmentNode node = AssignmentNode();
    tokens.pop();
    ExpressionNode expression = buildExpressionAST(tokens);
    tokens.pop();

    node.add_child(nameNode);
    node.add_child(expression);
    return node;
}

/*
To perform expression parsing, we require manipulating left recursion. To
maintain the integrity of our queue which gives us elements from left to right.
The idea is to remove left recursion somehow...

Rule: A: A '+' B | A '-' B | B.
We can express this rule in terms of x, y where x is a non-empty sequence of
nonterminal and terminal, and y is a sequence of nonterminals and terminals that
does not start with A.

Rewriting the rule, we get the following rule:
A: A(x1) | A(x2) | y, x1 represents '+' B and x2 represents '-' B.

We can split the rule above into 2 sets, one for A, and another for
nonterminals, A'. A: BA' A': (x1)A' | (x2)A' | e. e represents an empty string.
So the rules we will be solving for becomes the following:
A: BA'
A': '+' BA' | '-' BA' | e

More info can be found here: https://en.wikipedia.org/wiki/Left_recursion

*/
ExpressionNode AST::buildExpressionAST(std::queue<Token>& tokens) {
    /*
    We perform upcasting here because there is a chance of the following flow
    happening: Expr -> term -> factor -> expr.
    */
    ExpressionNode term = buildTermAST(tokens);
    return buildSubExpressionAST(tokens, &term);
}

ExpressionNode AST::buildSubExpressionAST(std::queue<Token>& tokens, ExpressionNode* node) {
    Token front = tokens.front();
    if (front.type == ADD || front.type == SUB) {
        tokens.pop();
        ExpressionNode term = buildTermAST(tokens);
        ExpressionNode expressionNode = ExpressionNode(front.type);
        expressionNode.add_child(*node);
        expressionNode.add_child(term);

        return buildSubExpressionAST(tokens, &expressionNode);
    }

    return *node;
}

/*
Similar idea to buildExpression, because of its left recursive nature, we can
perform the same thing. The rules will be transformed from: A : A '*' B | A '/'
B | A '%' B | B

to the following 2 rules:
A : BA'
A' : '*' BA' | '/' BA' | '%' BA' | e
*/
ExpressionNode AST::buildTermAST(std::queue<Token>& tokens) {
    ExpressionNode factorNode = buildFactorAST(tokens);
    return buildSubTermAST(tokens, &factorNode);
}

ExpressionNode AST::buildSubTermAST(std::queue<Token>& tokens, ExpressionNode* node) {
    Token front = tokens.front();
    if (front.type == MUL || front.type == MOD || front.type == DIV) {
        tokens.pop();
        ExpressionNode factorNode = buildFactorAST(tokens);
        ExpressionNode termNode = TermNode(front.type);
        termNode.add_child(*node);
        termNode.add_child(factorNode);

        return buildSubTermAST(tokens, &termNode);
    }

    return *node;
}

ExpressionNode AST::buildFactorAST(std::queue<Token>& tokens) {
    Token token = tokens.front();
    tokens.pop();
    switch (token.type) {
    case NAME:
        return buildVarNameAST(token);
    case INTEGER:
        return buildIntAST(token);
    case OPEN_BRACKET:
        return buildExprFromFactorAST(tokens);
    }
}

ExpressionNode AST::buildExprFromFactorAST(std::queue<Token>& tokens) {
    ExpressionNode expression = buildExpressionAST(tokens);
    tokens.pop();
    return expression;
}

VarNode AST::buildVarNameAST(Token token) {
    return VarNode(token.value);
}

ConstNode AST::buildIntAST(Token token) {
    return ConstNode(token.value);
}
