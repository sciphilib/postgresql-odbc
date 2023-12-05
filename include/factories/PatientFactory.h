#pragma once

#include "IFactory.h"
#include "Patient.h"
#include <memory>

class PatientFactory
{
public:
    std::unique_ptr<BaseObject>
    createObject(const std::map<std::string, std::string>& parameters)
    {
        std::string lastName, firstName, middleName, address;

        if (parameters.find("lastName") != parameters.end())
        {
            lastName = parameters.at("lastName");
        }
        else
        {
            std::cout << "Error: there is no lastName in parameters of Patient"
                      << std::endl;
            return nullptr;
        }

        if (parameters.find("firstName") != parameters.end())
        {
            firstName = parameters.at("firstName");
        }
        else
        {
            std::cout << "Error: there is no firstName in parameters of Patient"
                      << std::endl;
            return nullptr;
        }

        if (parameters.find("middleName") != parameters.end())
        {
            middleName = parameters.at("middleName");
        }
        else
        {
            std::cout << "Error: there is no firstName in parameters of Patient"
                      << std::endl;
            return nullptr;
        }

        if (parameters.find("address") != parameters.end())
        {
            address = parameters.at("address");
        }
        else
        {
            std::cout << "Error: there is no address in parameters of Patient"
                      << std::endl;
            return nullptr;
        }

        return std::make_unique<Patient>(lastName, firstName, middleName,
                                         address);
    }
};