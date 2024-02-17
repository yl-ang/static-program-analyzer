#pragma once

#include <exception>
#include <string>

class Exception : public std::exception {
protected:
    std::string errorMessage = "An exception has occurred";

public:
    Exception() = default;
    Exception(std::string);
    std::string getMessage();
};
