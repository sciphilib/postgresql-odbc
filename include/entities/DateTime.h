#pragma once
#include "BaseObject.h"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>

class DateTime : public BaseObject
{
public:
    DateTime(bool isOnlyTime);
    DateTime(const std::string& dateTimeStr, bool isOnlyTime);
    DateTime(int year, int month, int day, int hours, int minutes);
    DateTime(int hours, int minutes);

    std::string toString() const;
    std::string toTimeString() const;
    void setDateTime(const std::string& dateTimeStr);
    void print() const override
    {
        std::cout << "DateTime: date_ = " << std::asctime(&date_) << std::endl;
    }

private:
    bool isOnlyTime_;
    std::tm date_;
    int year_;
    int month_;
    int day_;
    int hours_;
    int minutes_;
};