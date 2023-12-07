#pragma once
#include "BaseObject.h"
#include <algorithm>
#include <array>
#include <stdexcept>
#include <string>

class Weekday : public BaseObject
{
public:
    Weekday(int id, int day) : id_(id), day_(day)
    {
        if (day < 1 || day > 7)
        {
            throw std::invalid_argument("Invalid number for dayweek");
        }
    }

    Weekday(int day) : id_(-1), day_(day)
    {
        if (day < 1 || day > 7)
        {
            throw std::invalid_argument("Invalid number for dayweek");
        }
    }

    Weekday(int id, const std::string& dayName) : id_(id)
    {
        auto it = std::find(DAYS.begin(), DAYS.end(), dayName);
        if (it != DAYS.end())
        {
            day_ = std::distance(DAYS.begin(), it) + 1;
        }
        else
        {
            throw std::invalid_argument("Invalid name for dayweek");
        }
    }

    Weekday(const std::string& dayName) : id_(-1)
    {
        auto it = std::find(DAYS.begin(), DAYS.end(), dayName);
        if (it != DAYS.end())
        {
            day_ = std::distance(DAYS.begin(), it) + 1;
        }
        else
        {
            throw std::invalid_argument("Invalid name for dayweek");
        }
    }

    int toInt() const { return day_; }
    std::string toString() const { return DAYS[day_ - 1]; }
    void print() const override
    {
        std::cout << "Weekday: id = " << id_ << ", day = " << day_ << std::endl;
    }

private:
    int id_;
    int day_;

    inline static const std::array<std::string, 7> DAYS = {
        "Monday", "Tuesday",  "Wednesday", "Thursday",
        "Friday", "Saturday", "Sunday"};
};