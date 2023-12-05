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
    DatabaseFacade databaseFacade;
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
    void createTableTestResults();
    void createTableDiagnosis();

    void insertAll();
    void insertWeekdays();
    void insertSpecializations();
    void insertDoctors();
    void insertAppointments();
    void insertPatients();
    void insertMedications();
    void insertVisits();
    void insertPrescribedMedications();
    void insertProcedures();
    void insertPrescribedProcedures();
    void insertTests();
    void insertTestResults();
    void insertDiagnosis();
};