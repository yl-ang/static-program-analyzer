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
        return ((hash<int>()(stmt.stmtNum) ^
                 (hash<int>()(static_cast<int>(stmt.type)) << 1)) >>
                1);
    }
};
}  // namespace std
// ai-gen end

namespace std {
template <>
struct hash<std::pair<StmtNum, StmtNum>> {
    std::size_t operator()(const std::pair<StmtNum, StmtNum>& p) const {
        // Use a combination of hash functions for StmtNum
        return hash<StmtNum>()(p.first) ^ hash<StmtNum>()(p.second);
    }
};
}  // namespace std

namespace std {
template <>
struct hash<std::pair<StmtNum, Variable>> {
    std::size_t operator()(const std::pair<StmtNum, Variable>& p) const {
        // Combine the hash values of StmtNum and Variable
        return hash<StmtNum>()(p.first) ^ hash<Variable>()(p.second);
    }
};
}  // namespace std
