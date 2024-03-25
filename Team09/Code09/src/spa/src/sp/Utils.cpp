
#include "sp/Utils.h"

#include <memory>
#include <string>

#include "sp/PatternTreeNode.h"
std::string getLexicalEnumString(LEXICAL_TOKEN_TYPE type) {
    if (auto result = LEXICAL_ENUM_STRINGS.find(type); result != LEXICAL_ENUM_STRINGS.end()) {
        return result->second;
    }
    return UNIDENTIFIED_LEXICAL_STRING;
}
/*
s1 - SP side traversal
s2 - QPS side query string
This function checks whether s1 ==s2 by first creating a PatternTreeNode for s2.
Then, it deserialises s1 to get a PatternTreeNode for s2.
Finally, it performs comparison and returns the result
*/
bool isExactMatch(std::string spInput, std::string qpsInput) {
    std::shared_ptr<PatternTreeNode> spTree = PatternTreeNode::deserializeToNode(spInput);
    std::shared_ptr<PatternTreeNode> qpsTree = PatternTreeNode::buildTreeFromString(qpsInput);
    return PatternTreeNode::isEqual(spTree, qpsTree);
}

bool isPartialMatch(std::string spInput, std::string qpsInput) {
    std::shared_ptr<PatternTreeNode> spTree = PatternTreeNode::deserializeToNode(spInput);
    std::shared_ptr<PatternTreeNode> qpsTree = PatternTreeNode::buildTreeFromString(qpsInput);
    return PatternTreeNode::isPartiallyEqual(spTree, qpsTree);
}
