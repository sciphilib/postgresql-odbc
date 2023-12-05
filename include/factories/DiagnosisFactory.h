#pragma once

#include "Diagnosis.h"
#include "IFactory.h"
#include <memory>

class DiagnosisFactory
{
public:
    std::unique_ptr<BaseObject>
    createObject(const std::map<std::string, std::string>& parameters)
    {
        int idVisit;
        std::string description;

        if (parameters.find("id_visit") != parameters.end())
        {
            idVisit = std::stoi(parameters.at("id_visit"));
        }
        else
        {
            std::cout
                << "Error: there is no id_visit in parameters of Diagnosis"
                << std::endl;
            return nullptr;
        }

        if (parameters.find("description") != parameters.end())
        {
            description = parameters.at("description");
        }
        else
        {
            std::cout
                << "Error: there is no description in parameters of Diagnosis"
                << std::endl;
            return nullptr;
        }

        return std::make_unique<Diagnosis>(idVisit, description);
    }
};