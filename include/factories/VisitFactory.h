#pragma once

#include "DateTime.h"
#include "IFactory.h"
#include "Visit.h"
#include <memory>

class VisitFactory
{
public:
    std::unique_ptr<BaseObject>
    createObject(const std::map<std::string, std::string>& parameters)
    {
        int idPatient, idDoctor;
        DateTime dateVisit(false), dateDischarge(false), dateClose(false);
        std::string complaints;

        if (parameters.find("id_patient") != parameters.end())
        {
            idPatient = std::stoi(parameters.at("id_patient"));
        }
        else
        {
            std::cout << "Error: there is no id_patient in parameters of Visit"
                      << std::endl;
            return nullptr;
        }

        if (parameters.find("id_doctor") != parameters.end())
        {
            idDoctor = std::stoi(parameters.at("id_doctor"));
        }
        else
        {
            std::cout << "Error: there is no id_doctor in parameters of Visit"
                      << std::endl;
            return nullptr;
        }

        if (parameters.find("date_visit") != parameters.end())
        {
            std::string date = parameters.at("date_visit");
            dateVisit.setDateTime(date);
        }
        else
        {
            std::cout << "Error: there is no date_visit in parameters of Visit"
                      << std::endl;
            return nullptr;
        }

        if (parameters.find("complaints") != parameters.end())
        {
            complaints = parameters.at("complaints");
        }
        else
        {
            std::cout << "Error: there is no complaints in parameters of Visit"
                      << std::endl;
            return nullptr;
        }

        if (parameters.find("date_discharge") != parameters.end())
        {
            std::string date = parameters.at("date_discharge");
            dateDischarge.setDateTime(date);
        }
        else
        {
            std::cout
                << "Error: there is no date_discharge in parameters of Visit"
                << std::endl;
            return nullptr;
        }

        if (parameters.find("date_close") != parameters.end())
        {
            std::string date = parameters.at("date_close");
            dateClose.setDateTime(date);
        }
        else
        {
            std::cout << "Error: there is no date_close in parameters of Visit"
                      << std::endl;
            return nullptr;
        }

        return std::make_unique<Visit>(idPatient, idDoctor, complaints,
                                       dateVisit, dateDischarge, dateClose);
    }
};