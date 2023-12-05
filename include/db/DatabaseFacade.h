#pragma once

#include "AppointmentTDG.h"
#include "DoctorTDG.h"
#include "MainFactory.h"
#include "MedicationTDG.h"
#include "PatientTDG.h"
#include "PrescribedMedicationTDG.h"
#include "PrescribedProcedureTDG.h"
#include "ProcedureTDG.h"
#include "SpecializationTDG.h"
#include "VisitTDG.h"
#include "WeekdayTDG.h"
#include "TestTDG.h"
#include "TestResultTDG.h"
#include "DiagnosisTDG.h"

class DatabaseFacade
{
public:
    DatabaseFacade() : hDbc_(nullptr) {}
    DatabaseFacade(SQLHDBC hDbc) : hDbc_(hDbc) {}
    void addWeekday(const std::string& dayName);

    void addSpecialization(const std::string& name);
    void addDoctor(const std::string& lastName, const std::string& firstName,
                   const std::string& middleName, int idSpec);
    void addAppointment(int idDoctor, int idWeekday, const DateTime& beginDate,
                        const DateTime& endDate, int office, int district);
    void addPatient(const std::string& lastName, const std::string& firstName,
                    const std::string& middleName, const std::string& address);
    void addMedication(const std::string& name);
    void addVisit(int idPatient, int idDoctor, const std::string& complaints,
                  const DateTime& dateVisit, const DateTime& dateDischarge,
                  const DateTime& dateClose);
    void addPrescribedMedication(int idVisit, int idMedication);
    void addProcedure(const std::string& name);
    void addPrescribedProcedure(int idVisit, int idProcedure, int count);
    void addTest(const std::string& name);
    void addTestResult(int idVisit, int idTest, const std::string& result);
    void addDiagnosis(int idVisit, const std::string& description);

    void setHdbc(SQLHDBC hdbc) { hDbc_ = hdbc; };

private:
    SQLHDBC hDbc_;
    MainFactory mainFactory;
};