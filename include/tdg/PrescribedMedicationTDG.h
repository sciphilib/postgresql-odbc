#pragma once

#include "PrescribedMedication.h"
#include "TableDataGateway.h"
#include <iostream>

class PrescribedMedicationTDG : public TableDataGateway
{
public:
    PrescribedMedicationTDG(SQLHDBC hDbc) : TableDataGateway(hDbc) {}

    void insert(const BaseObject& object) override
    {
        const PrescribedMedication& prmed =
            dynamic_cast<const PrescribedMedication&>(object);

        SQLHSTMT hStmt;
        SQLAllocHandle(SQL_HANDLE_STMT, hDbc_, &hStmt);

        SQLCHAR insertQuery[] = "INSERT INTO prescribed_medications (id_visit, "
                                "id_medication) VALUES (?, ?)";
        if (SQLPrepare(hStmt, insertQuery, SQL_NTS) != SQL_SUCCESS)
        {
            std::cerr << "Error: failed to prepare the SQL statement."
                      << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }

        int idVisit = prmed.getIdVisit();
        if (SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER,
                             0, 0, &idVisit, 0, nullptr) != SQL_SUCCESS)
        {
            std::cerr << "Error: failed to bind parameters." << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }

        int idMedication = prmed.getIdMedication();
        if (SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER,
                             0, 0, &idMedication, 0, nullptr) != SQL_SUCCESS)
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
            std::cerr << "Error: can not insert prescribedMedication. Message: "
                      << message << ", SQL State: " << sqlState << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }
        std::cout << "Success: prescribedMedication was successfully added."
                  << std::endl;

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }
};