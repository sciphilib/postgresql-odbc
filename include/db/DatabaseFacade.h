#pragma once

#include "MainFactory.h"
#include "WeekdayTDG.h"

class DatabaseFacade
{
public:
    DatabaseFacade(SQLHDBC hDbc) : hDbc_(hDbc) {}
    void addWeekday(const std::string& dayName);

private:
    SQLHDBC hDbc_;
    MainFactory mainFactory;
};