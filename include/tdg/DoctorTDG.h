#pragma once

#include "Doctor.h"
#include "TableDataGateway.h"
#include <iostream>
#include <memory>

class DoctorTDG : public TableDataGateway
{
public:
    DoctorTDG(SQLHDBC hDbc) : TableDataGateway(hDbc) {}

    bool update(int id, const BaseObject& object) override
    {
        SQLHSTMT hStmt;
        SQLRETURN retcode;

        const Doctor* ptr = nullptr;
        try
        {
            ptr = &dynamic_cast<const Doctor&>(object);
        }
        catch (const std::bad_cast& e)
        {
            std::cerr << "Dynamic cast failed\n";
            return false;
        }
        if (!ptr)
        {
            return false;
        }

        const Doctor& doctor = *ptr;
        std::string lastName = doctor.getLastName();
        std::string firstName = doctor.getFirstName();
        std::string middleName = doctor.getMiddleName();
        int idSpec = doctor.getIdSpec();

        retcode = SQLAllocHandle(SQL_HANDLE_STMT, hDbc_, &hStmt);
        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        {
            std::cerr << "Error allocating SQL Handle\n";
            return false;
        }

        retcode = SQLPrepare(hStmt,
                             (SQLCHAR*)"UPDATE doctors SET last_name = ?, "
                                       "first_name = ?, middle_name = ?, "
                                       "id_spec = ? WHERE id = ?",
                             SQL_NTS);
        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        {
            std::cerr << "Error preparing SQL query\n";
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return false;
        }

        retcode = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR,
                                    SQL_VARCHAR, lastName.length(), 0,
                                    (SQLCHAR*)lastName.c_str(), 0, NULL);
        retcode += SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR,
                                    SQL_VARCHAR, firstName.length(), 0,
                                    (SQLCHAR*)firstName.c_str(), 0, NULL);
        retcode += SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR,
                                    SQL_VARCHAR, middleName.length(), 0,
                                    (SQLCHAR*)middleName.c_str(), 0, NULL);
        retcode += SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_SLONG,
                                   SQL_INTEGER, 0, 0, &idSpec, 0, NULL);
        retcode += SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_SLONG,
                                   SQL_INTEGER, 0, 0, &id, 0, NULL);
        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        {
            std::cerr << "Error binding parameters\n";
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return false;
        }

        retcode = SQLExecute(hStmt);
        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        {
            std::cerr << "Error executing SQL query\n";
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return false;
        }

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return true;
    }

    std::unique_ptr<BaseObject> findById(int id) override
    {
        SQLHSTMT hStmt;
        SQLRETURN retcode;
        int id_, idSpec;
        std::string lastName, firstName, middleName;

        retcode = SQLAllocHandle(SQL_HANDLE_STMT, hDbc_, &hStmt);
        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        {
            std::cerr << "Error allocating SQL Handle\n";
            return nullptr;
        }

        retcode = SQLPrepare(
            hStmt, (SQLCHAR*)"SELECT * FROM doctors WHERE id = ?", SQL_NTS);
        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        {
            std::cerr << "Error preparing SQL query\n";
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return nullptr;
        }

        retcode = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG,
                                   SQL_INTEGER, 0, 0, &id, 0, NULL);
        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        {
            std::cerr << "Error binding parameters\n";
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return nullptr;
        }

        retcode = SQLExecute(hStmt);
        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        {
            std::cerr << "Error executing SQL query\n";
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return nullptr;
        }

        SQLCHAR resultData[256];
        if (SQLFetch(hStmt) == SQL_SUCCESS)
        {
            SQLGetData(hStmt, 1, SQL_C_SLONG, &id_, 0, NULL);
            SQLGetData(hStmt, 2, SQL_C_CHAR, resultData, sizeof(resultData),
                       NULL);
            lastName = std::string(reinterpret_cast<char*>(resultData));
            SQLGetData(hStmt, 3, SQL_C_CHAR, resultData, sizeof(resultData),
                       NULL);
            firstName = std::string(reinterpret_cast<char*>(resultData));
            SQLGetData(hStmt, 4, SQL_C_CHAR, resultData, sizeof(resultData),
                       NULL);
            middleName = std::string(reinterpret_cast<char*>(resultData));
            SQLGetData(hStmt, 5, SQL_C_SLONG, &idSpec, sizeof(resultData),
                       NULL);

            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

            return std::make_unique<Doctor>(id_, lastName, firstName,
                                            middleName, idSpec);
        }

        return nullptr;
    }

    void insert(const BaseObject& object) override
    {
        const Doctor& doctor = dynamic_cast<const Doctor&>(object);

        SQLHSTMT hStmt;
        SQLAllocHandle(SQL_HANDLE_STMT, hDbc_, &hStmt);

        SQLCHAR insertQuery[] = "INSERT INTO doctors (last_name, first_name, "
                                "middle_name, id_spec) VALUES (?, ?, ?, ?)";
        if (SQLPrepare(hStmt, insertQuery, SQL_NTS) != SQL_SUCCESS)
        {
            std::cerr << "Error: failed to prepare the SQL statement."
                      << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }

        if (SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                             0, 0, (SQLCHAR*)doctor.getLastName().c_str(), 0,
                             nullptr) != SQL_SUCCESS)
        {
            std::cerr << "Error: failed to bind parameters." << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }

        if (SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                             0, 0, (SQLCHAR*)doctor.getFirstName().c_str(), 0,
                             nullptr) != SQL_SUCCESS)
        {
            std::cerr << "Error: failed to bind parameters." << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }

        if (SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                             0, 0, (SQLCHAR*)doctor.getMiddleName().c_str(), 0,
                             nullptr) != SQL_SUCCESS)
        {
            std::cerr << "Error: failed to bind parameters." << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }

        int idSpec = doctor.getIdSpec();
        if (SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER,
                             0, 0, &idSpec, 0, nullptr) != SQL_SUCCESS)
        {
            std::cerr << "Error: failed to bind parameters." << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }

        SQLRETURN ret = SQLExecute(hStmt);
        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, 1, sqlState, &nativeError,
                          message, 1000, &textLength);
            std::cerr << "Error: can not insert doctor. Message: " << message
                      << ", SQL State: " << sqlState << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }
        std::cout << "Success: doctor was successfully added." << std::endl;

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }
};