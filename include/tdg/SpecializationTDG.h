#pragma once

#include "Specialization.h"
#include "TableDataGateway.h"
#include <iostream>
#include <memory>

class SpecializationTDG : public TableDataGateway
{
public:
    SpecializationTDG(SQLHDBC hDbc) : TableDataGateway(hDbc) {}

    bool update(int id, const BaseObject& object) override {}

    std::unique_ptr<BaseObject> findById(int id) override
    {
        SQLHSTMT hStmt;
        SQLRETURN retcode;
        int id_;
        std::string name;

        retcode = SQLAllocHandle(SQL_HANDLE_STMT, hDbc_, &hStmt);
        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        {
            std::cerr << "Error allocating SQL Handle\n";
            return nullptr;
        }

        retcode = SQLPrepare(
            hStmt, (SQLCHAR*)"SELECT * FROM specializations WHERE id = ?", SQL_NTS);
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

        SQLCHAR resultData[50];
        if (SQLFetch(hStmt) == SQL_SUCCESS)
        {
            SQLGetData(hStmt, 1, SQL_C_SLONG, &id_, 0, NULL);
            SQLGetData(hStmt, 2, SQL_C_CHAR, resultData, sizeof(resultData),
                       NULL);
            name = std::string(reinterpret_cast<char*>(resultData));

            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

            return std::make_unique<Specialization>(id_, name);
        }

        return nullptr;
    }

    void insert(const BaseObject& object) override
    {
        const Specialization& spec =
            dynamic_cast<const Specialization&>(object);

        SQLHSTMT hStmt;
        SQLAllocHandle(SQL_HANDLE_STMT, hDbc_, &hStmt);

        SQLCHAR insertQuery[] = "INSERT INTO specializations (name) VALUES (?)";
        if (SQLPrepare(hStmt, insertQuery, SQL_NTS) != SQL_SUCCESS)
        {
            std::cerr << "Error: failed to prepare the SQL statement."
                      << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }

        if (SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                             0, 0, (SQLCHAR*)spec.getName().c_str(), 0,
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
            std::cerr << "Error: can not insert specialization. Message: "
                      << message << ", SQL State: " << sqlState << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }
        std::cout << "Success: specialization was successfully added."
                  << std::endl;

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }
};