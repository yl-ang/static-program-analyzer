#include "Exception.h"

class SemanticError : public Exception {
public:
    SemanticError();
    SemanticError(std::string);
};
