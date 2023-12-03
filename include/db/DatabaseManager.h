#pragma once
#include <iostream>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>

#include "DatabaseFacade.h"
#include "Parser.h"

class DatabaseManager
{
public:
    DatabaseManager(const std::string& dsn, const std::string& user,
                    const std::string& pass);
    ~DatabaseManager();

private:
    SQLHENV hEnv;
    SQLHDBC hDbc;
    SQLHSTMT hStmt;
    std::string dsn_, user_, pass_;
    void connect();
    void initDatabase();

private:
    void createTableAll();
    void createTableWeekdays();
    void createTableSpecializations();
    void createTableDoctors();
    void createTableAppointments();
    void createTablePatients();
    void createTableVisits();
    void createTableMedications();
    void createTablePrescribedMedications();
    void createTableProcedures();
    void createTablePrescribedProcedures();
    void createTableTests();
    void createTableTestsResults();
    void createTableDiagnosis();
};