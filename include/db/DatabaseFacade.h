#pragma once

#include "Appointment.h"
#include "Diagnosis.h"
#include "Doctor.h"
#include "Medication.h"
#include "Patient.h"
#include "PrescribedMedication.h"
#include "PrescribedProcedure.h"
#include "Procedure.h"
#include "Specialization.h"
#include "Test.h"
#include "TestResult.h"
#include "Visit.h"
#include "Weekday.h"

#include "AppointmentTDG.h"
#include "DiagnosisTDG.h"
#include "DoctorTDG.h"
#include "MainFactory.h"
#include "MedicationTDG.h"
#include "PatientTDG.h"
#include "PrescribedMedicationTDG.h"
#include "PrescribedProcedureTDG.h"
#include "ProcedureTDG.h"
#include "SpecializationTDG.h"
#include "TestResultTDG.h"
#include "TestTDG.h"
#include "VisitTDG.h"
#include "WeekdayTDG.h"
#include <memory>
#include <unordered_map>

class DatabaseFacade
{
public:
    DatabaseFacade(AppointmentTDG* appointmentTDG, DoctorTDG* doctorTDG,
                   MedicationTDG* medicationTDG, PatientTDG* patientTDG,
                   PrescribedMedicationTDG* prescribedMedicationTDG,
                   PrescribedProcedureTDG* prescribedProcedureTDG,
                   ProcedureTDG* procedureTDG,
                   SpecializationTDG* specializationTDG, VisitTDG* visitTDG,
                   WeekdayTDG* weekdayTDG, TestTDG* testTDG,
                   TestResultTDG* testResultTDG, DiagnosisTDG* diagnosisTDG)
        : appointmentTDG(appointmentTDG), doctorTDG(doctorTDG),
          medicationTDG(medicationTDG), patientTDG(patientTDG),
          prescribedMedicationTDG(prescribedMedicationTDG),
          prescribedProcedureTDG(prescribedProcedureTDG),
          procedureTDG(procedureTDG), specializationTDG(specializationTDG),
          visitTDG(visitTDG), weekdayTDG(weekdayTDG), testTDG(testTDG),
          testResultTDG(testResultTDG), diagnosisTDG(diagnosisTDG)
    {
    }

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

    std::unique_ptr<Appointment> findAppointmentById(int id);
    std::unique_ptr<Weekday> findWeekdayById(int id);
    std::unique_ptr<Specialization> findSpecializationById(int id);
    std::unique_ptr<Doctor> findDoctorById(int id);
    std::unique_ptr<Patient> findPatientById(int id);
    std::unique_ptr<Visit> findVisitById(int id);
    std::unique_ptr<PrescribedMedication> findPrescribedMedicationById(int id);
    std::unique_ptr<Medication> findMedicationById(int id);
    std::unique_ptr<PrescribedProcedure> findPrescribedProcedureById(int id);
    std::unique_ptr<Procedure> findProcedureById(int id);
    std::unique_ptr<TestResult> findTestResultById(int id);
    std::unique_ptr<Test> findTestById(int id);
    std::unique_ptr<Diagnosis> findDiagnosisById(int id);

    std::unordered_map<int, Doctor> selectDoctorAll(int limit, int offset);
    std::unordered_map<int, Specialization> selectSpecializationAll(int limit, int offset);
    std::unordered_map<int, Appointment> selectAppointmentAll(int limit, int offset);
    std::unordered_map<int, Patient> selectPatientAll(int limit, int offset);
    std::unordered_map<int, Visit> selectVisitAll(int limit, int offset);
    std::vector<PrescribedMedication> selectPrMedAll(int limit, int offset);
    std::vector<Medication> selectMedicationAll(int limit, int offset);
    std::vector<PrescribedProcedure> selectPrProcAll(int limit, int offset);
    std::vector<Procedure> selectProcedureAll(int limit, int offset);
    std::vector<TestResult> selectTestResultAll(int limit, int offset);
    std::vector<Test> selectTestAll(int limit, int offset);
    std::vector<Diagnosis> selectDiagnosisAll(int limit, int offset);

    bool updateAppointment(int id, const Appointment& appointment);
    bool updateDiagnosis(int id, const Diagnosis& diagnosis);
    bool updateDoctor(int id, const Doctor& doctor);
    bool updateMedication(int id, const Medication& medication);
    bool updatePatient(int id, const Patient& patient);
    bool updatePrescribedMedication(int id, const PrescribedMedication& preMed);
    bool updatePrescribedProcedure(int id, const PrescribedProcedure& preProc);
    bool updateProcedure(int id, const Procedure& procedure);
    bool updateSpecialization(int id, const Specialization& specialization);
    bool updateTestResult(int id, const TestResult& testResult);
    bool updateTest(int id, const Test& test);
    bool updateVisit(int id, const Visit& visit);
    bool updateWeekday(int id, const Weekday& weekday);

    bool deleteDoctor(int id);
    bool deletePatient(int id);

private:
    MainFactory mainFactory;
    AppointmentTDG* appointmentTDG;
    DoctorTDG* doctorTDG;
    MedicationTDG* medicationTDG;
    PatientTDG* patientTDG;
    PrescribedMedicationTDG* prescribedMedicationTDG;
    PrescribedProcedureTDG* prescribedProcedureTDG;
    ProcedureTDG* procedureTDG;
    SpecializationTDG* specializationTDG;
    VisitTDG* visitTDG;
    WeekdayTDG* weekdayTDG;
    TestTDG* testTDG;
    TestResultTDG* testResultTDG;
    DiagnosisTDG* diagnosisTDG;
};