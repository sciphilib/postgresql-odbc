#pragma once

#include "TableDataGateway.h"
#include "Visit.h"
#include <iostream>
#include <memory>
#include <string>

class VisitTDG : public TableDataGateway
{
public:
    VisitTDG(SQLHDBC hDbc) : TableDataGateway(hDbc) {}

    std::unique_ptr<BaseObject> findById(int id) override
    {
        SQLHSTMT hStmt;
        SQLRETURN retcode;
        int id_, idPatient, idDoctor;
        std::string complaints;
        DateTime dateVisit(false), dateDischarge(false), dateClose(false);

        retcode = SQLAllocHandle(SQL_HANDLE_STMT, hDbc_, &hStmt);
        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        {
            std::cerr << "Error allocating SQL Handle\n";
            return nullptr;
        }

        retcode = SQLPrepare(
            hStmt, (SQLCHAR*)"SELECT * FROM visits WHERE id = ?", SQL_NTS);
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

        TIMESTAMP_STRUCT time;
        SQLLEN indicator;
        SQLCHAR resultData[256];
        if (SQLFetch(hStmt) == SQL_SUCCESS)
        {
            SQLGetData(hStmt, 1, SQL_C_SLONG, &id_, 0, NULL);
            SQLGetData(hStmt, 2, SQL_C_SLONG, &idPatient, 0, NULL);
            SQLGetData(hStmt, 3, SQL_C_SLONG, &idDoctor, 0, NULL);
            SQLGetData(hStmt, 4, SQL_C_CHAR, resultData, sizeof(resultData),
                       NULL);
            complaints = std::string(reinterpret_cast<char*>(resultData));
            SQLGetData(hStmt, 5, SQL_C_TIMESTAMP, &time, sizeof(time),
                       &indicator);
            if (indicator != SQL_NULL_DATA)
            {
                std::string timeStr = std::to_string(time.year) + "-" +
                                      std::to_string(time.month) + "-" +
                                      std::to_string(time.day) + " " +
                                      std::to_string(time.hour) + ":" +
                                      std::to_string(time.minute);
                dateVisit.setDateTime(timeStr);
            }
            SQLGetData(hStmt, 6, SQL_C_TIMESTAMP, &time, sizeof(time),
                       &indicator);
            if (indicator != SQL_NULL_DATA)
            {
                std::string timeStr = std::to_string(time.year) + "-" +
                                      std::to_string(time.month) + "-" +
                                      std::to_string(time.day) + " " +
                                      std::to_string(time.hour) + ":" +
                                      std::to_string(time.minute);
                dateDischarge.setDateTime(timeStr);
            }
            SQLGetData(hStmt, 7, SQL_C_TIMESTAMP, &time, sizeof(time),
                       &indicator);
            if (indicator != SQL_NULL_DATA)
            {
                std::string timeStr = std::to_string(time.year) + "-" +
                                      std::to_string(time.month) + "-" +
                                      std::to_string(time.day) + " " +
                                      std::to_string(time.hour) + ":" +
                                      std::to_string(time.minute);
                dateClose.setDateTime(timeStr);
            }

            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

            return std::make_unique<Visit>(id_, idPatient, idDoctor,
                                           complaints, dateVisit, dateDischarge,
                                           dateClose);
        }

        return nullptr;
    }

    void insert(const BaseObject& object) override
    {
        const Visit& visit = dynamic_cast<const Visit&>(object);

        SQLHSTMT hStmt;
        SQLAllocHandle(SQL_HANDLE_STMT, hDbc_, &hStmt);

        SQLCHAR insertQuery[] =
            "INSERT INTO visits (id_patient, id_doctor, complaints, "
            "date_visit, date_discharge, date_close) VALUES (?, ?, ?, ?, ?, ?)";
        if (SQLPrepare(hStmt, insertQuery, SQL_NTS) != SQL_SUCCESS)
        {
            std::cerr << "Error: failed to prepare the SQL statement."
                      << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }

        int idPatient = visit.getIdPatient();
        if (SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER,
                             0, 0, &idPatient, 0, nullptr) != SQL_SUCCESS)
        {
            std::cerr << "Error: failed to bind parameters." << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }

        int idDoctor = visit.getIdDoctor();
        if (SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER,
                             0, 0, &idDoctor, 0, nullptr) != SQL_SUCCESS)
        {
            std::cerr << "Error: failed to bind parameters." << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }

        std::string complaints = to_utf8(visit.getComplaints());
        if (SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                             complaints.length(), 0,
                             (SQLCHAR*)complaints.c_str(), 0,
                             nullptr) != SQL_SUCCESS)
        {
            std::cerr << "Error: failed to bind parameters." << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }

        std::string dateVisit = visit.getDateVisit().toString();
        if (SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                             dateVisit.length(), 0, (SQLCHAR*)dateVisit.c_str(),
                             0, nullptr) != SQL_SUCCESS)
        {
            std::cerr << "Error: failed to bind parameters." << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }

        std::string dateDischarge = visit.getDateDischarge().toString();
        if (SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                             dateDischarge.length(), 0,
                             (SQLCHAR*)dateDischarge.c_str(), 0,
                             nullptr) != SQL_SUCCESS)
        {
            std::cerr << "Error: failed to bind parameters." << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }

        std::string dateClose = visit.getDateClose().toString();
        if (SQLBindParameter(hStmt, 6, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                             dateClose.length(), 0, (SQLCHAR*)dateClose.c_str(),
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
            std::cerr << "Error: can not insert visit. Message: " << message
                      << ", SQL State: " << sqlState << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }
        std::cout << "Success: visit was successfully added." << std::endl;

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }
};