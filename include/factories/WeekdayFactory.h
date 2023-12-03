#pragma once

#include "IFactory.h"
#include "Weekday.h"
#include <memory>

class WeekdayFactory
{
public:
    std::unique_ptr<BaseObject>
    createObject(const std::map<std::string, std::string>& parameters)
    {
        std::string dayName;
        int day;

        if (parameters.find("dayName") != parameters.end())
        {
            dayName = parameters.at("dayName");
            return std::make_unique<Weekday>(dayName);
        }
        else if (parameters.find("day") != parameters.end())
        {
            day = std::stoi(parameters.at("day"));
            return std::make_unique<Weekday>(day);
        }

        return nullptr;
    }
};
