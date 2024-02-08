#include "ParserUtils.h"

std::string trim(std::string& str) {
    size_t start = str.find_first_not_of(WHITESPACES);
    if (start != std::string::npos) {
        str = str.substr(start);
    }

    size_t end = str.find_last_not_of(WHITESPACES);
    if (end != std::string::npos) {
        str = str.substr(0, end + 1);
    }

    return str;
}
