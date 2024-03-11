#include "SyntaxValidator.h"

#include <iostream>

bool SyntaxValidator::validateSyntax(std::vector<Token> input) {
    // Initialise parsing table and stack
    SyntaxValidator::ParsingTable parsingTable = initialiseSIMPLEParsingTable();
    input.push_back(Token(LEXICAL_TOKEN_TYPE::END_OF_FILE, "$", -1));
    std::stack<SyntaxValidator::Symbol> parsingStack;
    parsingStack.push(LEXICAL_TOKEN_TYPE::END_OF_FILE);
    parsingStack.push(NonTerminal::NT_PROGRAM);

    size_t index = 0;
    while ((parsingStack.size() > 0) && (index < input.size())) {
        SyntaxValidator::Symbol stackTop = parsingStack.top();
        parsingStack.pop();

        LEXICAL_TOKEN_TYPE currToken = input[index].type;

        // Terminal at top of stack, and token match
        if (std::holds_alternative<LEXICAL_TOKEN_TYPE>(stackTop) &&
            std::get<LEXICAL_TOKEN_TYPE>(stackTop) == currToken) {
#ifdef DEBUG_BUILD
            std::cout << "top of stack terminal: " << input[index].value << std::endl;
#endif

            index++;
            // Non terminal at top of stack, find relevant grammar rule from parsing table and replace
        } else if (std::holds_alternative<NonTerminal>(stackTop)) {
            auto ite = parsingTable.find({std::get<NonTerminal>(stackTop), currToken});
            if (ite == parsingTable.end()) {
#ifdef DEBUG_BUILD
                std::cout << "top of stack non terminal ERROR: " << std::get<NonTerminal>(stackTop) << std::endl;
                std::cout << "curr token: " << input[index].value << std::endl;
                std::cout << "next token: " << input[index + 1].value << std::endl;
                std::cout << "next next token: " << input[index + 2].value << std::endl;
#endif

                throw SyntaxError("Unexpected Token, no grammar rule can be applied");
            }
#ifdef DEBUG_BUILD
            std::cout << "top of stack non terminal: " << std::get<NonTerminal>(stackTop) << std::endl;
            std::cout << "top of stack token: " << input[index].value << std::endl;
#endif

            std::vector<SyntaxValidator::Symbol> grammarRule = ite->second;
            // Handles epsilon in the first follows set
            if (grammarRule.empty()) {
                continue;
            }

            // Handle special case cond_expr
            if ((std::get<NonTerminal>(stackTop) == NT_COND_EXPR) && (currToken == LEXICAL_TOKEN_TYPE::OPEN_BRACKET)) {
#ifdef DEBUG_BUILD
                std::cout << "Disambiguate" << std::endl;
#endif

                grammarRule = disambiguateCondExprRule(index, input, grammarRule);
            }

            // Push grammar rule onto the stack in reverse order
            for (auto elem = grammarRule.rbegin(); elem != grammarRule.rend(); ++elem) {
                parsingStack.push(*elem);
            }
        } else {
// lexical token type but not = currtoken
#ifdef DEBUG_BUILD
            std::cout << "top of stack terminal ERROR: " << std::get<LEXICAL_TOKEN_TYPE>(stackTop) << std::endl;
            std::cout << "curr token: " << input[index].value << " in " << input[index - 2].value
                      << input[index - 1].value << input[index].value << input[index + 1].value
                      << input[index + 2].value << std::endl;
#endif

            throw SyntaxError("Unexpected grammar rule");
        }
    }
    if (!parsingStack.empty() || index != input.size()) {
        throw SyntaxError(" Unexpected end of input");
    }

#ifdef DEBUG_BUILD
    std::cout << "Syntax is valid" << std::endl;
#endif
    return true;
}

