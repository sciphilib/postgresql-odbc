#include "DatabaseManager.h"

DatabaseManager::DatabaseManager(const std::string& dsn,
                                 const std::string& user,
                                 const std::string& pass)
    : dsn_(dsn), user_(user), pass_(pass)
{
    connect();
    initDatabase();
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
    // Выделение среды
    if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv) == SQL_ERROR)
    {
        std::cerr << "Error allocating an environment handle\n";
        exit(-1);
    }

    // Установка ODBC версии 3
    if (SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0) ==
        SQL_ERROR)
    {
        std::cerr << "Error setting the environment attribute setting\n";
        exit(-1);
    }

    // Выделение соединения
    if (SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc) == SQL_ERROR)
    {
        std::cerr << "Error allocating a database connection handle\n";
        exit(-1);
    }

    // Подключение к источнику данных
    int retcode = SQLConnect(hDbc, (SQLCHAR*)"PostgreSQLDSN", SQL_NTS,
                             (SQLCHAR*)"sciphilib", SQL_NTS,
                             (SQLCHAR*)"sciphilib", SQL_NTS);
    if (retcode == SQL_ERROR)
    {
        std::cerr << "Error connecting to the data source\n";
        exit(-1);
    }

    // Выделение оператора
    if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_ERROR)
    {
        std::cerr << "Error allocating a statement handle\n";
        exit(-1);
    }
}

void DatabaseManager::initDatabase()
{
    createTableAll();
    Parser parser;
    parser.parseAppointment("data/appointments.json");
    parser.printAppointments();
    DatabaseFacade databaseFacade(hDbc);
    databaseFacade.addWeekday("Monday");
    return;
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
    createTableTestsResults();
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
            SQLCHAR createTableQuery[] = "CREATE TABLE specializations (id INT "
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
                std::cerr
                    << "Error while trying to create table 'specializations'."
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
                "CREATE TABLE doctors (id INT "
                "PRIMARY KEY, last_name VARCHAR(255), first_name VARCHAR(255), "
                "middle_name VARCHAR(255), id_spec INT REFERENCES "
                "Specializations(id))";
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
                "CREATE TABLE appointments (id INT "
                "PRIMARY KEY, id_doctor INT REFERENCES Doctors(id), id_weekday "
                "INT REFERENCES weekdays(id), begin_date DATE, end_date DATE, "
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
                "CREATE TABLE patients (id INT "
                "PRIMARY KEY, last_name VARCHAR(255), first_name VARCHAR(255), "
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
                "CREATE TABLE visits (id INT "
                "PRIMARY KEY, id_patient INT REFERENCES patients(id), "
                "date_visit DATE, complaints VARCHAR(255), date_discharge "
                "DATE, date_close DATE)";
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
            SQLCHAR createTableQuery[] = "CREATE TABLE medications (id INT "
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
                "CREATE TABLE prescribed_medications (id INT "
                "PRIMARY KEY, id_visit INT REFERENCES visits(id), "
                "id_medication INT REFERENCES medications(id))";
            SQLRETURN createRet =
                SQLExecDirect(hStmt, createTableQuery, SQL_NTS);

            if (createRet == SQL_SUCCESS || createRet == SQL_SUCCESS_WITH_INFO)
            {
                std::cout
                    << "Table 'prescribed_medications' was succesfully created."
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
            SQLCHAR createTableQuery[] = "CREATE TABLE procedures (id INT "
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
                "CREATE TABLE prescribed_procedures (id INT "
                "PRIMARY KEY, id_visit INT REFERENCES visits(id), id_procedure "
                "INT REFERENCES procedures(id),"
                "count INT)";
            SQLRETURN createRet =
                SQLExecDirect(hStmt, createTableQuery, SQL_NTS);

            if (createRet == SQL_SUCCESS || createRet == SQL_SUCCESS_WITH_INFO)
            {
                std::cout
                    << "Table 'prescribed_medications' was succesfully created."
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
            SQLCHAR createTableQuery[] = "CREATE TABLE tests (id INT "
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

void DatabaseManager::createTableTestsResults()
{
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    SQLCHAR query[] =
        "SELECT 1 FROM information_schema.tables WHERE "
        "table_schema = 'public' AND table_name = 'tests_results'";
    SQLRETURN ret = SQLExecDirect(hStmt, query, SQL_NTS);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        if (SQLFetch(hStmt) == SQL_SUCCESS)
        {
            std::cout << "Table 'tests_results' is already exists."
                      << std::endl;
        }
        else
        {
            SQLCHAR createTableQuery[] =
                "CREATE TABLE tests_results (id INT "
                "PRIMARY KEY, id_visit INT REFERENCES visits(id), id_test "
                "INT REFERENCES tests(id), result VARCHAR(255))";
            SQLRETURN createRet =
                SQLExecDirect(hStmt, createTableQuery, SQL_NTS);

            if (createRet == SQL_SUCCESS || createRet == SQL_SUCCESS_WITH_INFO)
            {
                std::cout << "Table 'tests_results' was succesfully created."
                          << std::endl;
            }
            else
            {
                std::cerr << "Error while trying to create table "
                             "'tests_results'."
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
                "CREATE TABLE diagnosis (id INT "
                "PRIMARY KEY, id_visit INT REFERENCES visits(id), description "
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