#pragma once

#include <memory>
#include <string>
#include <vector>

class Matchable {
public:
    virtual ~Matchable() = default;

    virtual bool match(std::shared_ptr<Matchable>) = 0;
};

class MatchableBuilder {
public:
    static std::shared_ptr<Matchable> fromExpressionString(const std::string& input);
};
