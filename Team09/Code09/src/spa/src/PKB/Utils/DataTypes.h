#pragma once

#include <string>

// ai-gen start(gpt, 0, e)
// prompt:
// https://platform.openai.com/playground/p/JgbVHgb6Vyxb5DJwrkiMKS0k?model=gpt-4&mode=chat
enum class StatementType {
    READ,
    PRINT,
    ASSIGN,
    CALL,
    WHILE,
    IF,
};

typedef std::string Procedure;
typedef std::string Variable;
typedef std::string Constant;
typedef int StmtNum;

struct Stmt {
    StatementType type;
    StmtNum stmtNum;

    // must overload the == operator.
    bool operator==(const Stmt& other) const {
        return stmtNum == other.stmtNum && type == other.type;
    }
};
// ai-gen end

// ai-gen start(gpt, 0, e)
// prompt:
// https://platform.openai.com/playground/p/JMwYQcYxmb857W2JkifHSp5w?model=gpt-4&mode=chat
namespace std {
template <>
struct hash<Stmt> {
    std::size_t operator()(const Stmt& stmt) const {
        return ((hash<int>()(stmt.stmtNum) ^ (hash<int>()(static_cast<int>(stmt.type)) << 1)) >> 1);
    }
};
}  // namespace std
// ai-gen end

// Hash method for Follows and Parent Pairs i.e <StmtNum, StmtNum>
namespace std {
template <>
struct hash<std::pair<StmtNum, StmtNum>> {
    std::size_t operator()(const std::pair<StmtNum, StmtNum>& p) const {
        // Use a combination of hash functions for StmtNum
        return hash<StmtNum>()(p.first) ^ hash<StmtNum>()(p.second);
    }
};
}  // namespace std

// Hash method for Modifies and Uses Pairs i.e <StmtNum, Variable>
namespace std {
template <>
struct hash<std::pair<StmtNum, Variable>> {
    std::size_t operator()(const std::pair<StmtNum, Variable>& p) const {
        // Combine the hash values of StmtNum and Variable
        return hash<StmtNum>()(p.first) ^ hash<Variable>()(p.second);
    }
};
}  // namespace std

// Hash method for inner single pattern pair: i.e <lhs, rhs>
namespace std {
template <>
struct hash<std::pair<std::string, std::string>> {
    size_t operator()(const std::pair<std::string, std::string>& p) const {
        // Combine the hashes of lhs and rhs
        return hash<std::string>()(p.first) ^ (hash<std::string>()(p.second) << 1);
    }
};
}  // namespace std

// Hash method for full single pattern pairs: i.e <StmtNum, <lhs, rhs>>
namespace std {
template <>
struct hash<std::pair<int, std::pair<std::basic_string<char>, std::basic_string<char>>>> {
    size_t operator()(const std::pair<int, std::pair<std::basic_string<char>, std::basic_string<char>>>& p) const {
        size_t hash = 17;
        hash = hash * 31 + std::hash<int>{}(p.first);
        hash = hash * 31 + std::hash<std::basic_string<char>>{}(p.second.first);
        hash = hash * 31 + std::hash<std::basic_string<char>>{}(p.second.second);
        return hash;
    }
};
}  // namespace std
