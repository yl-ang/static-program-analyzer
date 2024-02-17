#include "Exception.h"

class SyntaxError : public Exception {
public:
    SyntaxError();
    SyntaxError(std::string);
};