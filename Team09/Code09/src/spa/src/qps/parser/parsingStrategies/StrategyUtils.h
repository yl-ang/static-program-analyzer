#pragma once

#include <vector>
#include <string>

#include "qps/clauseArguments/Synonym.h"

static std::vector<std::string> cleanParameters(const std::string&);
static Synonym * buildSynonym(const std::vector<Synonym>& entities,const std::string& str);
