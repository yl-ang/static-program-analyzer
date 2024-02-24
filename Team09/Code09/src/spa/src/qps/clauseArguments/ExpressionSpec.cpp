#include "ExpressionSpec.h"

ExpressionSpec::ExpressionSpec(const std::string& v) : value(v) {}

std::string ExpressionSpec::getValue() const {
    return value;
}

bool ExpressionSpec::isExpressionSpec() const {
    return true;
}

std::string ExpressionSpec::getClauseType() const {
    return "ExpressionSpec";
}

bool ExpressionSpec::operator==(const ClauseArgument& other) const {
    if (!other.isExpressionSpec()) {
        return false;
    }
    return this->getValue() == other.getValue();
}
