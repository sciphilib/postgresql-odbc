#pragma once

#include "PrescribedProcedure.h"
#include "TableDataGateway.h"
#include <iostream>
#include <memory>
#include <tuple>
#include <unordered_map>
#include <utility>

class PrescribedProcedureTDG : public TableDataGateway
{
public:
    PrescribedProcedureTDG(SQLHDBC hDbc) : TableDataGateway(hDbc) {}

    std::unordered_map<int, PrescribedProcedure> select(int limit, int offset)
    {
        SQLHSTMT hStmt;
        SQLRETURN retcode;
        int id_, idVisit, idProcedure, count;
        auto prprocs = std::unordered_map<int, PrescribedProcedure>();

        retcode = SQLAllocHandle(SQL_HANDLE_STMT, hDbc_, &hStmt);
        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        {
            std::cerr << "Error allocating SQL Handle\n";
            return prprocs;
        }

        retcode = SQLPrepare(
            hStmt,
            (SQLCHAR*)"SELECT * FROM prescribed_procedures LIMIT ? OFFSET ?",
            SQL_NTS);
        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        {
            std::cerr << "Error preparing SQL query\n";
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return prprocs;
        }

        retcode = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG,
                                   SQL_INTEGER, 0, 0, &limit, 0, NULL);
        retcode = SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_SLONG,
                                   SQL_INTEGER, 0, 0, &offset, 0, NULL);
        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        {
            std::cerr << "Error binding parameters\n";
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return prprocs;
        }

        retcode = SQLExecute(hStmt);
        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        {
            std::cerr << "Error executing SQL query\n";
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return prprocs;
        }

        while (SQLFetch(hStmt) == SQL_SUCCESS)
        {
            SQLGetData(hStmt, 1, SQL_C_SLONG, &id_, 0, NULL);
            SQLGetData(hStmt, 2, SQL_C_SLONG, &idVisit, 0, NULL);
            SQLGetData(hStmt, 3, SQL_C_SLONG, &idProcedure, 0, NULL);
            SQLGetData(hStmt, 4, SQL_C_SLONG, &count, 0, NULL);

            prprocs.emplace(std::piecewise_construct,
                            std::forward_as_tuple(id_),
                            std::forward_as_tuple(PrescribedProcedure(
                                id_, idVisit, idProcedure, count)));
        }
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

        return prprocs;
    }

    bool update(int id, const BaseObject& object) override {}

    bool deleteById(int id) override {}

    std::unique_ptr<BaseObject> findById(int id) override
    {
        SQLHSTMT hStmt;
        SQLRETURN retcode;
        int id_, idVisit, idProcedure, count;

        retcode = SQLAllocHandle(SQL_HANDLE_STMT, hDbc_, &hStmt);
        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        {
            std::cerr << "Error allocating SQL Handle\n";
            return nullptr;
        }

        retcode = SQLPrepare(
            hStmt, (SQLCHAR*)"SELECT * FROM prescribed_procedures WHERE id = ?",
            SQL_NTS);
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

        if (SQLFetch(hStmt) == SQL_SUCCESS)
        {
            SQLGetData(hStmt, 1, SQL_C_SLONG, &id_, 0, NULL);
            SQLGetData(hStmt, 2, SQL_C_SLONG, &idVisit, 0, NULL);
            SQLGetData(hStmt, 3, SQL_C_SLONG, &idProcedure, 0, NULL);
            SQLGetData(hStmt, 4, SQL_C_SLONG, &count, 0, NULL);

            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

            return std::make_unique<PrescribedProcedure>(id_, idVisit,
                                                         idProcedure, count);
        }

        return nullptr;
    }

    void insert(const BaseObject& object) override
    {
        const PrescribedProcedure& prproc =
            dynamic_cast<const PrescribedProcedure&>(object);

        SQLHSTMT hStmt;
        SQLAllocHandle(SQL_HANDLE_STMT, hDbc_, &hStmt);

        SQLCHAR insertQuery[] = "INSERT INTO prescribed_procedures (id_visit, "
                                "id_procedure, count) VALUES (?, ?, ?)";
        if (SQLPrepare(hStmt, insertQuery, SQL_NTS) != SQL_SUCCESS)
        {
            std::cerr << "Error: failed to prepare the SQL statement."
                      << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }

        int idVisit = prproc.getIdVisit();
        if (SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER,
                             0, 0, &idVisit, 0, nullptr) != SQL_SUCCESS)
        {
            std::cerr << "Error: failed to bind parameters." << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }

        int idProcedure = prproc.getIdProcedure();
        if (SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER,
                             0, 0, &idProcedure, 0, nullptr) != SQL_SUCCESS)
        {
            std::cerr << "Error: failed to bind parameters." << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }

        int count = prproc.getCount();
        if (SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER,
                             0, 0, &count, 0, nullptr) != SQL_SUCCESS)
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
            std::cerr << "Error: can not insert prescribedProcedure. Message: "
                      << message << ", SQL State: " << sqlState << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }
        std::cout << "Success: prescribedProcedure was successfully added."
                  << std::endl;

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }
};