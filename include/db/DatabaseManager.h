#pragma once
#include <iostream>
#include <memory>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>

#include "AppointmentTDG.h"
#include "DatabaseFacade.h"
#include "DiagnosisTDG.h"
#include "DoctorTDG.h"
#include "MedicationTDG.h"
#include "Parser.h"
#include "PatientTDG.h"
#include "PrescribedMedicationTDG.h"
#include "PrescribedProcedureTDG.h"
#include "ProcedureTDG.h"
#include "SpecializationTDG.h"
#include "TestResultTDG.h"
#include "TestTDG.h"
#include "VisitTDG.h"
#include "WeekdayTDG.h"

class DatabaseManager
{
public:
    DatabaseManager(const std::string& dsn, const std::string& user,
                    const std::string& pass);
    ~DatabaseManager();

private:
    SQLHENV hEnv;
    SQLHDBC hDbc;
    SQLHSTMT hStmt;
    std::string dsn_, user_, pass_;
    std::unique_ptr<DatabaseFacade> databaseFacade;
    std::unique_ptr<AppointmentTDG> appointmentTDG;
    std::unique_ptr<DoctorTDG> doctorTDG;
    std::unique_ptr<MedicationTDG> medicationTDG;
    std::unique_ptr<PatientTDG> patientTDG;
    std::unique_ptr<PrescribedMedicationTDG> prescribedMedicationTDG;
    std::unique_ptr<PrescribedProcedureTDG> prescribedProcedureTDG;
    std::unique_ptr<ProcedureTDG> procedureTDG;
    std::unique_ptr<SpecializationTDG> specializationTDG;
    std::unique_ptr<VisitTDG> visitTDG;
    std::unique_ptr<WeekdayTDG> weekdayTDG;
    std::unique_ptr<TestTDG> testTDG;
    std::unique_ptr<TestResultTDG> testResultTDG;
    std::unique_ptr<DiagnosisTDG> diagnosisTDG;

private:
    void connect();
    void initDatabase();
    void initGateways();
    void initFacade();

    void findSomeObjects();

    void createTableAll();
    void createTableWeekdays();
    void createTableSpecializations();
    void createTableDoctors();
    void createTableAppointments();
    void createTablePatients();
    void createTableVisits();
    void createTableMedications();
    void createTablePrescribedMedications();
    void createTableProcedures();
    void createTablePrescribedProcedures();
    void createTableTests();
    void createTableTestResults();
    void createTableDiagnosis();

    void insertAll();
    void insertWeekdays();
    void insertSpecializations();
    void insertDoctors();
    void insertAppointments();
    void insertPatients();
    void insertMedications();
    void insertVisits();
    void insertPrescribedMedications();
    void insertProcedures();
    void insertPrescribedProcedures();
    void insertTests();
    void insertTestResults();
    void insertDiagnosis();

    void updateAll();
    void updateAppointment();
    void updateDiagnosis();
    void updateDoctor();
    void updateMedication();
    void updatePatient();
    void updatePrescribedMedication();
    void updatePrescribedProcedure();
    void updateProcedure();
    void updateSpecialization();
    void updateTestResult();
    void updateTest();
    void updateVisit();
    void updateWeekday();
};