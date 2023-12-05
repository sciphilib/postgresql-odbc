#pragma once

#include "IFactory.h"
#include "Medication.h"
#include <memory>

class MedicationFactory
{
public:
    std::unique_ptr<BaseObject>
    createObject(const std::map<std::string, std::string>& parameters)
    {
        std::string name;

        if (parameters.find("name") != parameters.end())
        {
            name = parameters.at("name");
            return std::make_unique<Medication>(name);
        }

        return nullptr;
    }
};