#include "DatabaseManager.h"
#include "DeleteTest.h"
#include "FindTest.h"
#include "InsertTest.h"
#include "UpdateTest.h"

#include <memory>
#include <sqltypes.h>

DatabaseManager::DatabaseManager(const std::string& dsn,
                                 const std::string& user,
                                 const std::string& pass)
    : dsn_(dsn), user_(user), pass_(pass)
{
    connect();
    initGateways();
    initFacade();
    initDatabase();

    DatabaseMenu menu(databaseFacade.get());
    menu.start();
}

DatabaseManager::~DatabaseManager()
{
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    SQLDisconnect(hDbc);
    SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
    SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
}

void DatabaseManager::connect()
{
    if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv) == SQL_ERROR)
    {
        std::cerr << "Error allocating an environment handle\n";
        exit(-1);
    }

    if (SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0) ==
        SQL_ERROR)
    {
        std::cerr << "Error setting the environment attribute setting\n";
        exit(-1);
    }

    if (SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc) == SQL_ERROR)
    {
        std::cerr << "Error allocating a database connection handle\n";
        exit(-1);
    }

    int retcode = SQLConnect(hDbc, (SQLCHAR*)"PostgreSQLDSN", SQL_NTS,
                             (SQLCHAR*)"sciphilib", SQL_NTS,
                             (SQLCHAR*)"sciphilib", SQL_NTS);
    if (retcode == SQL_ERROR)
    {
        std::cerr << "Error connecting to the data source\n";
        exit(-1);
    }

    if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_ERROR)
    {
        std::cerr << "Error allocating a statement handle\n";
        exit(-1);
    }

    SQLCHAR setEncoding[] = "SET client_encoding TO 'UTF8'";
    SQLExecDirect(hStmt, setEncoding, SQL_NTS);

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void DatabaseManager::initGateways()
{
    appointmentTDG = std::make_unique<AppointmentTDG>(hDbc);
    doctorTDG = std::make_unique<DoctorTDG>(hDbc);
    medicationTDG = std::make_unique<MedicationTDG>(hDbc);
    patientTDG = std::make_unique<PatientTDG>(hDbc);
    prescribedMedicationTDG = std::make_unique<PrescribedMedicationTDG>(hDbc);
    prescribedProcedureTDG = std::make_unique<PrescribedProcedureTDG>(hDbc);
    procedureTDG = std::make_unique<ProcedureTDG>(hDbc);
    specializationTDG = std::make_unique<SpecializationTDG>(hDbc);
    visitTDG = std::make_unique<VisitTDG>(hDbc);
    weekdayTDG = std::make_unique<WeekdayTDG>(hDbc);
    testTDG = std::make_unique<TestTDG>(hDbc);
    testResultTDG = std::make_unique<TestResultTDG>(hDbc);
    diagnosisTDG = std::make_unique<DiagnosisTDG>(hDbc);
}

void DatabaseManager::initDatabase()
{
    createTableAll();
    insertAll();
    findSomeObjects();
    updateAll();
    deleteAll();

    /* Parser parser;
    parser.parseAppointment("data/appointments.json");
    parser.printAppointments(); */
    return;
}

void DatabaseManager::initFacade()
{
    databaseFacade = std::make_unique<DatabaseFacade>(
        appointmentTDG.get(), doctorTDG.get(), medicationTDG.get(),
        patientTDG.get(), prescribedMedicationTDG.get(),
        prescribedProcedureTDG.get(), procedureTDG.get(),
        specializationTDG.get(), visitTDG.get(), weekdayTDG.get(),
        testTDG.get(), testResultTDG.get(), diagnosisTDG.get());
}

void DatabaseManager::findSomeObjects()
{
    std::cout << "~~~~ Table search ~~~~" << std::endl;
    auto findTest = FindTest(databaseFacade.get());
    findTest.start();
}

void DatabaseManager::updateAll()
{
    std::cout << "~~~~ Table updating ~~~~" << std::endl;
    auto updateTest = UpdateTest(databaseFacade.get());
    updateTest.start();
}

void DatabaseManager::deleteAll()
{
    std::cout << "~~~~ Table deleting ~~~~" << std::endl;
    auto deleteTest = DeleteTest(databaseFacade.get());
    deleteTest.start();
}

