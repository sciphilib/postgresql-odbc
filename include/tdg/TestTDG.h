#pragma once

#include "TableDataGateway.h"
#include "Test.h"
#include <iostream>
#include <memory>

class TestTDG : public TableDataGateway
{
public:
    TestTDG(SQLHDBC hDbc) : TableDataGateway(hDbc) {}

    bool update(int id, const BaseObject& object) override {}

    bool deleteById(int id) override {}

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
            hStmt, (SQLCHAR*)"SELECT * FROM tests WHERE id = ?", SQL_NTS);
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

        SQLCHAR resultData[100];
        if (SQLFetch(hStmt) == SQL_SUCCESS)
        {
            SQLGetData(hStmt, 1, SQL_C_SLONG, &id_, 0, NULL);
            SQLGetData(hStmt, 2, SQL_C_CHAR, resultData, sizeof(resultData),
                       NULL);
            name = std::string(reinterpret_cast<char*>(resultData));

            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

            return std::make_unique<Test>(id_, name);
        }

        return nullptr;
    }

    void insert(const BaseObject& object) override
    {
        const Test& test = dynamic_cast<const Test&>(object);

        SQLHSTMT hStmt;
        SQLAllocHandle(SQL_HANDLE_STMT, hDbc_, &hStmt);

        SQLCHAR insertQuery[] = "INSERT INTO tests (name) VALUES (?)";
        if (SQLPrepare(hStmt, insertQuery, SQL_NTS) != SQL_SUCCESS)
        {
            std::cerr << "Error: failed to prepare the SQL statement."
                      << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }

        std::string testUtf8 = to_utf8(test.getName());
        if (SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                             testUtf8.length(), 0, (SQLCHAR*)testUtf8.c_str(),
                             0, nullptr) != SQL_SUCCESS)
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
            std::cerr << "Error: can not insert test. Message: " << message
                      << ", SQL State: " << sqlState << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }
        std::cout << "Success: test was successfully added." << std::endl;

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }
};