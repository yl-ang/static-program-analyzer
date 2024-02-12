#include <exception>
#include <string>

class Exception : public std::exception {
protected:
    std::string errorMessage = "An exception has occurred";

public:
    Exception();
    Exception(std::string);
    std::string getMessage();
};
