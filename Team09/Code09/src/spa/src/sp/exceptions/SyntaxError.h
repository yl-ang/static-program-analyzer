#pragma once

#include <string>
#include <exception>

class SyntaxError : public std::exception {
    private:
        const std::string errMsg;

    public: 
        SyntaxError(const std::string& errMsg);
        const char* what() const noexcept override;

};
