#pragma once

#include "Specialization.h"
#include "TableDataGateway.h"
#include <iostream>

class SpecializationTDG : public TableDataGateway
{
public:
    SpecializationTDG(SQLHDBC hDbc) : TableDataGateway(hDbc) {}

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