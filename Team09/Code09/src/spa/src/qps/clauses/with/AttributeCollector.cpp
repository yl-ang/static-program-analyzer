#include "AttributeCollector.h"

std::string AttributeCollector::collect(PKBFacadeReader& pkb, const Synonym& synonym, const std::string& value) {
    const auto& attr = synonym.getAttr();

    if (!attr.has_value()) {
        return value;
    }

    switch (attr.value()) {
    case SynonymAttributeType::PROCNAME:
        if (synonym.getType() == DesignEntityType::CALL) {
            return collectCallProcName(pkb, value);
        }
        return value;

    case SynonymAttributeType::VARNAME:
        if (synonym.getType() == DesignEntityType::READ) {
            return collectReadVarName(pkb, value);
        }
        if (synonym.getType() == DesignEntityType::PRINT) {
            return collectPrintVarName(pkb, value);
        }
        return value;

    case SynonymAttributeType::VALUE:
    case SynonymAttributeType::STMTNUM:
    case SynonymAttributeType::NONE:
        return value;
    }

    return value;
}

std::string AttributeCollector::collectCallProcName(PKBFacadeReader& pkb, std::string value) {
    std::optional procedureName = pkb.getCallFromStmtNum(std::stoi(value));
    if (procedureName.has_value()) {
        return procedureName.value();
    }
    throw QPSMissingProcNameForCallError();
}

std::string AttributeCollector::collectReadVarName(PKBFacadeReader& pkb, std::string value) {
    const auto& variables = pkb.getModifiesVariablesByStatement(std::stoi(value));
    if (variables.size() != 1) {
        throw QPSMissingProcNameForStmtError("Read Statement");
    }
    const auto& variable = *variables.begin();
    return variable;
}

std::string AttributeCollector::collectPrintVarName(PKBFacadeReader& pkb, std::string value) {
    const auto& variables = pkb.getUsesVariablesByStatement(std::stoi(value));
    if (variables.size() != 1) {
        throw QPSMissingProcNameForStmtError("Print Statement");
    }
    const auto& variable = *variables.begin();
    return variable;
}
