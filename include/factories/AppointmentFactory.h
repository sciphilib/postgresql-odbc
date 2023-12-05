#pragma once

#include "Appointment.h"
#include "DateTime.h"
#include "IFactory.h"
#include <memory>

class AppointmentFactory
{
public:
    std::unique_ptr<BaseObject>
    createObject(const std::map<std::string, std::string>& parameters)
    {
        int idDoctor, idWeekday, office, district;
        DateTime beginDate(true), endDate(true);

        if (parameters.find("idDoctor") != parameters.end())
        {
            idDoctor = std::stoi(parameters.at("idDoctor"));
        }
        else
        {
            std::cout
                << "Error: there is no idDoctor in parameters of Appointment"
                << std::endl;
            return nullptr;
        }

        if (parameters.find("idWeekday") != parameters.end())
        {
            idWeekday = std::stoi(parameters.at("idWeekday"));
        }
        else
        {
            std::cout
                << "Error: there is no idWeekday in parameters of Appointment"
                << std::endl;
            return nullptr;
        }

        if (parameters.find("office") != parameters.end())
        {
            office = std::stoi(parameters.at("office"));
        }
        else
        {
            std::cout
                << "Error: there is no office in parameters of Appointment"
                << std::endl;
            return nullptr;
        }

        if (parameters.find("district") != parameters.end())
        {
            district = std::stoi(parameters.at("district"));
        }
        else
        {
            std::cout
                << "Error: there is no district in parameters of Appointment"
                << std::endl;
            return nullptr;
        }

        if (parameters.find("beginDate") != parameters.end())
        {
            std::string beginDateStr = parameters.at("beginDate");
            beginDate.setDateTime(beginDateStr);
        }
        else
        {
            std::cout
                << "Error: there is no beginDate in parameters of Appointment"
                << std::endl;
        }

        if (parameters.find("endDate") != parameters.end())
        {
            std::string endDateStr = parameters.at("endDate");
            endDate.setDateTime(endDateStr);
        }
        else
        {
            std::cout
                << "Error: there is no endDate in parameters of Appointment"
                << std::endl;
        }

        return std::make_unique<Appointment>(idDoctor, idWeekday, beginDate,
                                             endDate, office, district);
    }
};