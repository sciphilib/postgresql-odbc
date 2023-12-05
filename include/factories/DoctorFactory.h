#pragma once

#include "Doctor.h"
#include "IFactory.h"
#include <memory>

class DoctorFactory
{
public:
    std::unique_ptr<BaseObject>
    createObject(const std::map<std::string, std::string>& parameters)
    {
        std::string lastName, firstName, middleName;
        int idSpec;

        if (parameters.find("lastName") != parameters.end())
        {
            lastName = parameters.at("lastName");
        }
        else
        {
            std::cout << "Error: there is no lastName in parameters of Doctor"
                      << std::endl;
            return nullptr;
        }

        if (parameters.find("firstName") != parameters.end())
        {
            firstName = parameters.at("firstName");
        }
        else
        {
            std::cout << "Error: there is no firstName in parameters of Doctor"
                      << std::endl;
            return nullptr;
        }

        if (parameters.find("middleName") != parameters.end())
        {
            middleName = parameters.at("middleName");
        }
        else
        {
            std::cout << "Error: there is no firstName in parameters of Doctor"
                      << std::endl;
            return nullptr;
        }

        if (parameters.find("idSpec") != parameters.end())
        {
            idSpec = std::stoi(parameters.at("idSpec"));
        }
        else
        {
            std::cout << "Error: there is no idSpec in parameters of Doctor"
                      << std::endl;
            return nullptr;
        }

        return std::make_unique<Doctor>(lastName, firstName, middleName,
                                        idSpec);
    }
};