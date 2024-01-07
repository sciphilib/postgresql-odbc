#pragma once

#include "DatabaseFacade.h"

class DatabaseTest
{
public:
    DatabaseTest(DatabaseFacade* databaseFacade) : databaseFacade(databaseFacade) {}
    virtual void start() = 0;

protected:
    DatabaseFacade* databaseFacade;
};