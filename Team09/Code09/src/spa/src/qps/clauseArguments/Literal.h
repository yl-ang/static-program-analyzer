#include <string>

#include "ClauseArgument.h"

class Literal : public ClauseArgument {
private:
    std::string value;

public:
    Literal(const std::string& v);
    std::string getValue() const;
    bool operator==(const Literal& other) const;
};
