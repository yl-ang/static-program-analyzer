#pragma once

#include <string>

struct QPSConstants {
    inline static const std::string WHITESPACES = " \n\r\t\b\f";
    inline static const std::string SPACE = " ";
    inline static const std::string QUOTATIONS = "\"";
    inline static const std::string WILDCARD = "_";
    inline static const std::string TRUE_STRING = "TRUE";
    inline static const std::string FALSE_STRING = "FALSE";

    inline static const std::string STMT = "stmt";
    inline static const std::string READ = "read";
    inline static const std::string PRINT = "print";
    inline static const std::string CALL = "call";
    inline static const std::string WHILE = "while";
    inline static const std::string IF = "if";
    inline static const std::string ASSIGN = "assign";
    inline static const std::string VARIABLE = "variable";
    inline static const std::string CONSTANT = "constant";
    inline static const std::string PROCEDURE = "procedure";

    inline static const std::string DESIGN_ENTITIES = STMT + '|' + READ + '|' + PRINT + '|' + CALL + '|' + WHILE + '|' +
                                                      IF + '|' + ASSIGN + '|' + VARIABLE + '|' + CONSTANT + '|' +
                                                      PROCEDURE;

    inline static const std::string FOLLOWS = "Follows";
    inline static const std::string FOLLOWS_STAR = "Follows*";
    inline static const std::string FOLLOWS_STAR_REGEX = "Follows\\*";
    inline static const std::string PARENT = "Parent";
    inline static const std::string PARENT_STAR = "Parent*";
    inline static const std::string PARENT_STAR_REGEX = "Parent\\*";
    inline static const std::string USES = "Uses";
    inline static const std::string MODIFIES = "Modifies";
    inline static const std::string NEXT = "Next";
    inline static const std::string NEXT_STAR = "Next*";
    inline static const std::string NEXT_STAR_REGEX = "Next\\*";
    inline static const std::string CALLS = "Calls";
    inline static const std::string CALLS_STAR = "Calls*";
    inline static const std::string CALLS_STAR_REGEX = "Calls\\*";
    inline static const std::string AFFECTS = "Affects";

    inline static const std::string REL_REF_REGEX =
        FOLLOWS + '|' + FOLLOWS_STAR_REGEX + '|' + PARENT + '|' + PARENT_STAR_REGEX + '|' + USES + '|' + MODIFIES +
        '|' + NEXT + '|' + NEXT_STAR_REGEX + '|' + CALLS + '|' + CALLS_STAR_REGEX + '|' + AFFECTS;

    inline static const std::string SELECT = "Select";
    inline static const std::string BOOLEAN = "BOOLEAN";
    inline static const std::string SUCH_THAT = "such that";
    inline static const std::string PATTERN = "pattern";

    inline static const std::string WITH = "with";

    inline static const std::string AND = "and";
    inline static const std::string NOT = "not";

    inline static const std::string ATTR_PROC_NAME = "procName";
    inline static const std::string ATTR_VAR_NAME = "varName";
    inline static const std::string ATTR_VALUE = "value";
    inline static const std::string ATTR_STMT_NO = "stmt#";

    inline static const std::string ATTR_NAME =
        ATTR_PROC_NAME + "|" + ATTR_VAR_NAME + "|" + ATTR_VALUE + "|" + ATTR_STMT_NO;

    inline static const std::string EXPR_SPEC_CLAUSE_TYPE = "ExpressionSpec";
    inline static const std::string INTEGER_CLAUSE_TYPE = "Integer";
    inline static const std::string LITERAL_CLAUSE_TYPE = "Literal";
    inline static const std::string WILDCARD_CLAUSE_TYPE = "Wildcard";

    /**
     * Select Clause
     *
     * Select{>=1 whitespaces}{capturing group}
     * - works for BOOLEAN, single, tuple return
     * - works for word, word.word, and angle-brackets
     */
    /**
     * Capturing group format:
     * =======================
     * (\\w+|\\w+\\.\\w+|\\<.*?\\>): Captures either:
     * \w+\.\w+(?:#)?: One or more word characters followed by a dot and then one or more word characters followed by an
     *   optional '#' (for .stmt#), or
     * \w+: One or more word characters (letters, digits, or underscores), or
     * \\<.*?\\>: A word enclosed in angle brackets.
     *
     * Note that \w+\.\w+ must come before \w+, since \w+ is a subset of \w+\.\w+ and will terminate prematurely once
     * matched.
     */
    inline static const std::string SELECT_CLAUSE = "\\s*Select\\s+(\\w+\\.\\w+(?:#)?|\\w+|\\<.*?\\>)\\s*";

    // Such That Clause
    // {>=1 whitespaces}such{>=1 whitespaces}that{>=1 whitespaces}{capturing group}
    // capturing group format -> {letters/digits}{optional *}{>=0 whitespaces}{bracketed non-greedy}
    inline static const std::string SUCHTHAT_CLAUSE = "\\s*such\\s+that\\s+(?:not )?(\\w+\\*?\\s*\\(.*?\\))\\s*";

    // Pattern Clause
    inline static const std::string PATTERN_CLAUSE = "\\s*pattern\\s+(?:not )?(\\w+\\s*\\(.*?\\))\\s*";

    // With Clause
    inline static const std::string WITH_CLAUSE =
        "\\s*with\\ (?:not "
        ")?(\\w+\\.\\w+(?:#)?|\"\\s*\\w+\\s*\"|\\w+)\\s*=\\s*(\\w+\\.\\w+(?:#)?|\"\\s*\\w+\\s*\"|\\w+)\\s*";

    // And Clause
    // and{letters/digits}{optional *}{>=0 whitespaces}{bracketed non-greedy}
    inline static const std::string AND_CLAUSE =
        "\\s*and\\s+(?:not\\s+)?((\\w+\\*?)\\s*\\((.*?)\\)|(\\w+\\.\\w+(?:#)?|\"\\s*\\w+\\s*\"|\\w+)\\s*=\\s*(\\w+\\."
        "\\w+(?:#)?"
        "|\"\\s*\\w+\\s*\"|\\w+))\\s*";

    // {letters/digits}{optional *}{>=0 whitespaces}{bracketed non-greedy}
    inline static const std::string SUCHTHAT_ARGS = "\\s*(\\w+\\*?)\\s*\\((.*?)\\)\\s*";

    // {letters/digits}{>=0 whitespaces}{bracketed non-greedy}
    inline static const std::string PATTERN_ARGS = "\\s*(\\w+)\\s*\\((.*?)\\)\\s*";

    // <{capturing group}>
    inline static const std::string TUPLE = "^<([^<>]*)>$";
};
