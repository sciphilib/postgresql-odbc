#pragma once

#include "IFactory.h"
#include "PrescribedProcedure.h"
#include <memory>

class PrescribedProcedureFactory
{
public:
    std::unique_ptr<BaseObject>
    createObject(const std::map<std::string, std::string>& parameters)
    {
        int idVisit, idProcedure, count;

        if (parameters.find("id_visit") != parameters.end())
        {
            idVisit = std::stoi(parameters.at("id_visit"));
        }
        else
        {
            std::cout << "Error: there is no id_visit in parameters of "
                         "PrescribedProcedure"
                      << std::endl;
            return nullptr;
        }

        if (parameters.find("id_procedure") != parameters.end())
        {
            idProcedure = std::stoi(parameters.at("id_procedure"));
        }
        else
        {
            std::cout << "Error: there is no id_procedure in parameters of "
                         "PrescribedProcedure"
                      << std::endl;
            return nullptr;
        }

        if (parameters.find("count") != parameters.end())
        {
            count = std::stoi(parameters.at("count"));
        }
        else
        {
            std::cout << "Error: there is no count in parameters of "
                         "PrescribedProcedure"
                      << std::endl;
            return nullptr;
        }

        return std::make_unique<PrescribedProcedure>(idVisit, idProcedure,
                                                     count);
    }
};