void DatabaseManager::insertAll()
{
    std::cout << "~~~~ Table inserting ~~~~" << std::endl;
    auto insertTest = InsertTest(databaseFacade.get());
    insertTest.start();
}

void DatabaseManager::createTableAll()
{
    std::cout << "~~~~ Table creating ~~~~" << std::endl;
    createTableWeekdays();
    createTableSpecializations();
    createTableDoctors();
    createTableAppointments();
    createTablePatients();
    createTableVisits();
    createTableMedications();
    createTablePrescribedMedications();
    createTableProcedures();
    createTablePrescribedProcedures();
    createTableTests();
    createTableTestResults();
    createTableDiagnosis();
}

void DatabaseManager::createTableWeekdays()
{
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    SQLCHAR query[] = "SELECT 1 FROM information_schema.tables WHERE "
                      "table_schema = 'public' AND table_name = 'weekdays'";
    SQLRETURN ret = SQLExecDirect(hStmt, query, SQL_NTS);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        if (SQLFetch(hStmt) == SQL_SUCCESS)
        {
            std::cout << "Table 'weekdays' is already exists." << std::endl;
        }
        else
        {
            SQLCHAR createTableQuery[] = "CREATE TABLE weekdays (id SERIAL "
                                         "PRIMARY KEY, name VARCHAR(255))";
            SQLRETURN createRet =
                SQLExecDirect(hStmt, createTableQuery, SQL_NTS);

            if (createRet == SQL_SUCCESS || createRet == SQL_SUCCESS_WITH_INFO)
            {
                std::cout << "Table 'weekdays' was succesfully created."
                          << std::endl;
            }
            else
            {
                std::cerr << "Error while trying to create table 'weekdays'."
                          << std::endl;
            }
        }
    }
    else
    {
        std::cerr << "Error while trying to check if table exists" << std::endl;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void DatabaseManager::createTableSpecializations()
{
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    SQLCHAR query[] =
        "SELECT 1 FROM information_schema.tables WHERE "
        "table_schema = 'public' AND table_name = 'specializations'";
    SQLRETURN ret = SQLExecDirect(hStmt, query, SQL_NTS);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        if (SQLFetch(hStmt) == SQL_SUCCESS)
        {
            std::cout << "Table 'specializations' is already exists."
                      << std::endl;
        }
        else
        {
            SQLCHAR createTableQuery[] =
                "CREATE TABLE specializations (id SERIAL "
                "PRIMARY KEY, name VARCHAR(255))";
            SQLRETURN createRet =
                SQLExecDirect(hStmt, createTableQuery, SQL_NTS);

            if (createRet == SQL_SUCCESS || createRet == SQL_SUCCESS_WITH_INFO)
            {
                std::cout << "Table 'specializations' was succesfully created."
                          << std::endl;
            }
            else
            {
                std::cerr << "Error while trying to create table "
                             "'specializations'."
                          << std::endl;
            }
        }
    }
    else
    {
        std::cerr << "Error while trying to check if table exists" << std::endl;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void DatabaseManager::createTableDoctors()
{
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    SQLCHAR query[] = "SELECT 1 FROM information_schema.tables WHERE "
                      "table_schema = 'public' AND table_name = 'doctors'";
    SQLRETURN ret = SQLExecDirect(hStmt, query, SQL_NTS);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        if (SQLFetch(hStmt) == SQL_SUCCESS)
        {
            std::cout << "Table 'doctors' is already exists." << std::endl;
        }
        else
        {
            SQLCHAR createTableQuery[] =
                "CREATE TABLE doctors (id SERIAL "
                "PRIMARY KEY, last_name VARCHAR(255), first_name "
                "VARCHAR(255), "
                "middle_name VARCHAR(255), id_spec INT REFERENCES "
                "Specializations(id) ON DELETE CASCADE)";
            SQLRETURN createRet =
                SQLExecDirect(hStmt, createTableQuery, SQL_NTS);

            if (createRet == SQL_SUCCESS || createRet == SQL_SUCCESS_WITH_INFO)
            {
                std::cout << "Table 'doctors' was succesfully created."
                          << std::endl;
            }
            else
            {
                std::cerr << "Error while trying to create table 'doctors'."
                          << std::endl;
            }
        }
    }
    else
    {
        std::cerr << "Error while trying to check if table exists" << std::endl;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void DatabaseManager::createTableAppointments()
{
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    SQLCHAR query[] = "SELECT 1 FROM information_schema.tables WHERE "
                      "table_schema = 'public' AND table_name = 'appointments'";
    SQLRETURN ret = SQLExecDirect(hStmt, query, SQL_NTS);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        if (SQLFetch(hStmt) == SQL_SUCCESS)
        {
            std::cout << "Table 'appointments' is already exists." << std::endl;
        }
        else
        {
            SQLCHAR createTableQuery[] =
                "CREATE TABLE appointments (id SERIAL "
                "PRIMARY KEY, id_doctor INT REFERENCES Doctors(id) ON DELETE "
                "CASCADE, "
                "id_weekday "
                "INT REFERENCES weekdays(id) ON DELETE CASCADE, begin_date "
                "TIME, end_date "
                "TIME, "
                "office INT, district INT)";
            SQLRETURN createRet =
                SQLExecDirect(hStmt, createTableQuery, SQL_NTS);

            if (createRet == SQL_SUCCESS || createRet == SQL_SUCCESS_WITH_INFO)
            {
                std::cout << "Table 'appointments' was succesfully created."
                          << std::endl;
            }
            else
            {
                std::cerr
                    << "Error while trying to create table 'appointments'."
                    << std::endl;
            }
        }
    }
    else
    {
        std::cerr << "Error while trying to check if table exists" << std::endl;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void DatabaseManager::createTablePatients()
{
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    SQLCHAR query[] = "SELECT 1 FROM information_schema.tables WHERE "
                      "table_schema = 'public' AND table_name = 'patients'";
    SQLRETURN ret = SQLExecDirect(hStmt, query, SQL_NTS);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        if (SQLFetch(hStmt) == SQL_SUCCESS)
        {
            std::cout << "Table 'patients' is already exists." << std::endl;
        }
        else
        {
            SQLCHAR createTableQuery[] =
                "CREATE TABLE patients (id SERIAL "
                "PRIMARY KEY, last_name VARCHAR(255), first_name "
                "VARCHAR(255), "
                "middle_name VARCHAR(255), address VARCHAR(255))";
            SQLRETURN createRet =
                SQLExecDirect(hStmt, createTableQuery, SQL_NTS);

            if (createRet == SQL_SUCCESS || createRet == SQL_SUCCESS_WITH_INFO)
            {
                std::cout << "Table 'patients' was succesfully created."
                          << std::endl;
            }
            else
            {
                std::cerr << "Error while trying to create table 'patients'."
                          << std::endl;
            }
        }
    }
    else
    {
        std::cerr << "Error while trying to check if table exists" << std::endl;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void DatabaseManager::createTableVisits()
{
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    SQLCHAR query[] = "SELECT 1 FROM information_schema.tables WHERE "
                      "table_schema = 'public' AND table_name = 'visits'";
    SQLRETURN ret = SQLExecDirect(hStmt, query, SQL_NTS);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        if (SQLFetch(hStmt) == SQL_SUCCESS)
        {
            std::cout << "Table 'visits' is already exists." << std::endl;
        }
        else
        {
            SQLCHAR createTableQuery[] =
                "CREATE TABLE visits (id SERIAL "
                "PRIMARY KEY, id_patient INT REFERENCES patients(id) ON DELETE "
                "CASCADE, "
                "id_doctor INT REFERENCES doctors(id) ON DELETE CASCADE, "
                "complaints VARCHAR(255),"
                "date_visit TIMESTAMP WITHOUT TIME ZONE, date_discharge "
                "TIMESTAMP WITHOUT TIME ZONE, date_close TIMESTAMP WITHOUT "
                "TIME ZONE)";
            SQLRETURN createRet =
                SQLExecDirect(hStmt, createTableQuery, SQL_NTS);

            if (createRet == SQL_SUCCESS || createRet == SQL_SUCCESS_WITH_INFO)
            {
                std::cout << "Table 'visits' was succesfully created."
                          << std::endl;
            }
            else
            {
                std::cerr << "Error while trying to create table 'visits'."
                          << std::endl;
            }
        }
    }
    else
    {
        std::cerr << "Error while trying to check if table exists" << std::endl;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void DatabaseManager::createTableMedications()
{
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    SQLCHAR query[] = "SELECT 1 FROM information_schema.tables WHERE "
                      "table_schema = 'public' AND table_name = 'medications'";
    SQLRETURN ret = SQLExecDirect(hStmt, query, SQL_NTS);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        if (SQLFetch(hStmt) == SQL_SUCCESS)
        {
            std::cout << "Table 'medications' is already exists." << std::endl;
        }
        else
        {
            SQLCHAR createTableQuery[] = "CREATE TABLE medications (id SERIAL "
                                         "PRIMARY KEY, name VARCHAR(255))";
            SQLRETURN createRet =
                SQLExecDirect(hStmt, createTableQuery, SQL_NTS);

            if (createRet == SQL_SUCCESS || createRet == SQL_SUCCESS_WITH_INFO)
            {
                std::cout << "Table 'medications' was succesfully created."
                          << std::endl;
            }
            else
            {
                std::cerr << "Error while trying to create table 'medications'."
                          << std::endl;
            }
        }
    }
    else
    {
        std::cerr << "Error while trying to check if table exists" << std::endl;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void DatabaseManager::createTablePrescribedMedications()
{
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    SQLCHAR query[] =
        "SELECT 1 FROM information_schema.tables WHERE "
        "table_schema = 'public' AND table_name = 'prescribed_medications'";
    SQLRETURN ret = SQLExecDirect(hStmt, query, SQL_NTS);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        if (SQLFetch(hStmt) == SQL_SUCCESS)
        {
            std::cout << "Table 'prescribed_medications' is already exists."
                      << std::endl;
        }
        else
        {
            SQLCHAR createTableQuery[] =
                "CREATE TABLE prescribed_medications (id SERIAL "
                "PRIMARY KEY, id_visit INT REFERENCES visits(id) ON DELETE "
                "CASCADE, "
                "id_medication INT REFERENCES medications(id) ON DELETE "
                "CASCADE)";
            SQLRETURN createRet =
                SQLExecDirect(hStmt, createTableQuery, SQL_NTS);

            if (createRet == SQL_SUCCESS || createRet == SQL_SUCCESS_WITH_INFO)
            {
                std::cout << "Table 'prescribed_medications' was "
                             "succesfully created."
                          << std::endl;
            }
            else
            {
                std::cerr << "Error while trying to create table "
                             "'prescribed_medications'."
                          << std::endl;
            }
        }
    }
    else
    {
        std::cerr << "Error while trying to check if table exists" << std::endl;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void DatabaseManager::createTableProcedures()
{
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    SQLCHAR query[] = "SELECT 1 FROM information_schema.tables WHERE "
                      "table_schema = 'public' AND table_name = 'procedures'";
    SQLRETURN ret = SQLExecDirect(hStmt, query, SQL_NTS);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        if (SQLFetch(hStmt) == SQL_SUCCESS)
        {
            std::cout << "Table 'procedures' is already exists." << std::endl;
        }
        else
        {
            SQLCHAR createTableQuery[] = "CREATE TABLE procedures (id SERIAL "
                                         "PRIMARY KEY, name VARCHAR(255))";
            SQLRETURN createRet =
                SQLExecDirect(hStmt, createTableQuery, SQL_NTS);

            if (createRet == SQL_SUCCESS || createRet == SQL_SUCCESS_WITH_INFO)
            {
                std::cout << "Table 'procedures' was succesfully created."
                          << std::endl;
            }
            else
            {
                std::cerr << "Error while trying to create table 'procedures'."
                          << std::endl;
            }
        }
    }
    else
    {
        std::cerr << "Error while trying to check if table exists" << std::endl;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void DatabaseManager::createTablePrescribedProcedures()
{
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    SQLCHAR query[] =
        "SELECT 1 FROM information_schema.tables WHERE "
        "table_schema = 'public' AND table_name = 'prescribed_procedures'";
    SQLRETURN ret = SQLExecDirect(hStmt, query, SQL_NTS);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        if (SQLFetch(hStmt) == SQL_SUCCESS)
        {
            std::cout << "Table 'prescribed_procedures' is already exists."
                      << std::endl;
        }
        else
        {
            SQLCHAR createTableQuery[] =
                "CREATE TABLE prescribed_procedures (id SERIAL "
                "PRIMARY KEY, id_visit INT REFERENCES visits(id) ON DELETE "
                "CASCADE, "
                "id_procedure "
                "INT REFERENCES procedures(id) ON DELETE CASCADE,"
                "count INT)";
            SQLRETURN createRet =
                SQLExecDirect(hStmt, createTableQuery, SQL_NTS);

            if (createRet == SQL_SUCCESS || createRet == SQL_SUCCESS_WITH_INFO)
            {
                std::cout << "Table 'prescribed_medications' was "
                             "succesfully created."
                          << std::endl;
            }
            else
            {
                std::cerr << "Error while trying to create table "
                             "'prescribed_medications'."
                          << std::endl;
            }
        }
    }
    else
    {
        std::cerr << "Error while trying to check if table exists" << std::endl;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void DatabaseManager::createTableTests()
{
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    SQLCHAR query[] = "SELECT 1 FROM information_schema.tables WHERE "
                      "table_schema = 'public' AND table_name = 'tests'";
    SQLRETURN ret = SQLExecDirect(hStmt, query, SQL_NTS);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        if (SQLFetch(hStmt) == SQL_SUCCESS)
        {
            std::cout << "Table 'tests' is already exists." << std::endl;
        }
        else
        {
            SQLCHAR createTableQuery[] = "CREATE TABLE tests (id SERIAL "
                                         "PRIMARY KEY, name VARCHAR(255))";
            SQLRETURN createRet =
                SQLExecDirect(hStmt, createTableQuery, SQL_NTS);

            if (createRet == SQL_SUCCESS || createRet == SQL_SUCCESS_WITH_INFO)
            {
                std::cout << "Table 'tests' was succesfully created."
                          << std::endl;
            }
            else
            {
                std::cerr << "Error while trying to create table 'tests'."
                          << std::endl;
            }
        }
    }
    else
    {
        std::cerr << "Error while trying to check if table exists" << std::endl;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void DatabaseManager::createTableTestResults()
{
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    SQLCHAR query[] = "SELECT 1 FROM information_schema.tables WHERE "
                      "table_schema = 'public' AND table_name = 'test_results'";
    SQLRETURN ret = SQLExecDirect(hStmt, query, SQL_NTS);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        if (SQLFetch(hStmt) == SQL_SUCCESS)
        {
            std::cout << "Table 'test_results' is already exists." << std::endl;
        }
        else
        {
            SQLCHAR createTableQuery[] =
                "CREATE TABLE test_results (id SERIAL "
                "PRIMARY KEY, id_visit INT REFERENCES visits(id) ON DELETE "
                "CASCADE, id_test "
                "INT REFERENCES tests(id) ON DELETE CASCADE, result "
                "VARCHAR(255))";
            SQLRETURN createRet =
                SQLExecDirect(hStmt, createTableQuery, SQL_NTS);

            if (createRet == SQL_SUCCESS || createRet == SQL_SUCCESS_WITH_INFO)
            {
                std::cout << "Table 'test_results' was succesfully created."
                          << std::endl;
            }
            else
            {
                std::cerr << "Error while trying to create table "
                             "'test_results'."
                          << std::endl;
            }
        }
    }
    else
    {
        std::cerr << "Error while trying to check if table exists" << std::endl;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void DatabaseManager::createTableDiagnosis()
{
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    SQLCHAR query[] = "SELECT 1 FROM information_schema.tables WHERE "
                      "table_schema = 'public' AND table_name = 'diagnosis'";
    SQLRETURN ret = SQLExecDirect(hStmt, query, SQL_NTS);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        if (SQLFetch(hStmt) == SQL_SUCCESS)
        {
            std::cout << "Table 'diagnosis' is already exists." << std::endl;
        }
        else
        {
            SQLCHAR createTableQuery[] =
                "CREATE TABLE diagnosis (id SERIAL "
                "PRIMARY KEY, id_visit INT REFERENCES visits(id) ON DELETE "
                "CASCADE, description "
                "VARCHAR(255))";
            SQLRETURN createRet =
                SQLExecDirect(hStmt, createTableQuery, SQL_NTS);

            if (createRet == SQL_SUCCESS || createRet == SQL_SUCCESS_WITH_INFO)
            {
                std::cout << "Table 'diagnosis' was succesfully created."
                          << std::endl;
            }
            else
            {
                std::cerr << "Error while trying to create table "
                             "'diagnosis'."
                          << std::endl;
            }
        }
    }
    else
    {
        std::cerr << "Error while trying to check if table exists" << std::endl;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}