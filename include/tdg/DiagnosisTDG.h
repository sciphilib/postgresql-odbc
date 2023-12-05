#pragma once

#include "Diagnosis.h"
#include "TableDataGateway.h"
#include <iostream>
#include <sql.h>

class DiagnosisTDG : public TableDataGateway
{
public:
    DiagnosisTDG(SQLHDBC hDbc) : TableDataGateway(hDbc) {}

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
                             description.length(), 0, (SQLCHAR*)description.c_str(), 0, nullptr) != SQL_SUCCESS)
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
            std::cerr << "Error: can not insert diagnosis. Message: "
                      << message << ", SQL State: " << sqlState << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }
        std::cout << "Success: diagnosis was successfully added."
                  << std::endl;

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }
};