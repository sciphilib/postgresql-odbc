#include "DateTime.h"

#include <sstream>

DateTime::DateTime(bool isOnlyTime) : isOnlyTime_(isOnlyTime)
{
    auto now = std::chrono::system_clock::now();
    auto timeNow = std::chrono::system_clock::to_time_t(now);
    date_ = *std::localtime(&timeNow);
}

DateTime::DateTime(const std::string& dateTimeStr, bool isOnlyTime)
    : isOnlyTime_(isOnlyTime)
{
    std::istringstream stream(dateTimeStr);
    if (!isOnlyTime_)
    {
        stream >> std::get_time(&date_, "%Y-%m-%d %H:%M");
    }
    else
    {
        char delimiter;

        if (!(stream >> hours_ >> delimiter >> minutes_) || delimiter != ':' ||
            !stream.eof())
        {
            throw std::invalid_argument("Error: incorrect time format");
        }

        if (hours_ < 0 || hours_ > 23 || minutes_ < 0 || minutes_ > 59)
        {
            throw std::invalid_argument("Error: incorrect time format");
        }
    }
}

DateTime::DateTime(int year, int month, int day, int hours, int minutes)
    : year_(year), month_(month), day_(day), hours_(hours), minutes_(minutes)
{
}

DateTime::DateTime(int hours, int minutes)
    : year_(0), month_(0), day_(0), hours_(hours), minutes_(minutes)
{
}

std::string DateTime::toString() const
{
    std::ostringstream stream;
    if (!isOnlyTime_)
    {
        stream << std::put_time(&date_, "%Y-%m-%d %H:%M");
    }
    else
    {
        stream << std::setw(2) << std::setfill('0') << hours_ << ":"
               << std::setw(2) << std::setfill('0') << minutes_;
    }
    return stream.str();
}

std::string DateTime::toTimeString() const
{
    std::ostringstream stream;
    stream << std::setw(2) << std::setfill('0') << hours_ << ":" << std::setw(2)
           << std::setfill('0') << minutes_;
    return stream.str();
}

void DateTime::setDateTime(const std::string& dateTimeStr)
{
    std::istringstream stream(dateTimeStr);
    if (!isOnlyTime_)
    {
        stream >> std::get_time(&date_, "%Y-%m-%d %H:%M");
    }
    else
    {
        char delimiter;

        if (!(stream >> hours_ >> delimiter >> minutes_) || delimiter != ':' ||
            !stream.eof())
        {
            throw std::invalid_argument("Error: incorrect time format");
        }

        if (hours_ < 0 || hours_ > 23 || minutes_ < 0 || minutes_ > 59)
        {
            throw std::invalid_argument("Error: incorrect time format");
        }
    }
}