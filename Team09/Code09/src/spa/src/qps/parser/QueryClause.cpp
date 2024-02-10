#include "QueryClause.h"

// Implementation of SelectClause member functions
SelectClause::SelectClause(const QueryEntity &entity) : entity(entity) {}

std::string SelectClause::getType() const {
    return "SELECT";
}

QueryEntity SelectClause::getEntity() const {
    return entity;
}