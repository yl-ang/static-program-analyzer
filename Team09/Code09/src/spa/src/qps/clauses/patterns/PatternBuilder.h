#include <vector>

#include "AssignPattern.h"
#include "IfPattern.h"
#include "Pattern.h"
#include "WhilePattern.h"
#include "qps/clauseArguments/Synonym.h"

class PatternBuilder {
public:
    static std::unique_ptr<Pattern> PatternBuilder::buildPattern(DesignEntityType, ClauseArgument*,
                                                                 std::vector<ClauseArgument*>);
};
