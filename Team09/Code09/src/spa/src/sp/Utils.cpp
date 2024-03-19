#include "sp/Utils.h"

#include <memory>
#include <string>

#include "sp/PatternTreeNode.h"
#include "sp/tokenizer/Token.h"
std::string getLexicalEnumString(LEXICAL_TOKEN_TYPE type) {
    switch (type) {
    case LETTER:
        return "letter";
    case DIGIT:
        return "digit";
    case NZDIGIT:
        return "nzdigit";
    case PROCEDURE:
        return "procedure";
    case NAME:
        return "name";
    case INTEGER:
        return "integer";
    case WHILE:
        return "while";
    case READ:
        return "read";
    case PRINT:
        return "print";
    case CALL:
        return "call";
    case IF:
        return "if";
    case SEMICOLON:
        return ";";
    case OPEN_CURLY_BRACE:
        return "{";
    case CLOSE_CURLY_BRACE:
        return "}";
    case OPEN_BRACKET:
        return "[";
    case CLOSE_BRACKET:
        return "]";
    case THEN:
        return "then";
    case ELSE:
        return "else";
    case EQUAL:
        return "=";
    case NOT:
        return "!";
    case ANDAND:
        return "&&";
    case OR:
        return "||";
    case LESS_THAN:
        return "<";
    case LESS_THAN_OR_EQUAL_TO:
        return "<=";
    case GREATER_THAN:
        return ">";
    case GREATER_THAN_OR_EQUAL_TO:
        return ">=";
    case EQUAL_CHECK:
        return "==";
    case NOT_EQUAL_CHECK:
        return "!=";
    case ADD:
        return "add";
    case SUB:
        return "sub";
    case MUL:
        return "mul";
    case DIV:
        return "div";
    case MOD:
        return "mod";
    default:
        return "unknown";
    }
}

/*
s1 - SP side traversal
s2 - QPS side query string
This function checks whether s1 ==s2 by first creating a PatternTreeNode for s2.
Then, it deserialises s1 to get a PatternTreeNode for s2.
Finally, it performs comparison and returns the result
*/
bool isExactMatch(std::string spInput, std::string qpsInput) {
    std::shared_ptr<PatternTreeNode> spTree = PatternTreeNode::deserializeToNode(spInput);
    std::shared_ptr<PatternTreeNode> qpsTree = PatternTreeNode::buildTreeFromString(qpsInput);
    return PatternTreeNode::isEqual(spTree, qpsTree);
}

bool isPartialMatch(std::string spInput, std::string qpsInput) {
    std::shared_ptr<PatternTreeNode> spTree = PatternTreeNode::deserializeToNode(spInput);
    std::shared_ptr<PatternTreeNode> qpsTree = PatternTreeNode::buildTreeFromString(qpsInput);
    return PatternTreeNode::isPartiallyEqual(spTree, qpsTree);
}
