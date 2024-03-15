#pragma once

#include "SelectClauseArgument.h"

class SingleReturn : public SelectClauseArgument {
private:
    std::vector<Synonym> synonyms;

public:
    SingleReturn(const Synonym& s);
    bool isSingleReturn() const override;
    std::vector<Synonym> getValue() const;
    std::string getSelectClauseType() const override;
    bool operator==(const SelectClauseArgument& other) const override;
};
