#include "DatabaseFacade.h"

void DatabaseFacade::addWeekday(const std::string& dayName)
{
    std::map<std::string, std::string> parameters{{"dayName", dayName}};

    auto weekday = mainFactory.createObject("Weekday", parameters);

    WeekdayTDG weekdayTDG(hDbc_);
    weekdayTDG.insert(*weekday);

    // to do: use get method to get and assign id of object
}
