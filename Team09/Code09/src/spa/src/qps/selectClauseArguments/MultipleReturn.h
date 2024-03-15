#pragma once

#include "SelectClauseArgument.h"

class MultipleReturn : public SelectClauseArgument {
private:
    std::vector<Synonym> synonyms;

public:
    MultipleReturn(const std::vector<Synonym>& s);
    bool isMultipleReturn() const override;
    std::vector<Synonym> getValue() const;
    std::string getSelectClauseType() const override;
    bool operator==(const SelectClauseArgument& other) const override;
};
