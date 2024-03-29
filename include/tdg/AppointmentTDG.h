#pragma once

#include "Appointment.h"
#include "DateTime.h"
#include "TableDataGateway.h"
#include <cstddef>
#include <iostream>
#include <memory>
#include <sql.h>
#include <sqltypes.h>
#include <sstream>
#include <string>
#include <unordered_map>

class AppointmentTDG : public TableDataGateway
{
public:
    AppointmentTDG(SQLHDBC hDbc) : TableDataGateway(hDbc) {}

    std::unordered_map<int, Appointment> select(int limit, int offset)
    {
        SQLHSTMT hStmt;
        SQLRETURN retcode;
        int id_, idDoctor, idDayOfWeek, office, district;
        DateTime beginDate(true), endDate(true);
        auto appointments = std::unordered_map<int, Appointment>();

        retcode = SQLAllocHandle(SQL_HANDLE_STMT, hDbc_, &hStmt);
        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        {
            std::cerr << "Error allocating SQL Handle\n";
            return appointments;
        }

        retcode = SQLPrepare(
            hStmt, (SQLCHAR*)"SELECT * FROM appointments LIMIT ? OFFSET ?",
            SQL_NTS);
        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        {
            std::cerr << "Error preparing SQL query\n";
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return appointments;
        }

        retcode = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG,
                                   SQL_INTEGER, 0, 0, &limit, 0, NULL);
        retcode = SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_SLONG,
                                   SQL_INTEGER, 0, 0, &offset, 0, NULL);
        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        {
            std::cerr << "Error binding parameters\n";
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return appointments;
        }

        retcode = SQLExecute(hStmt);
        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        {
            std::cerr << "Error executing SQL query\n";
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return appointments;
        }

        SQL_TIME_STRUCT beginTime, endTime;
        SQLLEN indicator;
        while (SQLFetch(hStmt) == SQL_SUCCESS)
        {
            SQLGetData(hStmt, 1, SQL_C_SLONG, &id_, 0, NULL);
            SQLGetData(hStmt, 2, SQL_C_SLONG, &idDoctor, 0, NULL);
            SQLGetData(hStmt, 3, SQL_C_SLONG, &idDayOfWeek, 0, NULL);
            SQLGetData(hStmt, 4, SQL_C_TIME, &beginTime, sizeof(beginTime),
                       &indicator);
            if (indicator != SQL_NULL_DATA)
            {
                std::string time = std::to_string(beginTime.hour) + ":" +
                                   std::to_string(beginTime.minute);
                beginDate.setDateTime(time);
            }
            SQLGetData(hStmt, 5, SQL_C_TIME, &endTime, sizeof(endTime),
                       &indicator);
            if (indicator != SQL_NULL_DATA)
            {
                std::string time = std::to_string(endTime.hour) + ":" +
                                   std::to_string(endTime.minute);
                endDate.setDateTime(time);
            }
            SQLGetData(hStmt, 6, SQL_C_SLONG, &office, 0, NULL);
            SQLGetData(hStmt, 7, SQL_C_SLONG, &district, 0, NULL);

            appointments.emplace(
                std::piecewise_construct, std::forward_as_tuple(id_),
                std::forward_as_tuple(Appointment(id_, idDoctor, idDayOfWeek,
                                                  beginDate, endDate, office,
                                                  district)));
        }
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

        return appointments;
    }

    bool deleteById(int id) override {}

    bool update(int id, const BaseObject& object) override
    {
        SQLHSTMT hStmt;
        SQLRETURN retcode;

        const Appointment* appointmentPtr = nullptr;
        try
        {
            appointmentPtr = &dynamic_cast<const Appointment&>(object);
        }
        catch (const std::bad_cast& e)
        {
            std::cerr << "Dynamic cast failed\n";
            return false;
        }
        if (!appointmentPtr)
        {
            return false;
        }

        const Appointment& appointment = *appointmentPtr;
        int idDoctor = appointment.getIdDoctor();
        int idWeekday = appointment.getIdWeekday();
        std::string beginStr = appointment.getBeginDate().toString();
        std::string endStr = appointment.getEndDate().toString();
        int office = appointment.getOffice();
        int district = appointment.getDistrict();

        retcode = SQLAllocHandle(SQL_HANDLE_STMT, hDbc_, &hStmt);
        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        {
            std::cerr << "Error allocating SQL Handle\n";
            return false;
        }

        retcode =
            SQLPrepare(hStmt,
                       (SQLCHAR*)"UPDATE appointments SET id_doctor = ?, "
                                 "id_weekday = ?, begin_date = ?, end_date = "
                                 "?, office = ?, district = ? WHERE id = ?",
                       SQL_NTS);
        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        {
            std::cerr << "Error preparing SQL query\n";
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return false;
        }

        retcode = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG,
                                   SQL_INTEGER, 0, 0, &idDoctor, 0, NULL);
        retcode += SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_SLONG,
                                    SQL_INTEGER, 0, 0, &idWeekday, 0, NULL);
        retcode += SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR,
                                    SQL_VARCHAR, beginStr.length(), 0,
                                    (SQLCHAR*)beginStr.c_str(), 0, NULL);
        retcode += SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR,
                                    SQL_VARCHAR, endStr.length(), 0,
                                    (SQLCHAR*)endStr.c_str(), 0, NULL);
        retcode += SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_SLONG,
                                    SQL_INTEGER, 0, 0, &office, 0, NULL);
        retcode += SQLBindParameter(hStmt, 6, SQL_PARAM_INPUT, SQL_C_SLONG,
                                    SQL_INTEGER, 0, 0, &district, 0, NULL);
        retcode += SQLBindParameter(hStmt, 7, SQL_PARAM_INPUT, SQL_C_SLONG,
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
        int id_, idDoctor, idDayOfWeek, office, district;
        DateTime beginDate(true), endDate(true);

        retcode = SQLAllocHandle(SQL_HANDLE_STMT, hDbc_, &hStmt);
        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        {
            std::cerr << "Error allocating SQL Handle\n";
            return nullptr;
        }

        retcode = SQLPrepare(
            hStmt, (SQLCHAR*)"SELECT * FROM appointments WHERE id = ?",
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

        SQL_TIME_STRUCT beginTime, endTime;
        SQLLEN indicator;
        if (SQLFetch(hStmt) == SQL_SUCCESS)
        {
            SQLGetData(hStmt, 1, SQL_C_SLONG, &id_, 0, NULL);
            SQLGetData(hStmt, 2, SQL_C_SLONG, &idDoctor, 0, NULL);
            SQLGetData(hStmt, 3, SQL_C_SLONG, &idDayOfWeek, 0, NULL);
            SQLGetData(hStmt, 4, SQL_C_TIME, &beginTime, sizeof(beginTime),
                       &indicator);
            if (indicator != SQL_NULL_DATA)
            {
                std::string time = std::to_string(beginTime.hour) + ":" +
                                   std::to_string(beginTime.minute);
                beginDate.setDateTime(time);
            }
            SQLGetData(hStmt, 5, SQL_C_TIME, &endTime, sizeof(endTime),
                       &indicator);
            if (indicator != SQL_NULL_DATA)
            {
                std::string time = std::to_string(endTime.hour) + ":" +
                                   std::to_string(endTime.minute);
                endDate.setDateTime(time);
            }
            SQLGetData(hStmt, 6, SQL_C_SLONG, &office, 0, NULL);
            SQLGetData(hStmt, 7, SQL_C_SLONG, &district, 0, NULL);

            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

            return std::make_unique<Appointment>(id_, idDoctor, idDayOfWeek,
                                                 beginDate, endDate, office,
                                                 district);
        }

        return nullptr;
    }

    void insert(const BaseObject& object) override
    {
        const Appointment& appointment =
            dynamic_cast<const Appointment&>(object);

        SQLHSTMT hStmt;
        SQLAllocHandle(SQL_HANDLE_STMT, hDbc_, &hStmt);

        SQLCHAR insertQuery[] =
            "INSERT INTO appointments (id_doctor, id_weekday, begin_date, "
            "end_date, office, district) VALUES (?, ?, ?, ?, ?, ?)";
        if (SQLPrepare(hStmt, insertQuery, SQL_NTS) != SQL_SUCCESS)
        {
            std::cerr << "Error: failed to prepare the SQL statement."
                      << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }

        int idDoctor = appointment.getIdDoctor();
        if (SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER,
                             0, 0, &idDoctor, 0, nullptr) != SQL_SUCCESS)
        {
            std::cerr << "Error: failed to bind parameters." << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }

        int idWeekday = appointment.getIdWeekday();
        if (SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER,
                             0, 0, &idWeekday, 0, nullptr) != SQL_SUCCESS)
        {
            std::cerr << "Error: failed to bind parameters." << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }

        std::string beginDate = appointment.getBeginDate().toString();
        if (SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                             beginDate.length(), 0, (SQLCHAR*)beginDate.c_str(),
                             0, nullptr) != SQL_SUCCESS)
        {
            std::cerr << "Error: failed to bind parameters." << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }

        std::string endDate = appointment.getEndDate().toString();
        if (SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                             endDate.length(), 0, (SQLCHAR*)endDate.c_str(), 0,
                             nullptr) != SQL_SUCCESS)
        {
            std::cerr << "Error: failed to bind parameters." << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }

        int office = appointment.getOffice();
        if (SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER,
                             0, 0, &office, 0, nullptr) != SQL_SUCCESS)
        {
            std::cerr << "Error: failed to bind parameters." << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }

        int district = appointment.getDistrict();
        if (SQLBindParameter(hStmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER,
                             0, 0, &district, 0, nullptr) != SQL_SUCCESS)
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
            std::cerr << "Error: can not insert appointment. Message: "
                      << message << ", SQL State: " << sqlState << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }
        std::cout << "Success: appointment was successfully added."
                  << std::endl;

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }
};