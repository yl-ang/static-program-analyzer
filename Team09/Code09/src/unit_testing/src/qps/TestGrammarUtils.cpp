#include "catch.hpp"
#include "qps/GrammarUtils.h"

// TODO(Han Qin): Test for all utils functions.
TEST_CASE("Grammar Utils") {
    // check ident
    REQUIRE(isIdent("Capitalize"));
    REQUIRE(isIdent("camelCase"));
    REQUIRE(isIdent("UPPER"));
    REQUIRE(isIdent("lower"));
    REQUIRE(isIdent("withNumbers091"));
    REQUIRE(isIdent("x"));

    REQUIRE_FALSE(isIdent("010startingWithNumbers"));
    REQUIRE_FALSE(isIdent("with space"));
    REQUIRE_FALSE(isIdent("with_underscore"));
    REQUIRE_FALSE(isIdent("with\"Quotes\""));
    REQUIRE_FALSE(isIdent("\"startsWithQuotes\""));
    REQUIRE_FALSE(isIdent(""));

    // check integer
    REQUIRE(isInteger("0"));
    REQUIRE(isInteger("1"));
    REQUIRE(isInteger("1000"));
    REQUIRE(isInteger("9848723"));

    REQUIRE_FALSE(isInteger("9,848,723"));
    REQUIRE_FALSE(isInteger("9 848 723"));
    REQUIRE_FALSE(isInteger("00912914"));
    REQUIRE_FALSE(isInteger("984nn123"));
    REQUIRE_FALSE(isInteger("0000"));

    REQUIRE(isDeclarationStatement("stmt s;"));
    REQUIRE_FALSE(isDeclarationStatement("statement s;"));

    REQUIRE(isSelectStatement("Select v"));
}

TEST_CASE("isExpressionSpec") {
    SECTION("empty string_false") {
        REQUIRE_FALSE(isExpressionSpec(""));
    }

    SECTION("wildcard_true") {
        REQUIRE(isExpressionSpec("_"));
    }

    SECTION("general argument_true") {
        REQUIRE(isExpressionSpec("_\"x+y\"_"));
    }

    SECTION("expr + expr_true") {
        REQUIRE(isExpressionSpec("_\"x+(x+y)\"_"));
    }

    SECTION("no underscore expr_true") {
        REQUIRE(isExpressionSpec("\"cs3203\""));
    }

    SECTION("long complicated expr_true") {
        std::string longExpr = "_\"(x + (y/z)) * (123 / (hello - (good + bye)))\"_";
        REQUIRE(isExpressionSpec(longExpr));
    }

    SECTION("only underscore_false") {
        REQUIRE_FALSE(isExpressionSpec("__"));
    }

    SECTION("mismatch quotation_false") {
        REQUIRE_FALSE(isExpressionSpec("c\""));
    }

    SECTION("mismatch underscore_false") {
        REQUIRE_FALSE(isExpressionSpec("_\"c\""));
    }

    SECTION("expr with empty brackets_false") {
        REQUIRE_FALSE(isExpressionSpec("() + x"));
    }

    SECTION("empty bracket with long expr_false") {
        std::string longExpr = "_\"(x + (y/z)) * (123 / (hello - (good() + bye)))\"_";
        REQUIRE_FALSE(isExpressionSpec(longExpr));
    }
}

TEST_CASE("isExpr") {
    SECTION("factor_true") {
        REQUIRE(isExpr("x"));
    }

    SECTION("term_true") {
        REQUIRE(isExpr("1* y"));
    }

    SECTION("expr + factor_true") {
        REQUIRE(isExpr("x + x + x"));
    }

    SECTION("expr + term_true") {
        REQUIRE(isExpr("x + x * y"));
    }

    SECTION("expr + expr_true") {
        REQUIRE(isExpr("x+(x+x)"));
    }
}

TEST_CASE("isTerm") {
    SECTION("factor_true") {
        REQUIRE(isTerm("x"));
    }

    SECTION("term * var name_true") {
        REQUIRE(isTerm("x*x"));
    }

    SECTION("term / const value_true") {
        REQUIRE(isTerm("x / 1"));
    }

    SECTION("term % var name_true") {
        REQUIRE(isTerm("x %x"));
    }

    SECTION("term * expr_true") {
        REQUIRE(isTerm("x * (x + y)"));
    }

    SECTION("bracket term") {
        REQUIRE(isTerm("(x*y)"));
    }

    SECTION("empty string_false") {
        REQUIRE_FALSE(isTerm(""));
    }

    SECTION("term * expr_false") {
        REQUIRE_FALSE(isTerm("x * (x+y"));
    }
}

TEST_CASE("isFactor") {
    SECTION("var name_true") {
        REQUIRE(isFactor("x"));
    }

    SECTION("const value_true") {
        REQUIRE(isFactor("12"));
    }

    SECTION("term_true") {
        REQUIRE(isFactor("(x * y)"));
    }

    SECTION("expression_plus_true") {
        REQUIRE(isFactor("(x+y)"));
    }

    SECTION("expression_minus_true") {
        REQUIRE(isFactor("(x-y)"));
    }

    SECTION("var name_false") {
        REQUIRE_FALSE(isFactor("(x"));
    }

    SECTION("const value_false") {
        REQUIRE_FALSE(isFactor("1)"));
    }

    SECTION("wildcard_false") {
        REQUIRE_FALSE(isFactor("_"));
    }
}

TEST_CASE("isTuple") {
    SECTION("elem_true") {
        REQUIRE(isTuple("var"));
    }

    SECTION("one elem with angular bracket_true") {
        REQUIRE(isTuple("<var>"));
    }

    SECTION("two elems_true") {
        REQUIRE(isTuple("<var, x>"));
    }

    SECTION("three elems_true") {
        REQUIRE(isTuple("<var, x, y>"));
    }

    SECTION("multiple elems & missing angular bracket_false") {
        REQUIRE_FALSE(isTuple("var, x"));
    }

    SECTION("multiple elems & extra bracket_false") {
        REQUIRE_FALSE(isTuple("<var, x>>"));
    }

    SECTION("multiple elems & one missing bracket_false") {
        REQUIRE_FALSE(isTuple("<var, x"));
    }

    SECTION("multiple elems & extra comma_false") {
        REQUIRE_FALSE(isTuple("<var, x,>"));
    }

    SECTION("multiple elems & missing variable_false") {
        REQUIRE_FALSE(isTuple("<var, ,x>"));
    }

    SECTION("elem and BOOLEAN_true") {
        REQUIRE(isTuple("<var, BOOLEAN>"));
    }
}

TEST_CASE("isElem") {
    SECTION("synonym_true") {
        REQUIRE(isElem("var"));
    }

    SECTION("integer_false") {
        REQUIRE_FALSE(isElem("10"));
    }
}
