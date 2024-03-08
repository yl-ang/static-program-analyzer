#pragma once

#include <vector>
#include <string>

#include "qps/clauseArguments/Synonym.h"

std::vector<std::string> cleanParameters(const std::string&);
Synonym* buildSynonym(const std::vector<Synonym>& entities,const std::string& str);
