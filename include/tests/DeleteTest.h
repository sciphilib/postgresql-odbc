#pragma once

#include "DatabaseTest.h"

class DeleteTest : public DatabaseTest
{
public:
    DeleteTest(DatabaseFacade* databaseFacade) : DatabaseTest(databaseFacade) {}
    void start() override { deleteDoctor(); }

private:
    void deleteDoctor()
    {
        int id = 1;
        if (databaseFacade->deleteDoctor(id))
        {
            std::cout << "SUCCESS: Doctor with id  = " << id << " was deleted\n";
        }
        else
        {
            std::cout << "FAIL: Doctor with id  = " << id << " was not deleted\n";
        }
    }
};