#pragma once

#include "Diagnosis.h"
#include "TableDataGateway.h"
#include <iostream>
#include <memory>
#include <sql.h>
#include <tuple>
#include <unordered_map>
#include <utility>

class DiagnosisTDG : public TableDataGateway
{
public:
    DiagnosisTDG(SQLHDBC hDbc) : TableDataGateway(hDbc) {}

    std::unordered_map<int, Diagnosis> select(int limit, int offset)
    {
        SQLHSTMT hStmt;
        SQLRETURN retcode;
        int id_, idVisit;
        std::string description;
        auto diagnosis = std::unordered_map<int, Diagnosis>();

        retcode = SQLAllocHandle(SQL_HANDLE_STMT, hDbc_, &hStmt);
        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        {
            std::cerr << "Error allocating SQL Handle\n";
            return diagnosis;
        }

        retcode = SQLPrepare(
            hStmt, (SQLCHAR*)"SELECT * FROM diagnosis LIMIT ? OFFSET ?",
            SQL_NTS);
        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        {
            std::cerr << "Error preparing SQL query\n";
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return diagnosis;
        }

        retcode = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG,
                                   SQL_INTEGER, 0, 0, &limit, 0, NULL);
        retcode = SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_SLONG,
                                   SQL_INTEGER, 0, 0, &offset, 0, NULL);
        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        {
            std::cerr << "Error binding parameters\n";
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return diagnosis;
        }

        retcode = SQLExecute(hStmt);
        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        {
            std::cerr << "Error executing SQL query\n";
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return diagnosis;
        }

        SQLCHAR resultData[256];
        while (SQLFetch(hStmt) == SQL_SUCCESS)
        {
            SQLGetData(hStmt, 1, SQL_C_SLONG, &id_, 0, NULL);
            SQLGetData(hStmt, 2, SQL_C_SLONG, &idVisit, 0, NULL);
            SQLGetData(hStmt, 3, SQL_C_CHAR, resultData, sizeof(resultData),
                       NULL);
            description = std::string(reinterpret_cast<char*>(resultData));

            diagnosis.emplace(
                std::piecewise_construct, std::forward_as_tuple(id_),
                std::forward_as_tuple(Diagnosis(id_, idVisit, description)));
        }
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

        return diagnosis;
    }

    bool deleteById(int id) override {}

    bool update(int id, const BaseObject& object) override
    {
        SQLHSTMT hStmt;
        SQLRETURN retcode;

        const Diagnosis* ptr = nullptr;
        try
        {
            ptr = &dynamic_cast<const Diagnosis&>(object);
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

        const Diagnosis& diagnosis = *ptr;
        int idVisit = diagnosis.getIdVisit();
        std::string description = diagnosis.getDescription();

        retcode = SQLAllocHandle(SQL_HANDLE_STMT, hDbc_, &hStmt);
        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        {
            std::cerr << "Error allocating SQL Handle\n";
            return false;
        }

        retcode = SQLPrepare(hStmt,
                             (SQLCHAR*)"UPDATE diagnosis SET id_visit = ?, "
                                       "description = ? WHERE id = ?",
                             SQL_NTS);
        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        {
            std::cerr << "Error preparing SQL query\n";
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return false;
        }

        retcode = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG,
                                   SQL_INTEGER, 0, 0, &idVisit, 0, NULL);
        retcode += SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR,
                                    SQL_VARCHAR, description.length(), 0,
                                    (SQLCHAR*)description.c_str(), 0, NULL);
        retcode += SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_SLONG,
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
        int id_, idVisit;
        std::string description;

        retcode = SQLAllocHandle(SQL_HANDLE_STMT, hDbc_, &hStmt);
        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        {
            std::cerr << "Error allocating SQL Handle\n";
            return nullptr;
        }

        retcode = SQLPrepare(
            hStmt, (SQLCHAR*)"SELECT * FROM diagnosis WHERE id = ?", SQL_NTS);
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
            SQLGetData(hStmt, 2, SQL_C_SLONG, &idVisit, 0, NULL);
            SQLGetData(hStmt, 3, SQL_C_CHAR, resultData, sizeof(resultData),
                       NULL);
            description = std::string(reinterpret_cast<char*>(resultData));

            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

            return std::make_unique<Diagnosis>(id_, idVisit, description);
        }

        return nullptr;
    }

    void insert(const BaseObject& object) override
    {
        const Diagnosis& diagnosis = dynamic_cast<const Diagnosis&>(object);

        SQLHSTMT hStmt;
        SQLAllocHandle(SQL_HANDLE_STMT, hDbc_, &hStmt);

        SQLCHAR insertQuery[] =
            "INSERT INTO diagnosis (id_visit, description) VALUES (?, ?)";
        if (SQLPrepare(hStmt, insertQuery, SQL_NTS) != SQL_SUCCESS)
        {
            std::cerr << "Error: failed to prepare the SQL statement."
                      << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }

        int idVisit = diagnosis.getIdVisit();
        if (SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER,
                             0, 0, &idVisit, 0, nullptr) != SQL_SUCCESS)
        {
            std::cerr << "Error: failed to bind parameters." << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }

        std::string description = to_utf8(diagnosis.getDescription());
        if (SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                             description.length(), 0,
                             (SQLCHAR*)description.c_str(), 0,
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
            std::cerr << "Error: can not insert diagnosis. Message: " << message
                      << ", SQL State: " << sqlState << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }
        std::cout << "Success: diagnosis was successfully added." << std::endl;

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }
};