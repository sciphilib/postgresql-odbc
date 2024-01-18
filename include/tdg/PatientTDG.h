#pragma once

#include "Patient.h"
#include "TableDataGateway.h"
#include <iostream>
#include <memory>
#include <tuple>
#include <unordered_map>
#include <utility>

class PatientTDG : public TableDataGateway
{
public:
    PatientTDG(SQLHDBC hDbc) : TableDataGateway(hDbc) {}

    std::unordered_map<int, Patient> select(int limit, int offset)
    {
        SQLHSTMT hStmt;
        SQLRETURN retcode;
        int id_;
        std::string lastName, firstName, middleName, address;
        auto patients = std::unordered_map<int, Patient>();

        retcode = SQLAllocHandle(SQL_HANDLE_STMT, hDbc_, &hStmt);
        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        {
            std::cerr << "Error allocating SQL Handle\n";
            return patients;
        }

        retcode = SQLPrepare(
            hStmt, (SQLCHAR*)"SELECT * FROM patients LIMIT ? OFFSET ?",
            SQL_NTS);
        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        {
            std::cerr << "Error preparing SQL query\n";
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return patients;
        }

        retcode = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG,
                                   SQL_INTEGER, 0, 0, &limit, 0, NULL);
        retcode = SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_SLONG,
                                   SQL_INTEGER, 0, 0, &offset, 0, NULL);
        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        {
            std::cerr << "Error binding parameters\n";
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return patients;
        }

        retcode = SQLExecute(hStmt);
        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        {
            std::cerr << "Error executing SQL query\n";
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return patients;
        }

        SQLCHAR resultData[256];
        while (SQLFetch(hStmt) == SQL_SUCCESS)
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

            patients.emplace(
                std::piecewise_construct, std::forward_as_tuple(id_),
                std::forward_as_tuple(
                    (Patient(id_, lastName, firstName, middleName, address))));
        }
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

        return patients;
    }

    bool deleteById(int id) override {}

    bool update(int id, const BaseObject& object) override
    {
        SQLHSTMT hStmt;
        SQLRETURN retcode;

        const Patient* ptr = nullptr;
        try
        {
            ptr = &dynamic_cast<const Patient&>(object);
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

        const Patient& patient = *ptr;
        std::string lastName = patient.getLastName();
        std::string firstName = patient.getFirstName();
        std::string middleName = patient.getMiddleName();
        std::string address = patient.getAddress();

        retcode = SQLAllocHandle(SQL_HANDLE_STMT, hDbc_, &hStmt);
        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        {
            std::cerr << "Error allocating SQL Handle\n";
            return false;
        }

        retcode = SQLPrepare(
            hStmt,
            (SQLCHAR*)"UPDATE patients SET last_name = ?, first_name = ?, "
                      "middle_name = ?, address = ? WHERE id = ?",
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
        retcode += SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR,
                                    SQL_VARCHAR, address.length(), 0,
                                    (SQLCHAR*)address.c_str(), 0, NULL);
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

            return std::make_unique<Patient>(id_, lastName, firstName,
                                             middleName, address);
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