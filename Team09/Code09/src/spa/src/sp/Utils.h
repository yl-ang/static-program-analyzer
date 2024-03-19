#include <string>

#include "sp/tokenizer/Token.h"

std::string getLexicalEnumString(LEXICAL_TOKEN_TYPE type);
bool isExactMatch(std::string s1, std::string s2);
bool isPartialMatch(std::string s1, std::string s2);
