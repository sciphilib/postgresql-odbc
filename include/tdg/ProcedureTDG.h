#pragma once

#include "Procedure.h"
#include "TableDataGateway.h"
#include <iostream>

class ProcedureTDG : public TableDataGateway
{
public:
    ProcedureTDG(SQLHDBC hDbc) : TableDataGateway(hDbc) {}

    void insert(const BaseObject& object) override
    {
        const Procedure& procedure = dynamic_cast<const Procedure&>(object);

        SQLHSTMT hStmt;
        SQLAllocHandle(SQL_HANDLE_STMT, hDbc_, &hStmt);

        SQLCHAR insertQuery[] = "INSERT INTO procedures (name) VALUES (?)";
        if (SQLPrepare(hStmt, insertQuery, SQL_NTS) != SQL_SUCCESS)
        {
            std::cerr << "Error: failed to prepare the SQL statement."
                      << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }

        std::string procedureUtf8 = to_utf8(procedure.getName());
        if (SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                             procedureUtf8.length(), 0,
                             (SQLCHAR*)procedureUtf8.c_str(), 0,
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
            std::cerr << "Error: can not insert procedure. Message: " << message
                      << ", SQL State: " << sqlState << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }
        std::cout << "Success: procedure was successfully added." << std::endl;

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }
};