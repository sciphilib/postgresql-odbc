#include "DateTime.h"

#include <sstream>

DateTime::DateTime()
{
    auto now = std::chrono::system_clock::now();
    auto timeNow = std::chrono::system_clock::to_time_t(now);
    date_ = *std::localtime(&timeNow);
}

DateTime::DateTime(const std::string& datetimeStr)
{
    std::istringstream stream(datetimeStr);
    stream >> std::get_time(&date_, "%Y-%m-%d %H:%M");
}

std::string DateTime::toString() const
{
    std::ostringstream stream;
    stream << std::put_time(&date_, "%Y-%m-%d %H:%M");
    return stream.str();
}