std::vector<SyntaxValidator::Symbol> SyntaxValidator::disambiguateCondExprRule(
    int index, std::vector<Token> input, std::vector<SyntaxValidator::Symbol> grammarRule) {
    std::stack<char> bracketStack;

#ifdef DEBUG_BUILD
    std::cout << "BracketStack size: " << bracketStack.size() << std::endl;
#endif

    int i = index;

    auto dupl = std::find(grammarRule.begin(), grammarRule.end(), SyntaxValidator::Symbol{NonTerminal::DUPL});

#ifdef DEBUG_BUILD
    if (dupl == grammarRule.end()) {
        std::cout << "ERROR: Could not find DUPL while trying to disambiguate" << std::endl;
    }
#endif

    // Perform bracket matching until the first closing parenthesis
    for (; i < input.size(); ++i) {
        LEXICAL_TOKEN_TYPE tokenType = input[i].type;
        if (tokenType == LEXICAL_TOKEN_TYPE::OPEN_BRACKET) {
            bracketStack.push('(');
        } else if (tokenType == LEXICAL_TOKEN_TYPE::CLOSE_BRACKET) {
            if (bracketStack.empty()) {
                throw SyntaxError("Unmatched close bracket");
            }
            bracketStack.pop();
            if (bracketStack.empty()) {
                // Check the next element after closing parenthesis
                if (i + 1 < input.size() &&
                    (input[i + 1].type == LEXICAL_TOKEN_TYPE::ANDAND || input[i + 1].type == LEXICAL_TOKEN_TYPE::OR)) {
// Grammar rule cond_expr: '(' cond_expr ')' _cond_expr
#ifdef DEBUG_BUILD
                    std::cout << "Using Grammar Rule ( cond expr ) _cond_expr" << std::endl;
#endif

                    grammarRule.erase(grammarRule.begin(), std::next(dupl));
                } else {
// Grammar rule cond_expr: rel_expr
#ifdef DEBUG_BUILD
                    std::cout << "Using Grammar Rule rel_expr" << std::endl;
#endif

                    grammarRule.erase(dupl, grammarRule.end());
                }

#ifdef DEBUG_BUILD
                std::cout << "Disambiguated Grammar Rule" << std::endl;
#endif

                return grammarRule;
            }
        }
    }
    throw SyntaxError("Unmatched open bracket");
}

