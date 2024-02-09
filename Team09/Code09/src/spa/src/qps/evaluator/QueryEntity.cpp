#include "QueryEntity.h"

QueryEntity::QueryEntity(const EntityType& t, const std::string& n) : type(t), name(n) {}

EntityType QueryEntity::getType() const {
    return type;
}

std::string QueryEntity::getName() const {
    return name;
}

bool QueryEntity::operator==(const QueryEntity& other) const {
    return type == other.type && name == other.name;
}

QueryEntity QueryEntity::createVariable(const std::string& name) {
    return QueryEntity{ EntityType::VARIABLE, name };
}

QueryEntity QueryEntity::createConstant(const std::string& name) {
    return QueryEntity{ EntityType::CONSTANT, name };
}

QueryEntity QueryEntity::createProcedure(const std::string& name) {
    return QueryEntity{ EntityType::PROCEDURE, name };
}
