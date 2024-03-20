#pragma once

#include <regex>

struct QPSRegexes {
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
     * \w+: One or more word characters (letters, digits, or underscores), or
     * \w+\.\w+: One or more word characters followed by a dot and then one or more word characters, or
     * \\<.*?\\>: A word enclosed in angle brackets.
     */
    inline static const std::regex SELECT_CLAUSE = std::regex("\\s*Select\\s+(\\w+|\\w+\\.\\w+|\\<.*?\\>)\\s*");

    // Such That Clause
    // {>=1 whitespaces}such{>=1 whitespaces}that{>=1 whitespaces}{capturing group}
    // capturing group format -> {letters/digits}{optional *}{>=0 whitespaces}{bracketed non-greedy}
    inline static const std::regex SUCHTHAT_CLAUSE = std::regex("\\s*such\\s+that\\s+(\\w+\\*?\\s*\\(.*?\\))\\s*");

    // Pattern Clause
    inline static const std::regex PATTERN_CLAUSE = std::regex("\\s*pattern\\s+(\\w+\\s*\\(.*?\\))\\s*");
    
    // And Clause
    // and{letters/digits}{optional *}{>=0 whitespaces}{bracketed non-greedy}
    inline static const std::regex AND_CLAUSE = std::regex("\\s*and\\s+(\\w+\\*?)\\s*\\((.*?)\\)\\s*");

    // {letters/digits}{optional *}{>=0 whitespaces}{bracketed non-greedy}
    inline static const std::regex SUCHTHAT_ARGS = std::regex("\\s*(\\w+\\*?)\\s*\\((.*?)\\)\\s*");

    // {letters/digits}{>=0 whitespaces}{bracketed non-greedy}
    inline static const std::regex PATTERN_ARGS = std::regex("\\s*(\\w+)\\s*\\((.*?)\\)\\s*");
    
    // <{capturing group}>
    inline static const std::regex TUPLE = std::regex("^<([^<>]*)>$");
};
