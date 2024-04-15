#include "GrammarUtils.h"

bool isIdent(const std::string& str) {
    return std::regex_match(str, std::regex("^[a-zA-Z][a-zA-Z0-9]*$"));
}

bool isName(const std::string& str) {
    return isIdent(str);
}

bool isInteger(const std::string& str) {
    return std::regex_match(str, std::regex("^(?:0|[1-9][0-9]*)$"));
}

bool isSynonym(const std::string& str) {
    return isIdent(str);
}

bool isWildcard(const std::string& str) {
    return std::regex_match(str, std::regex("^_$"));
}

bool isQuotedIdent(const std::string& str) {
    if (str.empty()) {
        return false;
    }
    bool startsAndEndsWithQuotes = std::regex_match(str, std::regex("^\".*\"$"));
    std::string ident = trim(str.substr(1, str.size() - 2));
    return startsAndEndsWithQuotes && isIdent(ident);
}

bool isStmtRef(const std::string& str) {
    return isSynonym(str) || isWildcard(str) || isInteger(str);
}

bool isEntRef(const std::string& str) {
    return isSynonym(str) || isWildcard(str) || isQuotedIdent(str);
}

bool isExpressionSpec(const std::string& str) {
    if (isWildcard(str)) {
        return true;
    }

    std::string removedUnderscoreString = str;
    if (isPartialExpression(str)) {
        removedUnderscoreString = trim(str.substr(1, str.size() - 2));
    }
    return isQuotedExpr(removedUnderscoreString);
}

bool isPartialExpression(const std::string& str) {
    return std::regex_match(str, std::regex("^_.*_$"));
}

bool isQuotedExpr(const std::string& str) {
    bool startsAndEndsWithQuotes = std::regex_match(str, std::regex("^\".*\"$"));
    if (startsAndEndsWithQuotes) {
        std::string removedQuoteString = trim(str.substr(1, str.size() - 2));
        return isExpr(removedQuoteString);
    }

    return false;
}

bool isExpr(const std::string& str) {
    if (str.empty()) {
        return false;
    }

    int index = -1;
    int bracketCounter = 0;
    for (int i = str.size() - 1; i >= 0; i--) {
        if ((str[i] == '+' || str[i] == '-') && bracketCounter == 0) {
            index = i;
            break;
        }
        if (str[i] == ')') {
            bracketCounter++;
        }
        if (str[i] == '(') {
            bracketCounter--;
        }
        // Open bracket before close bracket (since we going from the back)
        if (bracketCounter < 0) {
            return false;
        }
    }

    // Too many close brackets
    if (bracketCounter > 0) {
        return false;
    }

    // +/- not found, might be term
    if (index == -1) {
        return isTerm(str);
    }

    std::string expr = trim(str.substr(0, index));
    std::string term = trim(str.substr(index + 1, str.size() - index - 1));
    return isExpr(expr) && isTerm(term);
}

bool isTerm(const std::string& str) {
    if (str.empty()) {
        return false;
    }

    int index = -1;
    int bracketCounter = 0;
    for (int i = str.size() - 1; i >= 0; i--) {
        if ((str[i] == '*' || str[i] == '/' || str[i] == '%') && bracketCounter == 0) {
            index = i;
            break;
        }
        if (str[i] == ')') {
            bracketCounter++;
        }
        if (str[i] == '(') {
            bracketCounter--;
        }
        // Open bracket before close bracket (since we going from the back)
        if (bracketCounter < 0) {
            return false;
        }
    }

    // Too many close brackets
    if (bracketCounter > 0) {
        return false;
    }

    // +/- not found, might be factor
    if (index == -1) {
        return isFactor(str);
    }

    std::string term = trim(str.substr(0, index));
    std::string factor = trim(str.substr(index + 1, str.size() - index - 1));
    return isTerm(term) && isFactor(factor);
}

bool isFactor(const std::string& str) {
    std::string trimmedString = trim(str);
    if (isName(trimmedString) || isInteger(trimmedString)) {
        return true;
    }

    bool startsAndEndsWithBrackets = std::regex_match(str, std::regex("^\\(.*\\)$"));
    if (startsAndEndsWithBrackets) {
        std::string expr = trim(trimmedString.substr(1, trimmedString.size() - 2));
        return isExpr(expr);
    }
    return false;
}

bool isSelectStatement(const std::string& str) {
    return std::regex_search(str, std::regex("^" + QPSConstants::SELECT));
}

bool isDeclarationStatement(const std::string& str) {
    std::string pattern = "^(" + QPSConstants::DESIGN_ENTITIES + ")";
    return std::regex_search(str, std::regex(pattern));
}

bool isBoolean(const std::string& str) {
    return trim(str) == QPSConstants::BOOLEAN;
}

bool isTuple(const std::string& str) {
    bool startsAndEndsWithAngularBrackets = std::regex_match(str, std::regex("^<.*>$"));
    std::vector<std::string> elems = {};
    std::string removedBracketsString;
    if (startsAndEndsWithAngularBrackets) {
        removedBracketsString = str.substr(1, str.size() - 2);
        elems = splitByDelimiter(removedBracketsString, ",");
    } else {
        elems.push_back(str);
    }

    for (std::string elem : elems) {
        if (!isElem(elem)) {
            return false;
        }
    }
    return true;
}

bool isElem(const std::string& str) {
    return isSynonym(str) || isAttrRef(str);
}

bool isAttrCompare(const std::string& attrComp) {
    std::vector<std::string> expressions = splitByDelimiter(attrComp, "=");
    if (expressions.size() != 2) {
        return false;
    }
    return isRef(expressions[0]) && isRef(expressions[1]);
}

bool isRef(const std::string& ref) {
    return isQuotedIdent(ref) || isInteger(ref) || isAttrRef(ref);
}

bool isAttrRef(const std::string& attrRef) {
    std::vector<std::string> attrs = splitByDelimiter(attrRef, ".");
    if (attrs.size() != 2) {
        return false;
    }
    return isSynonym(attrs[0]) && isAttrName(attrs[1]);
}

bool isAttrName(const std::string& attrName) {
    return std::regex_search(attrName, std::regex("^(" + QPSConstants::ATTR_NAME + ")"));
}

bool isNotRelation(const std::string& notRelation) {
    return std::regex_search(notRelation, std::regex(QPSConstants::NOT));
}
