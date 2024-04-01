#include <vector>

#include "AssignPattern.h"
#include "IfPattern.h"
#include "Pattern.h"
#include "WhilePattern.h"
#include "qps/clauseArguments/Synonym.h"

class PatternBuilder {
public:
    static std::shared_ptr<Pattern> buildPattern(DesignEntityType, std::shared_ptr<ClauseArgument>,
                                                 std::vector<std::shared_ptr<ClauseArgument>>);
};
