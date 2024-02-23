#include "ExpressionSpec.h"

ExpressionSpec::ExpressionSpec(const std::string& v) : value(v) {}

std::string ExpressionSpec::getValue() const {
    return value;
}

bool ExpressionSpec::isExpressionSpec() const {
    return true;
}

bool ExpressionSpec::operator==(const ClauseArgument& other) const {
    if (const ExpressionSpec* ptr = dynamic_cast<const ExpressionSpec*>(&other)) {
        return this->getValue() == other.getValue();
    }
    return false;
}
