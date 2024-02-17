#include <string>

#include "sp/tokenizer/Token.h"

std::string getLexicalEnumString(LEXICAL_TOKEN_TYPE type) {
    switch (type) {
        case LETTER:
            return "'letter'";
        case DIGIT:
            return "'digit'";
        case NZDIGIT:
            return "'nzdigit'";
        case PROCEDURE:
            return "'procedure'";
        case NAME:
            return "'name'";
        case INTEGER:
            return "'integer'";
        case WHILE:
            return "'while'";
        case READ:
            return "'read'";
        case PRINT:
            return "'print'";
        case CALL:
            return "'call'";
        case IF:
            return "'if'";
        case SEMICOLON:
            return "';'";
        case OPEN_CURLY_BRACE:
            return "'{'";
        case CLOSE_CURLY_BRACE:
            return "'}'";
        case OPEN_BRACKET:
            return "'['";
        case CLOSE_BRACKET:
            return "']'";
        case THEN:
            return "'then'";
        case ELSE:
            return "'else'";
        case EQUAL:
            return "'=='";
        case NOT:
            return "'!'";
        case ANDAND:
            return "'&&'";
        case OR:
            return "'||'";
        case LESS_THAN:
            return "'<'";
        case LESS_THAN_OR_EQUAL_TO:
            return "'<='";
        case GREATER_THAN:
            return "'>'";
        case GREATER_THAN_OR_EQUAL_TO:
            return "'>='";
        case EQUAL_CHECK:
            return "'=='";
        case NOT_EQUAL_CHECK:
            return "'!='";
        case ADD:
            return "'+'";
        case SUB:
            return "'-'";
        case MUL:
            return "'*'";
        case DIV:
            return "'/'";
        case MOD:
            return "'%'";
        default:
            return "unknown";
    }
}