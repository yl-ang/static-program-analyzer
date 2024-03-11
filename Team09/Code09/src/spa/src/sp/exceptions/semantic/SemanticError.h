#pragma once

#include <exception>
#include <string>

class SemanticError : public std::exception {
private:
    const std::string errMsg;

public:
    SemanticError(const std::string& errMsg);
    const char* what() const noexcept override;
};
