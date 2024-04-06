#pragma once

#include "pkb/PKBClient/PKBFacadeReader.h"
#include "qps/clauseArguments/Synonym.h"

class AttributeCollector {
public:
    static std::string collect(PKBFacadeReader& pkb, const Synonym& synonym, const std::string& value) {
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

private:
    static std::string collectCallProcName(PKBFacadeReader& pkb, std::string value) {
        std::optional procedureName = pkb.getCallFromStmtNum(std::stoi(value));
        if (procedureName.has_value()) {
            return procedureName.value();
        }
        throw Exception("No procedure name found for Call Statement");
    }

    static std::string collectReadVarName(PKBFacadeReader& pkb, std::string value) {
        const auto& variables = pkb.getModifiesVariablesByStatement(std::stoi(value));
        if (variables.size() != 1) {
            throw Exception("None or more than 1 variable found when reading Read Statement");
        }
        const auto& variable = *variables.begin();
        return variable;
    }

    static std::string collectPrintVarName(PKBFacadeReader& pkb, std::string value) {
        const auto& variables = pkb.getUsesVariablesByStatement(std::stoi(value));
        if (variables.size() != 1) {
            throw Exception("None or more than 1 variable found when reading Print Statement");
        }
        const auto& variable = *variables.begin();
        return variable;
    }
};
