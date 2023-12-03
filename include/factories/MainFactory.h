#pragma once

#include "BaseObject.h"
#include "IFactory.h"
#include "WeekdayFactory.h"
#include <memory>

class MainFactory : public IFactory
{
public:
    virtual std::unique_ptr<BaseObject>
    createObject(const std::string& type,
                 const std::map<std::string, std::string>& parameters) override
    {
        if (type == "Weekday")
        {
            WeekdayFactory weekdayFactory;
            return weekdayFactory.createObject(parameters);
        }

        return nullptr;
    }
};