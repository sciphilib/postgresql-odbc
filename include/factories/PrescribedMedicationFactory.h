#pragma once

#include "IFactory.h"
#include "PrescribedMedication.h"
#include <memory>

class PrescribedMedicationFactory
{
public:
    std::unique_ptr<BaseObject>
    createObject(const std::map<std::string, std::string>& parameters)
    {
        int idVisit, idMedication;

        if (parameters.find("id_visit") != parameters.end())
        {
            idVisit = std::stoi(parameters.at("id_visit"));
        }
        else
        {
            std::cout << "Error: there is no id_visit in parameters of "
                         "PrescribedMedication"
                      << std::endl;
            return nullptr;
        }

        if (parameters.find("id_medication") != parameters.end())
        {
            idMedication = std::stoi(parameters.at("id_medication"));
        }
        else
        {
            std::cout << "Error: there is no id_medication in parameters of "
                         "PrescribedMedication"
                      << std::endl;
            return nullptr;
        }

        return std::make_unique<PrescribedMedication>(idVisit, idMedication);
    }
};