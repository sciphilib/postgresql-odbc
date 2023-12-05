#pragma once

#include "IFactory.h"
#include "Procedure.h"
#include <memory>

class ProcedureFactory
{
public:
    std::unique_ptr<BaseObject>
    createObject(const std::map<std::string, std::string>& parameters)
    {
        std::string name;

        if (parameters.find("name") != parameters.end())
        {
            name = parameters.at("name");
            return std::make_unique<Procedure>(name);
        }

        return nullptr;
    }
};