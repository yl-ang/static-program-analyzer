#pragma once
#include <algorithm>
#include <iostream>
#include <stack>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include "../exceptions/SyntaxError.h"
#include "Token.h"

// Define non-terminals, no need to define terminals since LEXICAL_TOKEN_TYPES are terminals
enum class NonTerminal {
    NT_PROGRAM,
    NT_PROCEDURE,
    NT_STMTLST,
    NT__STMTLST,
    NT_STMT,
    NT_READ,
    NT_PRINT,
    NT_WHILE,
    NT_IF,
    NT_ASSIGN,
    NT_COND_EXPR,
    NT__COND_EXPR,
    NT_REL_EXPR,
    NT_REL_OP,
    NT_REL_FACTOR,
    NT_EXPR,
    NT__EXPR,
    NT_TERM,
    NT__TERM,
    NT_FACTOR,
    NT_VAR_NAME,
    NT_PROC_NAME,
    NT_CONST_VALUE,
    DUPL
};

/**
 * @brief Does syntax validation using the LL(1) Parsing Technique, which is a top-down parser which requires grammar to
 * follow a certain set of rules, such as no left recursion and no left factoring. More on how the LL(1) Parsing
 * Technique works can be found at https://en.wikipedia.org/wiki/LL_parser
 */
class SyntaxValidator {
public:
    // Define custom hash function for std::pair<NonTerminal, LEXICAL_TOKEN_TYPE>
    struct PairHash {
        template <typename T1, typename T2>
        std::size_t operator()(const std::pair<T1, T2>& p) const {
            auto hashT1 = std::hash<T1>{}(p.first);
            auto hashT2 = std::hash<T2>{}(p.second);
            return hashT1 ^ hashT2;
        }
    };

    using Symbol = std::variant<NonTerminal, LEXICAL_TOKEN_TYPE>;
    using SymbolPair = std::pair<NonTerminal, LEXICAL_TOKEN_TYPE>;
    using ParsingTable = std::unordered_map<SymbolPair, std::vector<Symbol>, PairHash>;

    /**
     * @brief Parse input expression using LL(1) parsing technique
     */
    bool validateSyntax(std::vector<Token> input);

    /**
     * @brief Special case for COND_EXPR grammar rule. (Hacky fix)
     * For the pair {COND_EXPR, (}, 2 Rules: COND_EXPR -> REL_EXPR or COND_EXPR -> '(' COND_EXPR ')' _COND_EXPR can be
     * applied. The LL(1) parsing technique does not allow for such cases of duplicates, so we need to handle this case
     * explicitly. To handle this, we do bracket matching then check if '&&' or '||' is the next element. If so, apply
     * the rule COND_EXPR -> '(' COND_EXPR ')' _COND_EXPR If not, apply the rule COND_EXPR -> REL_EXPR
     */
    std::vector<SyntaxValidator::Symbol> disambiguateCondExprRule(int index, std::vector<Token> input,
                                                                  std::vector<SyntaxValidator::Symbol> grammarRule);

    /**
     * @brief Initialise parsing table with modified SIMPLE grammar rules
     * (Left recursion and left factoring removed)
     */
    ParsingTable initialiseSIMPLEParsingTable();
};
