#pragma once

#include "Patient.h"
#include "TableDataGateway.h"
#include <iostream>
#include <memory>

class PatientTDG : public TableDataGateway
{
public:
    PatientTDG(SQLHDBC hDbc) : TableDataGateway(hDbc) {}

    std::unique_ptr<BaseObject> findById(int id) override
    {
        SQLHSTMT hStmt;
        SQLRETURN retcode;
        int id_;
        std::string lastName, firstName, middleName, address;

        retcode = SQLAllocHandle(SQL_HANDLE_STMT, hDbc_, &hStmt);
        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        {
            std::cerr << "Error allocating SQL Handle\n";
            return nullptr;
        }

        retcode = SQLPrepare(
            hStmt, (SQLCHAR*)"SELECT * FROM patients WHERE id = ?", SQL_NTS);
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
            SQLGetData(hStmt, 5, SQL_C_CHAR, resultData, sizeof(resultData),
                       NULL);
            address = std::string(reinterpret_cast<char*>(resultData));

            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

            return std::make_unique<Patient>(id_, lastName, firstName, middleName, address);
        }

        return nullptr;
    }

    void insert(const BaseObject& object) override
    {
        const Patient& patient = dynamic_cast<const Patient&>(object);

        SQLHSTMT hStmt;
        SQLAllocHandle(SQL_HANDLE_STMT, hDbc_, &hStmt);

        SQLCHAR insertQuery[] = "INSERT INTO patients (last_name, first_name, "
                                "middle_name, address) VALUES (?, ?, ?, ?)";
        if (SQLPrepare(hStmt, insertQuery, SQL_NTS) != SQL_SUCCESS)
        {
            std::cerr << "Error: failed to prepare the SQL statement."
                      << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }

        if (SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                             0, 0, (SQLCHAR*)patient.getLastName().c_str(), 0,
                             nullptr) != SQL_SUCCESS)
        {
            std::cerr << "Error: failed to bind parameters." << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }

        if (SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                             0, 0, (SQLCHAR*)patient.getFirstName().c_str(), 0,
                             nullptr) != SQL_SUCCESS)
        {
            std::cerr << "Error: failed to bind parameters." << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }

        if (SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                             0, 0, (SQLCHAR*)patient.getMiddleName().c_str(), 0,
                             nullptr) != SQL_SUCCESS)
        {
            std::cerr << "Error: failed to bind parameters." << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }

        std::string utf8Address = to_utf8(patient.getAddress());
        if (SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                             utf8Address.length(), 0,
                             (SQLCHAR*)utf8Address.c_str(), 0,
                             nullptr) != SQL_SUCCESS)
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
            std::cerr << "Error: can not insert patient. Message: " << message
                      << ", SQL State: " << sqlState << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }
        std::cout << "Success: patient was successfully added." << std::endl;

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }
};