SyntaxValidator::ParsingTable SyntaxValidator::initialiseSIMPLEParsingTable() {
    SyntaxValidator::ParsingTable parsingTable = {
        {{NonTerminal::NT_PROGRAM, LEXICAL_TOKEN_TYPE::PROCEDURE},
         {NonTerminal::NT_PROCEDURE, NonTerminal::NT__PROGRAM}},
        {{NonTerminal::NT__PROGRAM, LEXICAL_TOKEN_TYPE::PROCEDURE},
         {NonTerminal::NT_PROCEDURE, NonTerminal::NT__PROGRAM}},
        {{NonTerminal::NT__PROGRAM, LEXICAL_TOKEN_TYPE::END_OF_FILE}, {}},

        {{NonTerminal::NT_PROCEDURE, LEXICAL_TOKEN_TYPE::PROCEDURE},
         {LEXICAL_TOKEN_TYPE::PROCEDURE, NonTerminal::NT_PROC_NAME, LEXICAL_TOKEN_TYPE::OPEN_CURLY_BRACE,
          NonTerminal::NT_STMTLST, LEXICAL_TOKEN_TYPE::CLOSE_CURLY_BRACE}},

        {{NonTerminal::NT_STMTLST, LEXICAL_TOKEN_TYPE::READ}, {NonTerminal::NT_STMT, NonTerminal::NT__STMTLST}},
        {{NonTerminal::NT_STMTLST, LEXICAL_TOKEN_TYPE::PRINT}, {NonTerminal::NT_STMT, NonTerminal::NT__STMTLST}},
        {{NonTerminal::NT_STMTLST, LEXICAL_TOKEN_TYPE::WHILE}, {NonTerminal::NT_STMT, NonTerminal::NT__STMTLST}},
        {{NonTerminal::NT_STMTLST, LEXICAL_TOKEN_TYPE::IF}, {NonTerminal::NT_STMT, NonTerminal::NT__STMTLST}},
        {{NonTerminal::NT_STMTLST, LEXICAL_TOKEN_TYPE::NAME}, {NonTerminal::NT_STMT, NonTerminal::NT__STMTLST}},

        {{NonTerminal::NT__STMTLST, LEXICAL_TOKEN_TYPE::CLOSE_CURLY_BRACE}, {}},
        {{NonTerminal::NT__STMTLST, LEXICAL_TOKEN_TYPE::READ}, {NonTerminal::NT_STMT, NonTerminal::NT__STMTLST}},
        {{NonTerminal::NT__STMTLST, LEXICAL_TOKEN_TYPE::PRINT}, {NonTerminal::NT_STMT, NonTerminal::NT__STMTLST}},
        {{NonTerminal::NT__STMTLST, LEXICAL_TOKEN_TYPE::WHILE}, {NonTerminal::NT_STMT, NonTerminal::NT__STMTLST}},
        {{NonTerminal::NT__STMTLST, LEXICAL_TOKEN_TYPE::IF}, {NonTerminal::NT_STMT, NonTerminal::NT__STMTLST}},
        {{NonTerminal::NT__STMTLST, LEXICAL_TOKEN_TYPE::NAME}, {NonTerminal::NT_STMT, NonTerminal::NT__STMTLST}},

        {{NonTerminal::NT_STMT, LEXICAL_TOKEN_TYPE::READ}, {NonTerminal::NT_READ}},
        {{NonTerminal::NT_STMT, LEXICAL_TOKEN_TYPE::PRINT}, {NonTerminal::NT_PRINT}},
        {{NonTerminal::NT_STMT, LEXICAL_TOKEN_TYPE::WHILE}, {NonTerminal::NT_WHILE}},
        {{NonTerminal::NT_STMT, LEXICAL_TOKEN_TYPE::IF}, {NonTerminal::NT_IF}},
        {{NonTerminal::NT_STMT, LEXICAL_TOKEN_TYPE::NAME}, {NonTerminal::NT_ASSIGN}},

        {{NonTerminal::NT_READ, LEXICAL_TOKEN_TYPE::READ},
         {LEXICAL_TOKEN_TYPE::READ, NonTerminal::NT_VAR_NAME, LEXICAL_TOKEN_TYPE::SEMICOLON}},

        {{NonTerminal::NT_PRINT, LEXICAL_TOKEN_TYPE::PRINT},
         {LEXICAL_TOKEN_TYPE::PRINT, NonTerminal::NT_VAR_NAME, LEXICAL_TOKEN_TYPE::SEMICOLON}},

        {{NonTerminal::NT_WHILE, LEXICAL_TOKEN_TYPE::WHILE},
         {LEXICAL_TOKEN_TYPE::WHILE, LEXICAL_TOKEN_TYPE::OPEN_BRACKET, NonTerminal::NT_COND_EXPR,
          LEXICAL_TOKEN_TYPE::CLOSE_BRACKET, LEXICAL_TOKEN_TYPE::OPEN_CURLY_BRACE, NonTerminal::NT_STMTLST,
          LEXICAL_TOKEN_TYPE::CLOSE_CURLY_BRACE}},

        {{NonTerminal::NT_IF, LEXICAL_TOKEN_TYPE::IF},
         {LEXICAL_TOKEN_TYPE::IF, LEXICAL_TOKEN_TYPE::OPEN_BRACKET, NonTerminal::NT_COND_EXPR,
          LEXICAL_TOKEN_TYPE::CLOSE_BRACKET, LEXICAL_TOKEN_TYPE::THEN, LEXICAL_TOKEN_TYPE::OPEN_CURLY_BRACE,
          NonTerminal::NT_STMTLST, LEXICAL_TOKEN_TYPE::CLOSE_CURLY_BRACE, LEXICAL_TOKEN_TYPE::ELSE,
          LEXICAL_TOKEN_TYPE::OPEN_CURLY_BRACE, NonTerminal::NT_STMTLST, LEXICAL_TOKEN_TYPE::CLOSE_CURLY_BRACE}},

        {{NonTerminal::NT_ASSIGN, LEXICAL_TOKEN_TYPE::NAME},
         {NonTerminal::NT_VAR_NAME, LEXICAL_TOKEN_TYPE::EQUAL, NonTerminal::NT_EXPR, LEXICAL_TOKEN_TYPE::SEMICOLON}},

        {{NonTerminal::NT_COND_EXPR, LEXICAL_TOKEN_TYPE::OPEN_BRACKET},
         {NonTerminal::NT_REL_EXPR, NonTerminal::DUPL, LEXICAL_TOKEN_TYPE::OPEN_BRACKET, NonTerminal::NT_COND_EXPR,
          LEXICAL_TOKEN_TYPE::CLOSE_BRACKET, NonTerminal::NT__COND_EXPR}},
        {{NonTerminal::NT_COND_EXPR, LEXICAL_TOKEN_TYPE::NOT},
         {LEXICAL_TOKEN_TYPE::NOT, LEXICAL_TOKEN_TYPE::OPEN_BRACKET, NonTerminal::NT_COND_EXPR,
          LEXICAL_TOKEN_TYPE::CLOSE_BRACKET}},
        {{NonTerminal::NT_COND_EXPR, LEXICAL_TOKEN_TYPE::NAME}, {NonTerminal::NT_REL_EXPR}},
        {{NonTerminal::NT_COND_EXPR, LEXICAL_TOKEN_TYPE::INTEGER}, {NonTerminal::NT_REL_EXPR}},

        {{NonTerminal::NT__COND_EXPR, LEXICAL_TOKEN_TYPE::ANDAND},
         {LEXICAL_TOKEN_TYPE::ANDAND, LEXICAL_TOKEN_TYPE::OPEN_BRACKET, NonTerminal::NT_COND_EXPR,
          LEXICAL_TOKEN_TYPE::CLOSE_BRACKET}},
        {{NonTerminal::NT__COND_EXPR, LEXICAL_TOKEN_TYPE::OR},
         {LEXICAL_TOKEN_TYPE::OR, LEXICAL_TOKEN_TYPE::OPEN_BRACKET, NonTerminal::NT_COND_EXPR,
          LEXICAL_TOKEN_TYPE::CLOSE_BRACKET}},

        {{NonTerminal::NT_REL_EXPR, LEXICAL_TOKEN_TYPE::OPEN_BRACKET},
         {NonTerminal::NT_REL_FACTOR, NonTerminal::NT_REL_OP, NonTerminal::NT_REL_FACTOR}},
        {{NonTerminal::NT_REL_EXPR, LEXICAL_TOKEN_TYPE::NAME},
         {NonTerminal::NT_REL_FACTOR, NonTerminal::NT_REL_OP, NonTerminal::NT_REL_FACTOR}},
        {{NonTerminal::NT_REL_EXPR, LEXICAL_TOKEN_TYPE::INTEGER},
         {NonTerminal::NT_REL_FACTOR, NonTerminal::NT_REL_OP, NonTerminal::NT_REL_FACTOR}},

        {{NonTerminal::NT_REL_OP, LEXICAL_TOKEN_TYPE::GREATER_THAN}, {LEXICAL_TOKEN_TYPE::GREATER_THAN}},
        {{NonTerminal::NT_REL_OP, LEXICAL_TOKEN_TYPE::GREATER_THAN_OR_EQUAL_TO},
         {LEXICAL_TOKEN_TYPE::GREATER_THAN_OR_EQUAL_TO}},
        {{NonTerminal::NT_REL_OP, LEXICAL_TOKEN_TYPE::LESS_THAN}, {LEXICAL_TOKEN_TYPE::LESS_THAN}},
        {{NonTerminal::NT_REL_OP, LEXICAL_TOKEN_TYPE::LESS_THAN_OR_EQUAL_TO},
         {LEXICAL_TOKEN_TYPE::LESS_THAN_OR_EQUAL_TO}},
        {{NonTerminal::NT_REL_OP, LEXICAL_TOKEN_TYPE::EQUAL_CHECK}, {LEXICAL_TOKEN_TYPE::EQUAL_CHECK}},
        {{NonTerminal::NT_REL_OP, LEXICAL_TOKEN_TYPE::NOT_EQUAL_CHECK}, {LEXICAL_TOKEN_TYPE::NOT_EQUAL_CHECK}},

        {{NonTerminal::NT_REL_FACTOR, LEXICAL_TOKEN_TYPE::OPEN_BRACKET}, {NonTerminal::NT_EXPR}},
        {{NonTerminal::NT_REL_FACTOR, LEXICAL_TOKEN_TYPE::NAME}, {NonTerminal::NT_EXPR}},
        {{NonTerminal::NT_REL_FACTOR, LEXICAL_TOKEN_TYPE::INTEGER}, {NonTerminal::NT_EXPR}},

        {{NonTerminal::NT_EXPR, LEXICAL_TOKEN_TYPE::OPEN_BRACKET}, {NonTerminal::NT_TERM, NonTerminal::NT__EXPR}},
        {{NonTerminal::NT_EXPR, LEXICAL_TOKEN_TYPE::NAME}, {NonTerminal::NT_TERM, NonTerminal::NT__EXPR}},
        {{NonTerminal::NT_EXPR, LEXICAL_TOKEN_TYPE::INTEGER}, {NonTerminal::NT_TERM, NonTerminal::NT__EXPR}},

        {{NonTerminal::NT__EXPR, LEXICAL_TOKEN_TYPE::SEMICOLON}, {}},
        {{NonTerminal::NT__EXPR, LEXICAL_TOKEN_TYPE::CLOSE_BRACKET}, {}},
        {{NonTerminal::NT__EXPR, LEXICAL_TOKEN_TYPE::GREATER_THAN}, {}},
        {{NonTerminal::NT__EXPR, LEXICAL_TOKEN_TYPE::GREATER_THAN_OR_EQUAL_TO}, {}},
        {{NonTerminal::NT__EXPR, LEXICAL_TOKEN_TYPE::LESS_THAN}, {}},
        {{NonTerminal::NT__EXPR, LEXICAL_TOKEN_TYPE::LESS_THAN_OR_EQUAL_TO}, {}},
        {{NonTerminal::NT__EXPR, LEXICAL_TOKEN_TYPE::EQUAL_CHECK}, {}},
        {{NonTerminal::NT__EXPR, LEXICAL_TOKEN_TYPE::NOT_EQUAL_CHECK}, {}},
        {{NonTerminal::NT__EXPR, LEXICAL_TOKEN_TYPE::ADD},
         {LEXICAL_TOKEN_TYPE::ADD, NonTerminal::NT_TERM, NonTerminal::NT__EXPR}},
        {{NonTerminal::NT__EXPR, LEXICAL_TOKEN_TYPE::SUB},
         {LEXICAL_TOKEN_TYPE::SUB, NonTerminal::NT_TERM, NonTerminal::NT__EXPR}},

        {{NonTerminal::NT_TERM, LEXICAL_TOKEN_TYPE::OPEN_BRACKET}, {NonTerminal::NT_FACTOR, NonTerminal::NT__TERM}},
        {{NonTerminal::NT_TERM, LEXICAL_TOKEN_TYPE::NAME}, {NonTerminal::NT_FACTOR, NonTerminal::NT__TERM}},
        {{NonTerminal::NT_TERM, LEXICAL_TOKEN_TYPE::INTEGER}, {NonTerminal::NT_FACTOR, NonTerminal::NT__TERM}},

        {{NonTerminal::NT__TERM, LEXICAL_TOKEN_TYPE::SEMICOLON}, {}},
        {{NonTerminal::NT__TERM, LEXICAL_TOKEN_TYPE::CLOSE_BRACKET}, {}},
        {{NonTerminal::NT__TERM, LEXICAL_TOKEN_TYPE::GREATER_THAN}, {}},
        {{NonTerminal::NT__TERM, LEXICAL_TOKEN_TYPE::GREATER_THAN_OR_EQUAL_TO}, {}},
        {{NonTerminal::NT__TERM, LEXICAL_TOKEN_TYPE::LESS_THAN}, {}},
        {{NonTerminal::NT__TERM, LEXICAL_TOKEN_TYPE::LESS_THAN_OR_EQUAL_TO}, {}},
        {{NonTerminal::NT__TERM, LEXICAL_TOKEN_TYPE::EQUAL_CHECK}, {}},
        {{NonTerminal::NT__TERM, LEXICAL_TOKEN_TYPE::NOT_EQUAL_CHECK}, {}},
        {{NonTerminal::NT__TERM, LEXICAL_TOKEN_TYPE::ADD}, {}},
        {{NonTerminal::NT__TERM, LEXICAL_TOKEN_TYPE::SUB}, {}},
        {{NonTerminal::NT__TERM, LEXICAL_TOKEN_TYPE::MUL},
         {LEXICAL_TOKEN_TYPE::MUL, NonTerminal::NT_FACTOR, NonTerminal::NT__TERM}},
        {{NonTerminal::NT__TERM, LEXICAL_TOKEN_TYPE::DIV},
         {LEXICAL_TOKEN_TYPE::DIV, NonTerminal::NT_FACTOR, NonTerminal::NT__TERM}},
        {{NonTerminal::NT__TERM, LEXICAL_TOKEN_TYPE::MOD},
         {LEXICAL_TOKEN_TYPE::MOD, NonTerminal::NT_FACTOR, NonTerminal::NT__TERM}},

        {{NonTerminal::NT_FACTOR, LEXICAL_TOKEN_TYPE::OPEN_BRACKET},
         {LEXICAL_TOKEN_TYPE::OPEN_BRACKET, NonTerminal::NT_EXPR, LEXICAL_TOKEN_TYPE::CLOSE_BRACKET}},
        {{NonTerminal::NT_FACTOR, LEXICAL_TOKEN_TYPE::NAME}, {NonTerminal::NT_VAR_NAME}},
        {{NonTerminal::NT_FACTOR, LEXICAL_TOKEN_TYPE::INTEGER}, {NonTerminal::NT_CONST_VALUE}},

        {{NonTerminal::NT_VAR_NAME, LEXICAL_TOKEN_TYPE::NAME}, {LEXICAL_TOKEN_TYPE::NAME}},
        {{NonTerminal::NT_PROC_NAME, LEXICAL_TOKEN_TYPE::NAME}, {LEXICAL_TOKEN_TYPE::NAME}},
        {{NonTerminal::NT_CONST_VALUE, LEXICAL_TOKEN_TYPE::INTEGER}, {LEXICAL_TOKEN_TYPE::INTEGER}}};

    return parsingTable;
}
