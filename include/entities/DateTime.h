#pragma once
#include "BaseObject.h"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>

class DateTime : public BaseObject
{
public:
    DateTime();
    DateTime(const std::string& datetimeStr);
    std::string toString() const;
    void print() const override
    {
        std::cout << "DateTime: date_ = " << std::asctime(&date_) << std::endl;
    }

private:
    std::tm date_;
};