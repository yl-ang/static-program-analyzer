#include <qps/validator/Validator.h>

#include <string>
#include <vector>

#include "catch.hpp"

TEST_CASE("isValidSelectStatement") {
    Validator validator;
    validator.validate({"Select va such that Follows(1,2) pattern a(1,2)"});
    REQUIRE(true);
}
